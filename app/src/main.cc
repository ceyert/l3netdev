
#include <memory>
#include <include/netdev.h>
#include <include/packet_processor.h>

int main(int argc, char const *argv[])
{
    std::unique_ptr<tapdev::NetDev> net_device_uptr{std::make_unique<tapdev::NetDev>("10.0.0.1", "cc:f9:e4:9d:c8:3f")};
    std::unique_ptr<tapdev::PacketProcessor> pprocessor_uptr{std::make_unique<tapdev::PacketProcessor>()};

    net_device_uptr->attach(pprocessor_uptr.get());
    net_device_uptr->loop();
    return 0;
}
