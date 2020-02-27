#ifndef PROTOCOL_HEADER_PHP_INCLUDED
#define PROTOCOL_HEADER_PHP_INCLUDED


#include <cstdint>
#include <sys/time.h>


struct PacketHeader {
    struct timeval ts;         // timestamp
    uint32_t caplen;
    uint32_t len;           // length (off wire)
    void *data;             // the data (which is len size long)
};


struct IpAddress {
    union{
        uint32_t address;
        uint8_t oct[4];
    };
};


struct MacAddress{
    uint8_t bin[6];
};


struct EthernetHeader {
    uint8_t mac_dst[6];
    uint8_t mac_src[6];
    uint16_t type;
};



struct IpHeader {
    uint8_t  ver_ihl;           // Version (4 bits) + Internet header length (4 bits)
    uint8_t  tos;               // Type of service
    uint16_t tlen;              // Total length
    uint16_t id;                // Identification
    uint16_t flags_fo;          // Flags (3 bits) + Fragment offset (13 bits)
    uint8_t  ttl;               // Time to live
    uint8_t  proto;             // Protocol
    uint16_t crc;               // Header checksum
    IpAddress  address_src;      // Source address
    IpAddress  address_dst;      // Destination address
    uint32_t   op_pad;         // Option + Padding
};


struct UdpHeader {
    uint16_t port_src;          // Source port
    uint16_t port_dst;          // Destination port
    uint16_t length;            // Datagram length
    uint16_t crc;               // Checksum
};


struct TcpHeader {
		uint16_t port_src;	    /* source port */
		uint16_t port_dst;	    /* destination port */
		uint32_t sequence_num;		/* sequence number */
		uint32_t ack_num;		/* acknowledgement number */
		uint8_t data_off;	    /* data offset, rsvd */
	#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
		uint8_t th_flags;
	#define TH_FIN 0x01
	#define TH_SYN 0x02
	#define TH_RST 0x04
	#define TH_PUSH 0x08
	#define TH_ACK 0x10
	#define TH_URG 0x20
	#define TH_ECE 0x40
	#define TH_CWR 0x80
	#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
		uint16_t window;		/* window */
		uint16_t checksum;		/* checksum */
		uint16_t urgent_ptr;	/* urgent pointer */
};


#endif // PROTOCOL_HEADER_PHP_INCLUDED
