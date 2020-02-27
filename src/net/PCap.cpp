#include "PCap.h"

#include <ctime>
#include <cstdint>
#include <string>

#include "protocol_header.h"



namespace net{

PCap::PCap() noexcept : pcap{nullptr}, allDevices{nullptr}, device{nullptr}, errorBuf{""}, packetHandler{nullptr}
{

}

PCap::~PCap() noexcept
{
    clear();
}

void PCap::clear(){
    if (pcap != nullptr){
        pcap_close(pcap);
        pcap = nullptr;
    }

    if (allDevices != nullptr){
        pcap_freealldevs(allDevices);
        allDevices = nullptr;
    }
}

void PCap::selectDevice(const std::string &deviceName) throw(exception::PCapException) {
    pcap_if_t *alldevs;
    pcap_if_t *device;

    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, nullptr, &alldevs, this->errorBuf) == -1){
        throw exception::PCapException(this->errorBuf);
    }

    for(device = alldevs; device != nullptr; device = device->next)
    {
        if (deviceName == device->name){
            this->device = device;
            this->allDevices = alldevs;
            return;
        }
    }

    throw exception::PCapException("could not find device");
}

void PCap::init() throw(exception::PCapException) {
    if ((this->pcap = pcap_open(this->device->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, this->captureTimeout, NULL, this->errorBuf)) == nullptr)
    {
        throw exception::PCapException(this->errorBuf);
    }
}

int64_t PCap::doCapture(const std::string &deviceName) throw(exception::PCapException) {
    this->clear();
    this->selectDevice(deviceName);
    this->init();

    int res = -1;
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    int64_t totalBytes = 0;

    while((res = pcap_next_ex(this->pcap, &header, &pkt_data)) >= 0){
        /* Timeout elapsed */
        if(res == 0){
            //printf("[read timeout]\n");
            continue;
        }

        totalBytes += header->len;
        this->packet_callback(header, pkt_data);
    }

    if(res == -1){
        throw exception::PCapException(pcap_geterr(this->pcap));
    }

    return totalBytes;
}

void PCap::packet_callback(const struct pcap_pkthdr *header, const u_char *pkt_data) noexcept
{
    EthernetHeader *ethernet = (EthernetHeader*)pkt_data;


    // IP packets
	if(ntohs(ethernet->type) == 0x0800) {

        IpHeader *ip = (IpHeader*)&pkt_data[ sizeof(EthernetHeader) ];

		switch (ip->proto)
		{
		    /// ICMP Protocol
			case 1:

                break;

            /// IGMP Protocol
			case 2:

                break;

            /// TCP Protocol
			case 6:
			    if (this->packetHandler->isFlags(AbstractPacketHandler::TCP)){
                    TcpHeader *tcp = (TcpHeader*)&pkt_data[ sizeof(EthernetHeader) + sizeof(IpHeader) ];
                    char *data = (char*)&pkt_data[ sizeof(EthernetHeader) + sizeof(IpHeader) + sizeof(TcpHeader) ];
                    PacketHeader packet{header->ts, header->caplen, header->len, data};

                    this->packetHandler->tcpHandler(packet, *ethernet, *ip, *tcp);
                }
                break;

			/// UDP Protocol
			case 17:
			    if (this->packetHandler->isFlags(AbstractPacketHandler::UDP)){
                    UdpHeader *udp = (UdpHeader*)&pkt_data[ sizeof(EthernetHeader) + sizeof(IpHeader) ];
                    char *data = (char*)&pkt_data[ sizeof(EthernetHeader) + sizeof(IpHeader) + sizeof(UdpHeader) ];
                    PacketHeader packet{header->ts, header->caplen, header->len, data};

                    this->packetHandler->udpHandler(packet, *ethernet, *ip, *udp);
			    }
                break;

            /// Some Other Protocol like ARP etc.
			default:
                break;
		}
	}
	else{
        //printf("none ip\n");
	}
}

}
