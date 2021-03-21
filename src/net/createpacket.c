#include "codes/packet.h"

#include "utils/string.h"

#include "net/members/all.h"
#include "net/createpacket.h"

PacketObject* getPacket(short packet_type)
{
  PacketObject* packet = (PacketObject*) malloc(1*sizeof(PacketObject));

  switch(packet_type)
  {
    case __ARP:
    {
      packet->packet_type = packet_type;
      arp_payload_len = -1;
      struct arp_head *packet_head = (struct arp_head*) malloc(sizeof(struct arp_head));

      clearMemory((char*)packet_head,sizeof(struct arp_head));

      packet->head_len = ARP_HEAD_LEN;
      packet->tail_len = ARP_TAIL_LEN;
      packet->head = (unsigned char*) packet_head;

      packet->payload = NULL;
      packet->payload_len = arp_payload_len;
      packet->packet_len = ARP_HEAD_LEN + ARP_TAIL_LEN + arp_payload_len;
      break;
    }

    case __ETHER:
    {
      packet->packet_type = packet_type;
      ether_payload_len = -1;

      struct ether_head *packet_head = (struct ether_head*) malloc(sizeof(struct ether_head));
      struct ether_tail *packet_tail = (struct ether_tail*) malloc(sizeof(struct ether_tail));

      clearMemory((char*)packet_head,sizeof(struct ether_head));
      clearMemory((char*)packet_tail,sizeof(struct ether_tail));

      packet->head_len = ETH_HEAD_LEN;
      packet->tail_len = ETH_TAIL_LEN;
      packet->head = (unsigned char*) packet_head;
      packet->tail = (unsigned char*) packet_tail;

      packet->payload = NULL;
      packet->payload_len = ether_payload_len;
      packet->packet_len = ETH_HEAD_LEN + ETH_TAIL_LEN + ether_payload_len;
      break;
    }

    case __IP4:
      ip4_payload_len = -1;
      packet->packet_type = __IP4;

      struct ip4_head *packet_head = (struct ip4_head*) malloc(sizeof(struct ip4_head));

      clearMemory((char*)packet_head,sizeof(struct ip4_head));

      packet->head_len = IP4_HEAD_LEN;
      packet->tail_len = IP4_TAIL_LEN;
      packet->head = (unsigned char*) packet_head;

      packet->payload_len = ip4_payload_len;

      packet->payload = NULL;
      packet->payload_len = ip4_payload_len;
      packet->packet_len = IP4_HEAD_LEN + IP4_TAIL_LEN + ip4_payload_len;
      break;

    default:
      fprintf(stderr, ERROR"Invalid Packet\n"RESET);
      free(packet);
      return NULL;
  }
  return packet;
}


