#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <cstdint>
#include <string>
#include <sstream>
#include <ios>

#include "protocol_header.h"


namespace net{

inline std::string ipString(uint32_t ip){
    uint8_t *ptr = (uint8_t*)(&ip);
    std::stringstream ss;

    ss << int(ptr[0]) << '.'
        << int(ptr[1]) << '.'
        << int(ptr[2]) << '.'
        << int(ptr[3]);

    return ss.str();
}

inline std::string ipString(const IpAddress &ip){
    return ipString(ip.address);
}

inline bool isIpPublic(const IpAddress &ip){
    if (ip.oct[0] == 10)                                              // class A private
        return false;
    else if (ip.oct[0] == 172 && ip.oct[1] >= 16 && ip.oct[1] <= 31)      // class B private
        return false;
    else if (ip.oct[0] == 192 && ip.oct[1] == 168)                      // class C private
        return false;

    return true;
}

inline bool isIpPrivate(const IpAddress &ip){
    return !isIpPublic(ip);
}

inline std::string macString(const uint8_t mac[6]){
    std::stringstream ss;
    ss
        << std::hex << (short)mac[0] << ':'
        << std::hex << (short)mac[1] << ':'
        << std::hex << (short)mac[2] << ':'
        << std::hex << (short)mac[3] << ':'
        << std::hex << (short)mac[4] << ':'
        << std::hex << (short)mac[5];

    return ss.str();
}

}


#endif // UTIL_H_INCLUDED
