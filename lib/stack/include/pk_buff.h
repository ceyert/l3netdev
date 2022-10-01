#ifndef PK_BUFF_H
#define PK_BUFF_H

#include <sys/types.h>
#include <cstring>
#include "route.h"

namespace tapdev
{

#define MTU 1500

    struct package_buff
    {
        explicit package_buff() = default;

        package_buff(const package_buff &other) = delete;

        const package_buff &operator=(const package_buff &other) = delete;

        package_buff(package_buff &&other) noexcept(true)
        {
            len = other.len;
            device_ip_addr = other.device_ip_addr;
            memcpy(payload, other.payload, MTU);
            memcpy(device_hardw_addr, other.device_hardw_addr, 6);
            dest_route_entry = std::move(other.dest_route_entry);

            other.len = 0;
            other.device_ip_addr = 0;
            memset(other.payload, 0, MTU);
            memset(other.device_hardw_addr, 0, 6);
        }

        package_buff &operator=(package_buff &&other) noexcept(true)
        {
            if (this != &other)
            {
                len = other.len;
                device_ip_addr = other.device_ip_addr;
                memcpy(payload, other.payload, MTU);
                memcpy(device_hardw_addr, other.device_hardw_addr, 6);
                dest_route_entry = std::move(other.dest_route_entry);

                other.len = 0;
                other.device_ip_addr = 0;
                memset(other.payload, 0, MTU);
                memset(other.device_hardw_addr, 0, 6);
            }

            return *this;
        }

        ssize_t len{};
        uint8_t payload[MTU]{};
        uint8_t device_hardw_addr[6]{};
        uint32_t device_ip_addr{};
        route_entry dest_route_entry{};
    };

    using package_buff_t = package_buff;

}

#endif
