#include <iostream>
#include "include/packet_processor.h"
#include <include/ethernet.h>
#include <include/arp.h>
#include <include/in.hpp>
#include <include/ip.h>

namespace tapdev
{

    PacketProcessor::PacketProcessor() : _tid{[this]()
                                              { this->worker(); }},
                                         _stop{false} {}

    PacketProcessor::~PacketProcessor()
    {
        std::lock_guard<std::mutex> _lock_guard(_packet_processor_mutex);
        this->_stop = true;
        this->_tid.join();
    }

    void PacketProcessor::update(package_buff_t &&pkt)
    {
        this->pkt_concurr_queue.push_back(std::move(pkt));
    }

    void PacketProcessor::worker()
    {
        while (!this->_stop || !this->pkt_concurr_queue.empty())
        {
            if (this->pkt_concurr_queue.empty())
                continue;

            package_buff_t pkb;
            this->pkt_concurr_queue.wait_and_pop(pkb);

            ethernet_header *eth = get_ethernet_header(pkb.payload);
            uint16_t type = stack::in::ntohs(eth->type);

            switch (type)
            {
            case ETH_P_ARP:
                _ARP()->recv(std::move(pkb));
                break;
            case ETH_P_IP:
                _IP()->recv(std::move(pkb));
                break;
            default:
                break;
            }
        }
    }
}
