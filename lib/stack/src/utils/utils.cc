#include <cstdint>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <include/utils/utils.h>

uint32_t inet_bf(const char *addr) {
    in_addr _internet_addr{};

    if (inet_pton(AF_INET, addr, &_internet_addr) != 1) {
        perror("inet binary formatting failed");
        exit(1);
    }

    return ntohl(_internet_addr.s_addr);
}

std::string inet_pf(uint32_t addr) {
    in_addr _internet_addr{};
    _internet_addr.s_addr = addr;
    return inet_ntoa(_internet_addr);
}
