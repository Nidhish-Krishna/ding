#include "net/send.h"
#include "net/sockets.h"

short sendEth(char *bytedata, short len)
{
  char buff[len];
  short i=0;
  struct sockaddr_ll inface;

  inface.sll_ifindex = interface;

  for(;i<len;i++) buff[i]=*(bytedata+i);

  return sendto(sock_desc, buff, len, 0, (struct sockaddr*)&inface, sizeof(inface));
}


