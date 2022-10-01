#ifndef ARP_H
#define ARP_H

#include <map>
#include <thread>
#include <mutex>
#include "include/utils/concurrent_map.hpp"
#include "ethernet.h"
#include "pk_buff.h"

#define ARP_ETHERNET 0x0001
#define ARP_REQUEST 0x0001
#define ARP_IPV4 0x0800
#define ARP_REPLY 0x0002

namespace tapdev
{

    struct arp_header
    {
        uint16_t hardw{};
        uint16_t protocol{};
        uint8_t hardw_addr_len{};
        uint8_t protocol_addr_len{};
        uint16_t op_code{};
        uint8_t sender_hardw_addr[6]{};
        uint32_t sender_ip_addr{};
        uint8_t target_hardw_addr[6]{};
        uint32_t target_ip_addr{};
    } __attribute__((packed));

    struct arp_cache
    {
        arp_cache()
            : protocol{0}, time{0}
        {
            memset(this->sender_hardw_addr, 0, 6);
        }

        arp_cache(const arp_cache &other) noexcept(true)
        {
            memcpy(this->sender_hardw_addr, other.sender_hardw_addr, 6);
            this->protocol = other.protocol;
            this->time = other.time;
        }

        const arp_cache &operator=(const arp_cache &other) noexcept(true)
        {
            if (this != &other)
            {
                memcpy(this->sender_hardw_addr, other.sender_hardw_addr, 6);
                this->protocol = other.protocol;
                this->time = other.time;
            }

            return *this;
        }

        arp_cache(arp_cache &&other) noexcept(true)
        {
            memcpy(this->sender_hardw_addr, other.sender_hardw_addr, 6);
            this->protocol = other.protocol;
            this->time = other.time;

            memset(other.sender_hardw_addr, 0, 6);
            other.protocol = 0;
            other.time = 0;
        }

        explicit operator bool() const
        {
            return (this->sender_hardw_addr[0] != 0 || this->sender_hardw_addr[1] != 0 || this->sender_hardw_addr[2] != 0 ||
                    this->sender_hardw_addr[3] != 0 || this->sender_hardw_addr[4] != 0 || this->sender_hardw_addr[5] != 0);
        }

        uint8_t sender_hardw_addr[6]{};
        uint16_t protocol{0};
        time_t time{0};
    };

    using arp_cache_t = arp_cache;

    struct arp_cache_timer
    {
        std::thread thread_id{};
        std::mutex cache_mutex{};
        int timeout{};
        bool stop{};
    };

    using arp_cache_timer_t = arp_cache_timer;

    class ARP
    {
    public:
        static ARP *instance();

    public:
        ARP(const ARP &) = delete;

        ARP(ARP &&) = delete;

        const ARP &operator=(const ARP &) = delete;

        ARP operator=(ARP &&) = delete;

    public:
        void recv(package_buff_t &&pkb);

        arp_cache_t arp_cache_lookup(uint32_t addr);

    public:
        static void request(package_buff_t &&, uint32_t addr, uint32_t target_ip_addr);

    private:
        explicit ARP();

        ~ARP();

    private:
        void arp_cache_update(uint32_t addr, uint8_t *sender_hardw_addr);

        void create_arp_cache_entry(uint32_t addr, uint16_t protocol, uint8_t *sender_hardw_addr);

    private:
        arp_cache_timer_t _arp_cache_timer{};

        concurrent_map<uint32_t, arp_cache_t> _arp_cache_concurr_map{};

    private:
        static void reply(package_buff_t &&pkb);

        static void arp_cache_expire_worker(void *contex);

        static inline arp_header *get_arp_header(ethernet_header *eth);
    };

#define _ARP() ARP::instance()

}

#endif
