#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <map>

#include "AbstractPacketHandler.h"


namespace net{


/**
 * Implements the Packet Handler. It handles different packet types, such as TCP, UDP, etc.
*/
class PacketHandler : public AbstractPacketHandler
{
    private:

        std::map<std::string, uint32_t> _macIp;

        std::string calculateHash(const PacketHeader &packet);

        void writeToDb(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const TcpHeader &tcp);

    public:

        PacketHandler();
        virtual ~PacketHandler();

    /**
     * This is the TCP callback.
     *
     * @param
     * @param
     * @param
     * @return void
    */
    virtual void tcpHandler(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const TcpHeader &tcp) noexcept override;

    /**
     * This is the UDP callback.
     *
     * @param
     * @param
     * @param
     * @return void
    */
    virtual void udpHandler(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const UdpHeader &tcp) noexcept override;
};

}

#endif // PACKETHANDLER_H
