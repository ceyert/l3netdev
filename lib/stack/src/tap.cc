#include <cstring>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <include/tap.h>

namespace tapdev
{

    TAPDev *TAPDev::instance()
    {
        static TAPDev tap_dev_instance(TAP_DEV_NAME);
        return &tap_dev_instance;
    }

    TAPDev::TAPDev(std::string &&dev_name) : addr{TAP_IP_ADDR},
                                             route{TAP_ROUTE}
    {

        this->tap_file_decriptor = TAPDev::alloc(dev_name);
        if (this->tap_file_decriptor < 0)
        {
            std::cerr << "ERR : Allocating tap device \n";
            exit(1);
        }

        std::clog << "TAP Virtual Network Device " << dev_name.c_str() << " allocated. \n";

        TAPDev::exec_iff_up(dev_name);
        TAPDev::exec_iff_address(dev_name);
        TAPDev::exec_iff_route(dev_name);
    }

    TAPDev::~TAPDev()
    {
        close(this->tap_file_decriptor);
    }

    int TAPDev::alloc(std::string &dev_name)
    {
        struct ifreq ifr
        {
        };
        int file_descriptor, err;

        const char *const clone_device = "/dev/net/tun";
        if ((file_descriptor = open(clone_device, O_RDWR)) < 0)
        {
            std::cerr << "ERR : Cannot open TUN/TAP device \n";

            exit(1);
        }

        memset(&ifr, 0, sizeof(ifr));

        /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
         *        IFF_TAP   - TAP device
         *
         *        IFF_NO_PI - Do not provide packet information
         */
        ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
        if (!dev_name.empty())
        {
            strncpy(ifr.ifr_name, dev_name.c_str(), IFNAMSIZ);
        }

        if ((err = ioctl(file_descriptor, TUNSETIFF, (void *)&ifr)) < 0)
        {
            std::cerr << "ERR: Could not ioctl TUN device \n";

            close(file_descriptor);
            return err;
        }

        dev_name.erase();
        dev_name.append(ifr.ifr_name);
        return file_descriptor;
    }

    ssize_t TAPDev::read_device(void *buf, size_t len) const
    {
        return read(tap_file_decriptor, buf, len);
    }

    ssize_t TAPDev::write_device(void *buf, size_t len) const
    {
        return write(tap_file_decriptor, buf, len);
    }

    void TAPDev::exec_iff_up(std::string &dev_name)
    {
        std::stringstream ss;
        ss << "ip link set dev " << dev_name << " up";
        std::clog << "Exec : " << ss.str() << "\n";
        system(ss.str().c_str()); // execute cmd
    }

    void TAPDev::exec_iff_address(std::string &dev_name)
    {
        std::stringstream ss;
        ss << "ip address add dev " << dev_name << " local " << addr;
        std::clog << "Exec : " << ss.str() << "\n";
        system(ss.str().c_str()); // execute cmd
    }

    void TAPDev::exec_iff_route(std::string &dev_name)
    {
        std::stringstream ss;
        ss << "ip route add dev " << dev_name << " " << route;
        std::clog << "Exec : " << ss.str() << "\n";
        system(ss.str().c_str()); // execute cmd
    }

}
