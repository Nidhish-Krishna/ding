#include "utils/base.h"
#include "memory/allocIP4.h"

char* allocateIP4(char *word, char *len)
{
  char *ipaddr;
  int length = 0, i = 0, index=0, offset = 0;
  ipaddr = (char*) malloc(4);

  while(*(word+i))
  {
    if(length>3)
    {
      fprintf(stderr, ERROR "Invalid IP Address ! Greater than 256 in the octet <%d> in <%s> !\n" RESET, index+1, word);
      free(ipaddr); return NULL;
    }
    else {;}

    if(((*(word+i)<48) || (*(word+i)>57)) && (*(word+i)!='.'))
    {
      fprintf(stderr, ERROR "Invalid IP address or IP Format in <%s> !\n" RESET, word);
      free(ipaddr);
      return NULL;
    }
    else{;}

    if((*(word+i)=='.'))
    {
      *(ipaddr+index++) = strToint256(word,offset, length);
      length = 0;
      offset=i+1;
    }
    else { length++; }
    i++;
  }

  if(length>3)
  {
    fprintf(stderr, ERROR "Invalid IP Address ! Greater than 256 in the octet <%d> in <%s> !\n" RESET, index+1, word); free(ipaddr);
    return NULL;
  }
  else {;}

  if(index!=3)
  {
    fprintf(stderr, ERROR "Invalid IP Address ! Need 4 octets, but only <%d> octet(s) in <%s> !\n" RESET, index+1, word); free(ipaddr);
    return NULL;
  }
  else if((index==3) && (*(word+i-1)=='.'))
  {
    fprintf(stderr, ERROR "Invalid IP Address or IP Format <%s> !\n" RESET, word);  free(ipaddr);
    return NULL;
  }
  else{;}

  *(ipaddr+index) = strToint256(word,offset, length);
  *len = 4;

  return ipaddr;
}


