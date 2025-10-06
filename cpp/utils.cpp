#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <cstdio>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

std::string getMacAddress() {
    ULONG bufferSize = 0;
    GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL, &bufferSize);
    std::vector<BYTE> buffer(bufferSize);
    PIP_ADAPTER_ADDRESSES pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());

    if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &bufferSize) == NO_ERROR) {
        for (; pAddresses != NULL; pAddresses = pAddresses->Next) {
            if (pAddresses->PhysicalAddressLength != 0 && pAddresses->IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
                char macStr[18];
                sprintf_s(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
                    pAddresses->PhysicalAddress[0], pAddresses->PhysicalAddress[1],
                    pAddresses->PhysicalAddress[2], pAddresses->PhysicalAddress[3],
                    pAddresses->PhysicalAddress[4], pAddresses->PhysicalAddress[5]);
                return std::string(macStr);
            }
        }
    }
    return "";
}

#elif defined(__APPLE__) || defined(__linux__)
#include <fstream>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <netpacket/packet.h>
#include <chrono>
#include <cmath>

std::string getMacAddress() {
    struct ifaddrs *ifaddr, *ifa;
    int s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return "";
    }

    std::string macAddress = "";
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET) {
            continue;
        }

        std::string interfaceName(ifa->ifa_name);
        if (interfaceName == "lo") {
            continue;
        }
        
        struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
        std::stringstream ss;
        for (int i = 0; i < s->sll_halen; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)s->sll_addr[i];
            if (i < s->sll_halen - 1) ss << ":";
        }
        macAddress = ss.str();
        if (!macAddress.empty()) {
            break; // Found the first non-loopback MAC address
        }
    }

    freeifaddrs(ifaddr);
    return macAddress;
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

long days_between(const std::string& date1_str, const std::string& date2_str) {
    std::tm tm1 = {};
    std::tm tm2 = {};
    std::stringstream ss1(date1_str);
    std::stringstream ss2(date2_str);

    ss1 >> std::get_time(&tm1, "%Y-%m-%d");
    ss2 >> std::get_time(&tm2, "%Y-%m-%d");

    auto time1 = std::chrono::system_clock::from_time_t(std::mktime(&tm1));
    auto time2 = std::chrono::system_clock::from_time_t(std::mktime(&tm2));

    auto hours = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<3600>>>(time2 - time1).count();
    return static_cast<long>(std::round(hours / 24.0));
}

#else
std::string getMacAddress() {
    std::cerr << "Unsupported operating system for getting MAC address." << std::endl;
    return "";
}
#endif
