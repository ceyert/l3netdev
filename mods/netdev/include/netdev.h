#ifndef NETDEV_H
#define NETDEV_H

#include <cstdint>
#include <unistd.h>
#include "include/pk_buff.h"
#include "include/utils/observer.hpp"

namespace tapdev
{

#define MAX_EVENTS 32

    class NetDev : public Subject<package_buff_t &&>
    {
    public:
        NetDev(const char *addr, const char *const hardw_addr);
        ~NetDev();
        void loop();

    private:
        uint32_t addr{};
        uint8_t hardw_addr[6]{};
        int epoll_fd{};
    };
}

#endif
