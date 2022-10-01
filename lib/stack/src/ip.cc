#include <iostream>
#include <cstring>
#include <bitset>
#include <include/ip.h>
#include <include/ethernet.h>
#include <include/icmp.h>
#include <include/arp.h>
#include <include/in.hpp>
#include <include/pk_buff.h>

/*
 * rfc 791
 * https://tools.ietf.org/html/rfc791
 */

namespace tapdev
{

    /* IP Options */
    namespace IPOptions
    {

        static const constexpr uint8_t EOOL = 0x0;
        static const constexpr uint8_t NOP = 0x1;
        static const constexpr uint8_t SEC = 0x82;
        static const constexpr uint8_t LSRR = 0x83;
        static const constexpr uint8_t SSRR = 0x89;
        static const constexpr uint8_t RR = 0x7;
        static const constexpr uint8_t SID = 0x88;
        static const constexpr uint8_t TS = 0x44;

    }

    IP *IP::instance()
    {
        static IP ip_instance;
        return &ip_instance;
    }

    void IP::recv(package_buff_t &&pkb)
    {
        std::clog << "IP receive package.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        ip_header *iph = get_ip_header(eth);

        if (iph->version != IPv4)
        {
            std::cerr << "Version is not IPv4 \n";

            return;
        }

        if (pkb.len < sizeof(ethernet_header) + sizeof(ip_header))
        {
            std::cerr << "IPv4 packet is too small \n";

            return;
        }

        if (iph->ip_header_len < 5)
        {
            std::cerr << "IPv4 header length must be at least 5 \n";

            return;
        }

        if (IP_HEADER_SIZE(iph) < sizeof(ip_header))
        {
            std::cerr << "IPv4 header is too small \n";

            return;
        }

        if (iph->time_to_live == 0)
        {
            std::cerr << "Ip TTL is 0 \n";

            _ICMP()->send(std::move(pkb), TIME_EXCEEDED, 0x00);
            return;
        }

        uint16_t cksum = IPUtils::checksum(iph, IP_HEADER_SIZE(iph));
        if (cksum != 0)
        {
            std::cerr << "IP Invalid Checksum \n";

            return;
        }

        iph->source_ip_addr = stack::in::ntohl(iph->source_ip_addr);
        iph->dest_ip_addr = stack::in::ntohl(iph->dest_ip_addr);
        iph->len = stack::in::ntohs(iph->len);

        if (iph->ip_header_len > 5)
            check_opts(iph);

        route_entry _route_entry = _ROUTE()->lookup(iph->dest_ip_addr);
        pkb.dest_route_entry = std::move(_route_entry);

        /* Is this packet for us */
        if (pkb.dest_route_entry.flags & ROUTE_HOST)
        {
            switch (iph->protocol)
            {
            case ICMPv4:
                _ICMP()->recv(std::move(pkb));
                break;
            default:
                break;
            }
        }
        else
            IP::forward(std::move(pkb));
    }

    void IP::send(package_buff_t &&pkb, uint8_t protocol)
    {
        std::clog << "IP sending package.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        ip_header *iph = get_ip_header(eth);

        // fill header info
        iph->version = IPv4;
        iph->ip_header_len = 0x05;
        iph->type_of_service = 0;
        iph->fragment_offset = 0x4000;
        iph->time_to_live = 64;
        iph->protocol = protocol;
        iph->cksum = 0;

        // swap source_ip_addr dest_ip_addr
        iph->source_ip_addr ^= iph->dest_ip_addr;
        iph->dest_ip_addr ^= iph->source_ip_addr;
        iph->source_ip_addr ^= iph->dest_ip_addr;

        if (pkb.dest_route_entry.flags & ROUTE_LOOPBACK)
        {
            std::clog << "To loopback. \n";

            IP::send_out(std::move(pkb), pkb.device_hardw_addr);
            return;
        }

        if (pkb.dest_route_entry.flags & ROUTE_GATEWAY)
        {
            std::clog << "To route gateway. \n";

            iph->dest_ip_addr = pkb.dest_route_entry.gateway;

            arp_cache_t arp_cache = _ARP()->arp_cache_lookup(iph->dest_ip_addr);
            if (arp_cache)
                IP::send_out(std::move(pkb), arp_cache.sender_hardw_addr);
            else
                _ARP()->request(std::move(pkb), iph->source_ip_addr, iph->dest_ip_addr);
        }
    }

