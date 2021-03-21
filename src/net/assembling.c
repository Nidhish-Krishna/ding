#include "net/assembling.h"

// DISASSEMBLING TO BE WRITTEN

void assemblePacket(PacketObject* packet)
{
  short i,j=0;
  unsigned char* final_packet;
  int final_packet_len = packet->head_len + packet->payload_len + packet->tail_len;

  final_packet = (unsigned char*) malloc(final_packet_len);

  for(i=0;i<packet->head_len;i++,j++)
    *(final_packet+j) = *((packet->head)+i);

  for(i=0;i<packet->payload_len;i++,j++)
    *(final_packet+j) = *((packet->payload)+i);

  for(i=0;i<packet->tail_len;i++,j++)
    *(final_packet+j) = *((packet->tail)+i);

  packet->assmbld_packet_struct = (unsigned char*) final_packet;
  packet->packet_len = final_packet_len;
}

