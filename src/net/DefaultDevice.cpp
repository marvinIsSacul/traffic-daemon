#include "DefaultDevice.h"


namespace net{


DefaultDevice::DefaultDevice(): alldevs{nullptr}
{
    //ctor
}

DefaultDevice::~DefaultDevice()
{
    clear();
}

void DefaultDevice::clear(){
    if (alldevs != nullptr){
        /* We don't need any more the device list. Free it */
        pcap_freealldevs(alldevs);
        alldevs = nullptr;
    }
    this->error.clear();
}

pcap_if_t* DefaultDevice::getDefaultDevice() {

    char errbuf[PCAP_ERRBUF_SIZE + 1];

    clear();

    /* Retrieve the device list from the local machine */
    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, nullptr /* auth is not needed */, &this->alldevs, errbuf) == -1){
        this->error = errbuf;
        return nullptr;
    }

    /* Print the list */
    for(auto d = this->alldevs; d != nullptr; d = d->next){
        return d;
    }

    return nullptr;
}

std::string const &DefaultDevice::getError() noexcept{
    return this->error;
}

std::vector<std::string> DefaultDevice::getDeviceNames(){
    std::vector<std::string> deviceNames;
    char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
    pcap_if_t *alldevs = nullptr;

    /* Retrieve the device list from the local machine */
    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, nullptr /* auth is not needed */, &alldevs, errbuf) == -1){
        this->error = errbuf;
    }

    /* Print the list */
    for(auto d = alldevs; d != nullptr; d = d->next){
        deviceNames.push_back(d->name);
    }

    if (alldevs != nullptr){
        pcap_freealldevs(alldevs);
    }

    return deviceNames;
}

}
