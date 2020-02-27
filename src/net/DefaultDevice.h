#ifndef DEFAULTDEVICE_H
#define DEFAULTDEVICE_H

#include <string>
#include <vector>

#include <winsock2.h>
#define HAVE_REMOTE
#include <pcap.h>


namespace net{


/**
 * Abstracts default network device of PCap.
*/
class DefaultDevice{

private:
    pcap_if_t *alldevs;
    std::string error;

    void clear();

public:

    DefaultDevice();

    ~DefaultDevice();

    /**
     * Gets a default device.
    */
    pcap_if_t *getDefaultDevice();

    std::vector<std::string> getDeviceNames();

    const std::string &getError() noexcept;
};

}


#endif // DEFAULTDEVICE_H
