#include <sys/epoll.h>
#include <cstring>
#include <array>
#include <include/netdev.h>
#include <include/utils/utils.h>
#include <include/tap.h>
#include <include/pk_buff.h>

namespace tapdev
{

    NetDev::NetDev(const char *addr, const char *const hardw_addr) : addr(inet_bf(addr))
    {

        std::sscanf(hardw_addr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                    &this->hardw_addr[0],
                    &this->hardw_addr[1],
                    &this->hardw_addr[2],
                    &this->hardw_addr[3],
                    &this->hardw_addr[4],
                    &this->hardw_addr[5]);

        epoll_fd = epoll_create1(0);
        if (epoll_fd < 0)
        {
            std::cerr << "epoll_create1() \n";

            exit(EXIT_FAILURE);
        }

        struct epoll_event event
        {
        };
        memset(&event, 0, sizeof(event));
        event.data.fd = _TAPD()->get_file_desc();
        event.events = EPOLLIN;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) < 0)
        {
            std::cerr << "epoll_ctl() \n";

            close(epoll_fd);
            exit(EXIT_FAILURE);
        }
    }

    void NetDev::loop()
    {
        std::array<epoll_event, MAX_EVENTS> events{};

        while (true)
        {
            int nevents = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
            if (nevents < 0)
            {
                if (errno == EINTR)
                    continue;
            }
            for (int i = 0; i < nevents; ++i)
            {
                if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP ||
                    !(events[i].events & EPOLLIN))
                {

                    std::cerr << "epoll event error\n";
                    close(events[i].data.fd);
                }
                else if (events[i].data.fd == _TAPD()->get_file_desc())
                {
                    package_buff_t package{};
                    ssize_t nread = _TAPD()->read_device(package.payload, MTU);

                    if (nread < 0)
                    {
                        std::cerr << "Reading from interface\n";
                        exit(1);
                    }

                    package.len = nread;
                    package.device_ip_addr = this->addr;
                    memcpy(package.device_hardw_addr, this->hardw_addr, 6);

                    this->notify(std::move(package)); // notify packet processor
                }
            }
        }
    }

    NetDev::~NetDev()
    {
        close(epoll_fd);
    }

}