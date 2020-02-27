#ifndef LIBPCAP_H
#define LIBPCAP_H

#include <string>

//#ifdef WIN32
#include <winsock2.h>
//#else

//#endif // WIN32

#define HAVE_REMOTE
#include <pcap.h>

#include "../exception/PCapException.h"
#include "AbstractPacketHandler.h"


namespace net{

class PCap
{
    public:
        PCap() noexcept;
        virtual ~PCap() noexcept;

        int64_t doCapture(const std::string &deviceName) throw(exception::PCapException);

        void setPacketHandler(AbstractPacketHandler *packetHandler) noexcept {
            this->packetHandler = packetHandler;
        }

        AbstractPacketHandler *getPacketHandler(void) noexcept{
            return this->packetHandler;
        }

        void setCaptureTimeout(int captureTimeout) noexcept {
            this->captureTimeout = captureTimeout;
        }

        int getCaptureTimeout() const noexcept {
            return this->captureTimeout;
        }

    private:

        pcap_t *pcap;
        pcap_if_t *allDevices;
        pcap_if_t *device;
        char errorBuf[PCAP_ERRBUF_SIZE + 1];
        AbstractPacketHandler *packetHandler;
        int captureTimeout = 0;


        void init() throw (exception::PCapException);
        void selectDevice (const std::string &deviceName) throw(exception::PCapException);

        void clear();

        void packet_callback(const struct pcap_pkthdr *header, const u_char *pkt_data) noexcept;
};

}

#endif // LIBPCAP_H
