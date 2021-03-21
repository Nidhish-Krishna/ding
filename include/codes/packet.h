///* CODES FOR PACKETS AND PACKET MEMBERS *///

#define __ETHER 0x00
#define __ARP 0x01
#define __IP4 0x02

#define __ARP_HW_TYPE 0x0100              //number (2 bytes)
#define __ARP_PROTO_TYPE 0x0101           //number (2 bytes)
#define __ARP_HW_ADDRESS_LEN 0x0102       //number (1 byte)
#define __ARP_PROTO_ADDRESS_LEN 0x0103    //number (1 byte)
#define __ARP_OPERATION 0x0104            //number (2 bytes)
#define __ARP_SRC_MAC 0x0105              //mac
#define __ARP_SRC_IP 0x0106               //ip4
#define __ARP_DEST_MAC 0x0107             //mac
#define __ARP_DEST_IP 0x0108              //ip4
#define __ARP_PAYLOAD 0x0109              //string (or) stream (or) packet

#define __ETHER_DEST_MAC 0x0000           //mac
#define __ETHER_SRC_MAC 0x0001            //mac
#define __ETHER_TYPE 0x0002               //number (2 bytes)
#define __ETHER_PAYLOAD 0x0003            //string (or) stream (or) packet
#define __ETHER_CRC 0x0004                //number (4 bytes)

#define __IP4_VERSION 0x0201              //bit (4 bits)
#define __IP4_HEADER_LEN 0x0202           //bit (4 bits)
#define __IP4_TYPE_OF_SERVICE 0x0203      //number (1 byte)
#define __IP4_TOTAL_LEN 0x0204            //number (2 bytes)
#define __IP4_IDENTIFICATION 0x0205       //number (2 bytes)
#define __IP4_FLAGS 0x0206                //bit (3 bits)
#define __IP4_FRAG_OFFSET 0x0207          //bit (13 bits)
#define __IP4_TTL 0x0208                  //number (1 byte)
#define __IP4_PROTOCOL 0x0209             //number (1 byte)
#define __IP4_CHECKSUM 0x020a             //number (2 bytes)
#define __IP4_SRC_IP 0x020b               //ip4
#define __IP4_DEST_IP 0x020c              //ip4
#define __IP4_PAYLOAD 0x020d              //string (or) stream (or) packet

