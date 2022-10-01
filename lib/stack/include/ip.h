#ifndef IP_H
#define IP_H

#include "ethernet.h"
#include "pk_buff.h"

namespace tapdev
{

#define IPv4 0x04
#define ICMPv4 0x01

#define MIN_IP_HDR_SIZE 20
#define IP_HEADER_SIZE(iph) (4 * (iph)->ip_header_len)

    namespace IPUtils
    {
        extern uint16_t checksum(void *addr, int count);
    }

    struct ip_header
    {
        uint8_t ip_header_len : 4 {};
        uint8_t version : 4 {};
        uint8_t type_of_service{};
        uint16_t len{};
        uint16_t id{};
        uint16_t fragment_offset{};
        uint8_t time_to_live{};
        uint8_t protocol{};
        uint16_t cksum{};
        uint32_t source_ip_addr{};
        uint32_t dest_ip_addr{};
        uint8_t data[];
    } __attribute__((packed));

    inline ip_header *get_ip_header(struct ethernet_header *eth)
    {
        return reinterpret_cast<ip_header *>(eth->payload);
    }

    class IP
    {
    public:
        static IP *instance();

    public:
        IP(const IP &) = delete;

        IP(IP &&) = delete;

        const IP &operator=(const IP &) = delete;

        IP operator=(IP &&) = delete;

    public:
        static void recv(package_buff_t &&pkb);

        static void send(package_buff_t &&pkb, uint8_t protocol);

    private:
        explicit IP() = default;

        static void forward(package_buff_t &&pkb);

        static void send_out(package_buff_t &&pkb, uint8_t *hardw_addr);

        static void check_opts(ip_header *iph);
    };

#define _IP() IP::instance()

}

#endif
