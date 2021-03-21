#ifndef SYS_SOCK_H
#define SYS_SOCK_H
#include <sys/socket.h>
#endif

#include<linux/if_packet.h>

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

short sendEth(char* bytedata, short len);

