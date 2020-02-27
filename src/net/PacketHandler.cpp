#include "PacketHandler.h"

#include "model/UserMap.h"
#include "model/User.h"
#include "exception/dump.h"

#include "util/md5.h"


namespace net{


PacketHandler::PacketHandler()
{

}

PacketHandler::~PacketHandler()
{
    //dtor
}

std::string PacketHandler::calculateHash(const PacketHeader &packet){
    constexpr size_t HEX_SIZE = 32;
    constexpr size_t HASH_SIZE = 16;
    char hash[HASH_SIZE];
    char hex_string[HEX_SIZE + 1] = {0};    /// set all to NULL

    /// calculate md5 of packet data

    md5_buffer(static_cast<const char*>(packet.data), packet.len, hash);
    md5_sig_to_string(hash, hex_string, HEX_SIZE);

    return hex_string;
}


void PacketHandler::writeToDb(const PacketHeader &, const EthernetHeader &ethernet,
                            const IpHeader &ip, const TcpHeader &tcp)
{
    //const std::string &hex_string = calculateHash(packet);

    /*try {
        model::User userSrc;
        userSrc.setMacAddress(net::macString(ethernet.mac_src));
        printf("insert");
    }
    catch (const exception::DatabaseException &e){
        ex_dump(e);
    }

    try {
        model::User userDst;
        userDst.setMacAddress(net::macString(ethernet.mac_dst));
    }
    catch (const exception::DatabaseException &e){
        ex_dump(e);
    }*/

    try {
        model::UserMap usermapSrc;
        usermapSrc.setMacAddress(net::macString(ethernet.mac_src));
        usermapSrc.setIpAddress(net::ipString(ip.address_src));
        usermapSrc.setPort(tcp.port_src);

        /// only capture if the source is the private address.
        /// and only capture from private address and those which don't use a standard port

        if (1 || net::isIpPrivate(ip.address_src)){
            usermapSrc.save();
            _macIp[usermapSrc.getMacAddress()] = ip.address_src.address;
        }

    }

    catch (const exception::DatabaseException &e){
        ex_dump(e);
    }

    try {
        model::UserMap usermapDst;
        usermapDst.setMacAddress(net::macString(ethernet.mac_dst));
        usermapDst.setIpAddress(net::ipString(ip.address_dst));
        usermapDst.setPort(tcp.port_dst);
       // usermapDst.setHash(hex_string);

        /// only capture private address and those which don't use a standard port

        if ( 1 || net::isIpPrivate(ip.address_dst)) {
            usermapDst.save();
            _macIp[usermapDst.getMacAddress()] = ip.address_dst.address;
        }
    }
    catch (const exception::DatabaseException &e){
        ex_dump(e);
    }

}

void PacketHandler::tcpHandler(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const TcpHeader &tcp) noexcept
{
    this->incProtocolBytes(Protocol::TCP, packet.len);

    /// if data flows from internet to private or vice versa.
    /// not private to private.
    if (true || (net::isIpPublic(ip.address_dst) && net::isIpPrivate(ip.address_src)) ||
       (net::isIpPublic(ip.address_src) && net::isIpPrivate(ip.address_dst)))
    {

        writeToDb(packet, ethernet, ip, tcp);
    }

}

void PacketHandler::udpHandler(const PacketHeader &packet, const EthernetHeader &ethernet,
                            const IpHeader &ip, const UdpHeader &tcp) noexcept
{
    this->incProtocolBytes(Protocol::UDP, packet.len);

    (void) packet;
    (void) ethernet;
    (void) ip;
    (void) tcp;

    try{

    }
    catch (const exception::DatabaseException &e){
        ex_dump(e);
    }
    catch (...){

    }
}


}
