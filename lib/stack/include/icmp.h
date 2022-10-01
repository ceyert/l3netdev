#ifndef ICMP_H
#define ICMP_H

#include <cstdint>
#include "pk_buff.h"
#include "ip.h"

namespace tapdev
{

#define ECHO_REPLY 0x00
#define DST_UNREACHABLE 0x03
#define ECHO_REQUEST 0x08
#define TIME_EXCEEDED 0x0b
#define MALFORMED 0x0c

    struct icmp_header
    {
        uint8_t type{};
        uint8_t code{};
        uint16_t cksum{};
        uint8_t data[];
    } __attribute__((packed));

    class ICMP
    {
    public:
        static ICMP *instance();

    public:
        ICMP(const ICMP &) = delete;

        ICMP(ICMP &&) = delete;

        const ICMP &operator=(const ICMP &) = delete;

        ICMP operator=(ICMP &&) = delete;

    public:
        static void recv(package_buff_t &&pkb);

        static void send(package_buff_t &&pkb, uint8_t type, uint8_t code);

    private:
        explicit ICMP() = default;

        static inline icmp_header *get_icmp_header(ip_header *iph);
    };

#define _ICMP() ICMP::instance()

}

#endif
