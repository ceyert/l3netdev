#include <include/utils/utils.h>
#include <include/route.h>

namespace tapdev
{

    ROUTE *ROUTE::instance()
    {
        static ROUTE route_instance;
        return &route_instance;
    }

    ROUTE::ROUTE()
    {
        route_entry route_host{inet_bf("10.0.0.1"), 0, 0xffffff00, ROUTE_HOST, 0};
        this->_route_entries.emplace_back(std::move(route_host));

        route_entry route_loopback{inet_bf("127.0.0.1"), 0, 0xff000000, ROUTE_LOOPBACK, 0};
        this->_route_entries.emplace_back(std::move(route_loopback));

        route_entry gateway{0, inet_bf("10.0.0.5"), 0, ROUTE_GATEWAY, 0};
        this->_route_entries.emplace_back(std::move(gateway));
    }

    route_entry ROUTE::lookup(uint32_t dest_ip_addr)
    {
        std::clog << "ROUTE lookup.. \n";

        for (route_entry &_route_entry : this->_route_entries)
        {
            if ((dest_ip_addr & _route_entry.netmask) == (_route_entry.destination & _route_entry.netmask))
                return _route_entry;
        }
        return {};
    }

}
