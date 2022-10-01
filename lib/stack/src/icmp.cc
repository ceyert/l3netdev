#include <iostream>
#include <cstring>
#include <include/ip.h>
#include <include/in.hpp>
#include <include/icmp.h>
#include <include/pk_buff.h>

/*
 * rfc 792
 * https://tools.ietf.org/html/rfc792
 */

namespace tapdev
{

    ICMP *ICMP::instance()
    {
        static ICMP icmp_instance;
        return &icmp_instance;
    }

    void ICMP::recv(package_buff_t &&pkb)
    {
        std::clog << "ICMP receive package.."
                  << "\n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        ip_header *iph = get_ip_header(eth);
        icmp_header *icmph = get_icmp_header(iph);

        int icmp_len{iph->len - IP_HEADER_SIZE(iph)};

        uint16_t cksum = IPUtils::checksum(icmph, icmp_len);
        if (cksum != 0)
        {
            std::cerr << "ICMP Invalid Checksum \n";
            return;
        }

        switch (icmph->type)
        {
        case ECHO_REQUEST:
            std::clog << "ICMP ECHO REQUEST message received \n";
            ICMP::send(std::move(pkb), ECHO_REPLY, 0x00);
            break;
        case ECHO_REPLY:
            std::clog << "ICMP ECHO REPLY message received \n";
            break;
        case DST_UNREACHABLE:
            std::clog << "ICMP DESTINATION UNREACHABLE message received \n";
            break;
        case TIME_EXCEEDED:
            std::clog << "ICMP TIME EXCEEDED message received \n";
            break;
        default:
            break;
        }
    }

    void ICMP::send(package_buff_t &&pkb, uint8_t type, uint8_t code)
    {
        std::clog << "ICMP sending package.. \n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        ip_header *iph = get_ip_header(eth);
        icmp_header *icmph = get_icmp_header(iph);

        // the first 64 bits of the original datagram's data
        uint8_t f64[8];
        memcpy(f64, icmph, 8);

        // Internet Header
        uint8_t iphdr[IP_HEADER_SIZE(iph)];
        memcpy(iphdr, iph, IP_HEADER_SIZE(iph));

        icmph = reinterpret_cast<icmp_header *>((iph)->data);
        icmph->type = type;
        icmph->code = code;
        icmph->cksum = 0;

        if (type == TIME_EXCEEDED)
        {
            uint8_t *ptr = icmph->data;
            *ptr = 0x0000;

            memcpy(ptr + 4, iphdr, IP_HEADER_SIZE(iph));
            memcpy(ptr + 4 + IP_HEADER_SIZE(iph), f64, 8);

            iph->source_ip_addr = stack::in::ntohl(iph->source_ip_addr);
            iph->dest_ip_addr = pkb.device_ip_addr;
            iph->len = stack::in::ntohs(iph->len);
        }

        int icmp_len{iph->len - MIN_IP_HDR_SIZE};
        icmph->cksum = IPUtils::checksum(icmph, icmp_len);

        _IP()->send(std::move(pkb), ICMPv4);
    }

    inline icmp_header *ICMP::get_icmp_header(ip_header *iph)
    {
        int offset{IP_HEADER_SIZE(iph) - MIN_IP_HDR_SIZE};
        return reinterpret_cast<icmp_header *>(iph->data + offset);
    }

}
