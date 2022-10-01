#ifndef TAP_H
#define TAP_H

#include <string>
#include <unistd.h>
#include <iostream>

namespace tapdev
{

#define TAP_DEV_NAME "tap_dev0"
#define TAP_IP_ADDR "10.0.0.5"
#define TAP_ROUTE "10.0.0.0/24"

    class TAPDev
    {
    public:
        static TAPDev *instance();

    public:
        TAPDev(const TAPDev &) = delete;

        TAPDev(const TAPDev &&) = delete;

        const TAPDev &operator=(const TAPDev &) = delete;

        TAPDev operator=(TAPDev &&) = delete;

    public:
        ssize_t read_device(void *buf, size_t len) const;

        ssize_t write_device(void *buf, size_t len) const;

        int get_file_desc() const { return tap_file_decriptor; }

    private:
        explicit TAPDev(std::string &&dev);

        ~TAPDev();

    private:
        int tap_file_decriptor{};
        std::string addr{};
        std::string route{};

        static int alloc(std::string &dev);

        void exec_iff_route(std::string &dev);

        void exec_iff_address(std::string &dev);

        void exec_iff_up(std::string &dev);
    };

#define _TAPD() TAPDev::instance()

}

#endif
