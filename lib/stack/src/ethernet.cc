#include <cstring>
#include <include/ethernet.h>
#include <include/in.hpp>
#include <include/tap.h>

namespace tapdev
{

    ETH *ETH::instance()
    {
        static ETH eth_instance;
        return &eth_instance;
    }

    void ETH::xmit(package_buff_t &&pkb,
                   const uint8_t *const dst_hwaddr,
                   const uint8_t *const hardw_addr,
                   ssize_t len,
                   uint16_t type)
    {
        std::clog << "ETH xmit frame.."
                  << "\n";

        ethernet_header *eth = get_ethernet_header(pkb.payload);
        eth->type = stack::in::htons(type);
        memcpy(eth->dest_mac, dst_hwaddr, 6);
        memcpy(eth->source_mac, hardw_addr, 6);

        _TAPD()->write_device(pkb.payload, len);
    }

}
