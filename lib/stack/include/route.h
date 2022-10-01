#ifndef ROUTE_H
#define ROUTE_H

#include <cstdint>
#include <vector>

namespace tapdev
{

#define ROUTE_LOOPBACK 0x01
#define ROUTE_GATEWAY 0x02
#define ROUTE_HOST 0x04

    struct route_entry
    {
        route_entry() = default;

        route_entry(uint32_t destination, uint32_t gateway, uint32_t netmask, uint8_t flags, uint32_t metric)
            : destination{destination},
              gateway{gateway},
              netmask{netmask},
              flags{flags},
              metric{metric} {}

        route_entry(const route_entry &other)
        {
            destination = other.destination;
            gateway = other.gateway;
            netmask = other.netmask;
            flags = other.flags;
            metric = other.metric;
        }

        const route_entry &operator=(const route_entry &other) noexcept(true)
        {
            if (this != &other)
            {
                destination = other.destination;
                gateway = other.gateway;
                netmask = other.netmask;
                flags = other.flags;
                metric = other.metric;
            }

            return *this;
        }

        route_entry(route_entry &&other) noexcept(true)
        {
            destination = other.destination;
            gateway = other.gateway;
            netmask = other.netmask;
            flags = other.flags;
            metric = other.metric;

            other.destination = 0;
            other.gateway = 0;
            other.netmask = 0;
            other.flags = 0;
            other.metric = 0;
        }

        route_entry &operator=(route_entry &&other) noexcept(true)
        {
            if (this != &other)
            {
                destination = other.destination;
                gateway = other.gateway;
                netmask = other.netmask;
                flags = other.flags;
                metric = other.metric;

                other.destination = 0;
                other.gateway = 0;
                other.netmask = 0;
                other.flags = 0;
                other.metric = 0;
            }

            return *this;
        }

        uint32_t destination{};
        uint32_t gateway{};
        uint32_t netmask{};
        uint8_t flags{};
        uint32_t metric{};
    };

    class ROUTE
    {
    public:
        static ROUTE *instance();

    public:
        ROUTE(const ROUTE &) = delete;

        ROUTE(ROUTE &&) = delete;

        const ROUTE &operator=(const ROUTE &) = delete;

        ROUTE operator=(ROUTE &&) = delete;

    public:
        route_entry lookup(uint32_t dest_addr);

    private:
        explicit ROUTE();

        std::vector<route_entry> _route_entries{};
    };

#define _ROUTE() ROUTE::instance()

}

#endif
