#include "utils/base.h"
#include "memory/allocMAC.h"

char* allocateMAC(char *word, char *len)
{
  char *macaddr;
  boolean charVal;
  int i=0, length = 0, index = 0, offset = 0;

  macaddr = (char*) malloc(6);
  
  while(*(word+i))
  {
    charVal = True;

    if      (*(word+i)=='-') {;}
    else if ((*(word+i)>=48) && (*(word+i)<=57)) {;}
    else if ((*(word+i)>=97) && (*(word+i)<=102)) {;}
    else if ((*(word+i)>=65) && (*(word+i)<=70)) {;}
    else {charVal = False;}

    if(!charVal)
    {
      fprintf(stderr, ERROR "Invalid MAC address ! Invalid Hex numbers in <%s>!\n" RESET, word);
      free(macaddr); return NULL;
    }
    else {;}

    if((*(word+i)=='-'))
    {
      if(length!=2)
      {
        fprintf(stderr, ERROR "Invalid MAC Address ! Only octet must be given in <%s>!\n" RESET, word);
        free(macaddr); return NULL;
      }
      else {;}
      *(macaddr+index++) = hexstrToint256(word,offset);
      length = 0;
      offset=i+1;
    }
    else { length++; }
    i++;
  }

  if(length>2) { fprintf(stderr, ERROR "Invalid MAC Address !\n" RESET); free(macaddr); return NULL; }
  else {;}

  if(index!=5) { fprintf(stderr, ERROR "Invalid MAC Address !\n" RESET); free(macaddr); return NULL;}
  else if((index==5) && (*(word+i-1)=='-')) { fprintf(stderr, ERROR "Invalid MAC Address !\n" RESET); free(macaddr); return NULL;}
  else{;}

  *(macaddr+index) = hexstrToint256(word,offset);
  *len = 6;

  return macaddr;
}


