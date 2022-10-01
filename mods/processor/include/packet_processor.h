#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <thread>
#include <mutex>
#include <queue>

#include "include/pk_buff.h"
#include "include/utils/observer.hpp"
#include "include/utils/concurrent_queue.hpp"

namespace tapdev
{

    class PacketProcessor final : public Observer<package_buff_t &&>
    {
    public:
        PacketProcessor();

        ~PacketProcessor();

        void update(package_buff_t &&data) override;

    private:
        void worker();

        std::thread _tid{};
        std::mutex _packet_processor_mutex{};
        bool _stop{false};
        concurrent_queue<package_buff_t> pkt_concurr_queue{};
    };
}

#endif
