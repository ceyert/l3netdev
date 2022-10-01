#include <iostream>
#include <cstring>
#include <include/ethernet.h>
#include <include/arp.h>
#include <include/in.hpp>
#include <include/pk_buff.h>

/*
 * rfc 826
 * https://tools.ietf.org/html/rfc826
 */

namespace tapdev
{

    static const uint8_t HW_BROADCAST[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    ARP *ARP::instance()
    {
        static ARP arp_instance;
        return &arp_instance;
    }

    ARP::ARP()
    {
        this->_arp_cache_timer.stop = false;
        this->_arp_cache_timer.timeout = 5;
        this->_arp_cache_timer.thread_id = std::thread(&ARP::arp_cache_expire_worker, this);
    }

    ARP::~ARP()
    {
        std::lock_guard<std::mutex> _lock_guard{this->_arp_cache_timer.cache_mutex};
        this->_arp_cache_timer.stop = true;
        this->_arp_cache_timer.thread_id.join();
    }

    void ARP::recv(package_buff_t &&pkb)
    {
        std::clog << "ARP receive package.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        arp_header *arph = this->get_arp_header(eth);

        if (pkb.len < sizeof(ethernet_header) + sizeof(arp_header))
        {
            std::cerr << "ARP packet is too small \n";
            return;
        }

        arph->hardw = stack::in::ntohs(arph->hardw);
        arph->protocol = stack::in::ntohs(arph->protocol);
        arph->op_code = stack::in::ntohs(arph->op_code);
        arph->target_ip_addr = stack::in::ntohl(arph->target_ip_addr);
        arph->sender_ip_addr = stack::in::ntohl(arph->sender_ip_addr);

        if (arph->hardw != ARP_ETHERNET && arph->hardw_addr_len != 0x06)
            return;

        if (arph->protocol != ARP_IPV4 && arph->protocol_addr_len != 0x04)
            return;

        bool merge{false};
        arp_cache_t arp_cache_found = this->arp_cache_lookup(arph->sender_ip_addr);
        if (arp_cache_found)
        {
            this->arp_cache_update(arph->sender_ip_addr, arph->sender_hardw_addr);
            merge = true;
        }

        if (pkb.device_ip_addr == arph->target_ip_addr)
        {
            if (!merge)
                this->create_arp_cache_entry(arph->sender_ip_addr, arph->protocol, arph->sender_hardw_addr);
        }
        else
            return;

        switch (arph->op_code)
        {
        case ARP_REQUEST:
            std::clog << "Got 1 ARP Request"
                      << "\n";
            this->reply(std::move(pkb));
            break;
        default:
            break;
        }
    }

    void ARP::reply(package_buff_t &&pkb)
    {
        std::clog << "ARP reply.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        arp_header *arph = get_arp_header(eth);

        memcpy(arph->target_hardw_addr, arph->sender_hardw_addr, 6);
        arph->target_ip_addr = arph->sender_ip_addr;

        eth->type = stack::in::htons(eth->type);

        memcpy(arph->sender_hardw_addr, pkb.device_hardw_addr, 6);
        arph->sender_ip_addr = stack::in::htonl(pkb.device_ip_addr);

        arph->op_code = stack::in::htons(ARP_REPLY);

        arph->hardw = stack::in::htons(arph->hardw);
        arph->protocol = stack::in::htons(arph->protocol);

        _ETH()->xmit(std::move(pkb), arph->target_hardw_addr, arph->sender_hardw_addr, pkb.len, ETH_P_ARP);
    }

    void ARP::request(package_buff_t &&pkb, uint32_t sender_ip_addr, uint32_t target_ip_addr)
    {
        std::clog << "ARP request.. \n";

        auto eth = get_ethernet_header(pkb.payload);
        auto arph = get_arp_header(eth);

        memcpy(arph->sender_hardw_addr, pkb.device_hardw_addr, 6);
        arph->sender_ip_addr = sender_ip_addr;

        memcpy(arph->target_hardw_addr, HW_BROADCAST, 6);
        arph->target_ip_addr = target_ip_addr;

        arph->op_code = stack::in::htons(ARP_REQUEST);
        arph->hardw = stack::in::htons(ARP_ETHERNET);
        arph->protocol = stack::in::htons(ARP_IPV4);
        arph->hardw_addr_len = 0x06;
        arph->protocol_addr_len = 0x04;

        arph->sender_ip_addr = stack::in::htonl(arph->sender_ip_addr);
        arph->target_ip_addr = stack::in::htonl(arph->target_ip_addr);

        size_t len = sizeof(arp_header) + sizeof(ethernet_header);

        _ETH()->xmit(std::move(pkb), arph->target_hardw_addr, arph->sender_hardw_addr, len, ETH_P_ARP);
    }

    void ARP::arp_cache_expire_worker(void *contex)
    {
        ARP *ctx = static_cast<ARP *>(contex);

        while (!ctx->_arp_cache_timer.stop)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            time_t now{time(nullptr)};

            for (auto iter_base = ctx->_arp_cache_concurr_map.cbegin(); iter_base != ctx->_arp_cache_concurr_map.cend();)
            {
                if (difftime(now, iter_base->second.time) > ctx->_arp_cache_timer.timeout)
                    ctx->_arp_cache_concurr_map.erase(iter_base++);
                else
                    ++iter_base;
            }
        }
    }

    arp_cache_t ARP::arp_cache_lookup(uint32_t addr)
    {
        arp_cache_t found{this->_arp_cache_concurr_map.find(addr)};
        if (found)
            return found;
        return {};
    }

    void ARP::arp_cache_update(uint32_t addr, uint8_t *sender_hardw_addr)
    {
        arp_cache_t cache{};
        memcpy(cache.sender_hardw_addr, sender_hardw_addr, 6);
        cache.time = time(nullptr);
        this->_arp_cache_concurr_map.update(addr, std::move(cache));
    }

    void ARP::create_arp_cache_entry(uint32_t addr, uint16_t protocol, uint8_t *sender_hardw_addr)
    {
        arp_cache_t cache{};
        cache.protocol = protocol;
        cache.time = time(nullptr);
        memcpy(cache.sender_hardw_addr, sender_hardw_addr, 6);
        this->_arp_cache_concurr_map.insert(addr, std::move(cache));
    }

    inline arp_header *ARP::get_arp_header(ethernet_header *eth)
    {
        return reinterpret_cast<arp_header *>(eth->payload);
    }

}
