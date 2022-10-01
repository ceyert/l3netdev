#ifndef ETHERNET_H
#define ETHERNET_H

#include "pk_buff.h"

namespace tapdev
{

#define ETH_P_ARP 0x0806
#define ETH_P_IP 0x0800

    struct ethernet_header
    {
        uint8_t dest_mac[6]{};
        uint8_t source_mac[6]{};
        uint16_t type{};
        uint8_t payload[];
    } __attribute__((packed));

    class ETH
    {
    public:
        static ETH *instance();

    public:
        ETH(const ETH &) = delete;

        ETH(ETH &&) = delete;

        const ETH &operator=(const ETH &) = delete;

        ETH operator=(ETH &&) = delete;

    public:
        static void xmit(package_buff_t &&pkb,
                         const uint8_t *const dst_hwaddr,
                         const uint8_t *const hardw_addr,
                         ssize_t len,
                         uint16_t type);

    private:
        explicit ETH() = default;
    };

    inline ethernet_header *get_ethernet_header(void *buff)
    {
        return reinterpret_cast<ethernet_header *>(buff);
    }

#define _ETH() ETH::instance()

}

#endif