    void IP::forward(package_buff_t &&pkb)
    {
        std::clog << "IP forwarding.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        ip_header *iph = get_ip_header(eth);

        iph->source_ip_addr = stack::in::htonl(iph->source_ip_addr);
        iph->dest_ip_addr = stack::in::htonl(iph->dest_ip_addr);
        iph->len = stack::in::htons(iph->len);

        if (iph->time_to_live <= 1)
        {
            _ICMP()->send(std::move(pkb), TIME_EXCEEDED, 0x00);
            return;
        }

        iph->time_to_live--;

        iph->dest_ip_addr = pkb.dest_route_entry.gateway;

        IP::send(std::move(pkb), iph->protocol);
    }

    void IP::send_out(package_buff_t &&pkb, uint8_t *hardw_addr)
    {
        std::clog << "IP sending package out.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        ip_header *iph = get_ip_header(eth);

        iph->len = stack::in::htons(iph->len);
        iph->dest_ip_addr = stack::in::htonl(iph->dest_ip_addr);
        iph->source_ip_addr = stack::in::htonl(iph->source_ip_addr);
        iph->fragment_offset = stack::in::htons(iph->fragment_offset);

        iph->cksum = IPUtils::checksum(iph, IP_HEADER_SIZE(iph));
        _ETH()->xmit(std::move(pkb), hardw_addr, pkb.device_hardw_addr, pkb.len, ETH_P_IP);
    }

    void IP::check_opts(ip_header *iph)
    {
        int opts_count = IP_HEADER_SIZE(iph) - MIN_IP_HDR_SIZE;
        uint8_t *options = reinterpret_cast<uint8_t *>(iph->data);

        for (int i = 0; i < opts_count; ++i)
        {
            switch (*(options + i) & 0xffu)
            {
            case IPOptions::LSRR:
            {
                uint8_t dst[4];
                memcpy(dst, (options + i) + 3, 4);

                std::string dst0 = std::bitset<8>(dst[0]).to_string();
                std::string dst1 = std::bitset<8>(dst[1]).to_string();
                std::string dst2 = std::bitset<8>(dst[2]).to_string();
                std::string dst3 = std::bitset<8>(dst[3]).to_string();
                std::string bf = dst0.append(dst1).append(dst2).append(dst3);

                iph->dest_ip_addr = std::bitset<32>(bf).to_ulong();
                break;
            }
            case IPOptions::EOOL:
            case IPOptions::NOP:
            case IPOptions::SEC:
            case IPOptions::SSRR:
            case IPOptions::RR:
            case IPOptions::SID:
            case IPOptions::TS:
            default:
                break;
            }
        }
    }

    namespace IPUtils
    {

        uint16_t checksum(void *addr, int count)
        {

            /* Compute Internet Checksum for "count" bytes
             *    beginning at location "addr".
             *    https://tools.ietf.org/html/rfc1071
             */

            uint32_t sum = 0;

            uint16_t *ptr_16 = static_cast<uint16_t *>(addr);

            for (; count > 1; count -= 2)
            {
                // This is the inner loop
                sum += *ptr_16++;
            }

            // Add left-over byte, if any
            if (count > 0)
                sum += *static_cast<uint8_t *>(addr);

            // Fold 32-bit sum to 16 bits
            while (sum >> 16u)
                sum = (sum & 0xffffu) + (sum >> 16u);

            return ~sum;
        }
    }

}
