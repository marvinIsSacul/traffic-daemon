#ifndef ABSTRACTPACKETHANDLER_H_INCLUDED
#define ABSTRACTPACKETHANDLER_H_INCLUDED

#include <map>

#include "protocol_header.h"
#include "util.h"


namespace net{


/**
 * Defines how a packet should be handled.
*/
class AbstractPacketHandler{

public:

    enum Protocol{
        NONE = 0,
        TCP = 1,
        UDP = 2,
        ICMP = 4
    };


    explicit AbstractPacketHandler(int flags = NONE) : _flags{flags} {}
    AbstractPacketHandler(char) = delete;

    virtual ~AbstractPacketHandler(){}

    virtual AbstractPacketHandler &operator=(int) = delete;
    virtual AbstractPacketHandler &operator=(char) = delete;

    /**
     * Gets the currently set flags.
     *
     * @param flags
     * @return int
    */
    virtual int getFlags() const { return this->_flags; }

    /**
     * Asserts whether flag has been set or not.
     *
     * @param flags
     * @return bool
    */
    virtual bool isFlags(int flags) const { return this->_flags & flags; }

    /**
     * Sets new flags and overrides the old flags.
     *
     * @param flags The new flags to set.
    */
    virtual void setFlags(int flags){ this->_flags = flags; }

    /**
     * Appends new flags to the old flags.
     *
     * @param flags The flags to append
    */
    virtual void attachFlags(int flags) { this->setFlags(this->getFlags() | flags); }

    /**
     * Removes the given flags from the old flags.
     *
     * @param flags The flags to remove
    */
    virtual void detachFlags(int flags) { this->setFlags(this->getFlags() & ~flags); }

    /**
     * Gets the total number of bytes that have been handled.
     * the total of all the protocols that can be handled.
     *
     * @return int64_t The total bytes of the protocol.
    */
    virtual int64_t getTotalBytes() const {
        int64_t bytes = 0;
        for (auto data : this->protocolBytes)
            bytes += data.second;
        return bytes;
    }

    /**
     * Gets the total bytes for a given protocol.
    */
    virtual int64_t getProtocolBytes(Protocol protocol) throw(std::out_of_range) {
        return this->protocolBytes.at(protocol);
    }

    /**
     * This is the TCP handler callback.
     *
     * @exception none
     * @param packet The header of the packet actual data
     * @param ethernet The header of the ethernet
     * @param ip The header of the ip
     * @param tcp The header of the tcp frame
     * @return void
    */
    virtual void tcpHandler(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const TcpHeader &tcp) noexcept = 0;

    /**
     * This is the UDP handler callback.
     *
     * @exception none
     * @param packet The header of the packet actual data
     * @param ethernet The header of the ethernet frame
     * @param ip The header of the ip frame
     * @param udp The header of the udp frame
     * @return void
    */
    virtual void udpHandler(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const UdpHeader &udp) noexcept = 0;

protected:

    /**
     * One of the only ways to alter the protocol bytes.
     * Sets the total number of bytes for a given protocol.
     *
     * @exception none
     * @param protocol The protocol
     * @param bytes The bytes
    */
    virtual void setProtocolBytes(Protocol protocol, int64_t bytes) noexcept{
        this->protocolBytes[protocol] = bytes;
    }

    /**
     * One of the only ways to alter the protocol bytes.
     * Increases the total number of bytes for a given protocol.
     *
     * @exception none
     * @param protocol The protocol
     * @param bytes The bytes
    */
    virtual void incProtocolBytes(Protocol protocol, int64_t incBytesBy) noexcept{
        this->protocolBytes[protocol] += incBytesBy;
    }

     /**
     * One of the only ways to alter the protocol bytes.
     * Decreases the total number of bytes for a given protocol.
     *
     * @exception none
     * @param protocol The protocol
     * @param bytes The bytes
    */
    virtual void decProtocolBytes(Protocol protocol, int64_t decBytesBy) noexcept{
        this->protocolBytes[protocol] -= decBytesBy;
    }

private:

    int _flags;
    std::map<int, int64_t> protocolBytes;
};

}


#endif // ABSTRACTPACKETHANDLER_H_INCLUDED
