#include "print.h"

#include "net/send.h"
#include "net/receive.h"
#include "net/interface.h"
#include "net/assembling.h"
#include "net/createpacket.h"

#include "datastructures/bst.h"

#include "functions.h"
#include "utils/base.h"
#include "codes/datatype.h"

UserVariable* ifcond(Node* args)
{
  return NULL;
}

UserVariable* loop(Node* args)
{
  return NULL;
}

UserVariable* interfacefunc(Node* args)
{
  if(args == NULL)
  {
    fprintf(stderr, ERROR"interface() requires an argument !\nPlease specify an interface name !\n"RESET);
    return NULL;
  }
  else {;}

  UserVariable* value = (UserVariable*)(((Node*)(args->data))->data);
  if(value->datatype != STRING_TYPE_CODE)
  {
    fprintf(stderr, ERROR"interface() requires a <string> argument !\nPlease specify the interface name as string !\n"RESET);
    return NULL;
  }
  else {;}
  if (getInterface(value->value,value->length)) {;}  
  else return NULL;
  return value;
  
}

UserVariable* sendfunc(Node* args)
{
  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = NUMBER_TYPE_CODE;
  return_var->value = (char*) malloc(1*sizeof(char));
  return_var->length = 1;

  while(args != NULL)
  {
    UserVariable* packet_variable = (UserVariable*)(((Node*)(args->data))->data);

    if(!packet_variable->length)
    {
      fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, packet_variable->identifier);
      free(return_var->value);
      free(return_var);
      return NULL;
    }

    else {;}
    if(packet_variable->datatype != PACKET_TYPE_CODE)
    {
      fprintf(stderr, ERROR"Invalid argument !\nsend() takes only packet arguments !\n"RESET);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else {;}

    PacketObject* packet = (PacketObject*)((PacketValue*)(packet_variable->value))->packet;
    assemblePacket(packet);

    short status = sendEth(packet->assmbld_packet_struct, packet->packet_len);
    if(status<0)
    {
      fprintf(stderr, ERROR"Packet was not sent !\nCheck whether you have forgot to set the interface...\n"RESET);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else *(return_var->value) = 1;
    args = (Node*)args->right;
  }

  return return_var;
}


UserVariable* recvfunc(Node* args)
{
  return NULL;
}


UserVariable* cat(Node* args)
{
  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = STRING_TYPE_CODE;
  return_var->value = (char*) malloc(1*sizeof(char));
  return_var->length = 0;

  short index = 0;

  while(args != NULL)
  {
    UserVariable* value = (UserVariable*)(((Node*)(args->data))->data);

    if(!value->length)
    {
      fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, value->identifier);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else {;}

    if(value->datatype != STRING_TYPE_CODE) return_var->datatype = STREAM_TYPE_CODE;
    else {;}

    short i=0, arg_len = value->length;
    (return_var->length) += arg_len;

    return_var->value = (char*) realloc(return_var->value, return_var->length);
    while(i<arg_len) *(return_var->value+index++) = *(((char*)(value->value))+i++);

    args = (Node*)args->right;
  }
  return return_var;
}

UserVariable* container(Node* args)
{
  UserVariable* val = (UserVariable*)(((Node*)(args->data))->data);
  char* packet_name = val->value;
  short* index = (short*) malloc(sizeof(short));

  PacketValue* value = (PacketValue*) malloc(sizeof(PacketValue));

  BinarySearchTree* current = packet_tree;
  BinarySearchTree* member_tree;

  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = PACKET_TYPE_CODE;
  return_var->value = (char*) value;
  return_var->length = 1;

  while(*(packet_name))
  {
    current = getNode(current, *(packet_name++), index);

    if (current == NULL) { free(index); free(value); free(return_var); return NULL; }
    else {;}
  }

  if(!*((current->key)+(*index)))
  {
    value->packet = getPacket(current->type);
    value->member_tree = (Node*) current->data;
  }
  else { free(index); free(value); free(return_var); return NULL; }

  if(value->packet == NULL)  { free(index); free(value); free(return_var); return NULL; }
  else {;}

  free(index);
  return return_var;
}

UserVariable* print(Node* args)
{
  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = NUMBER_TYPE_CODE;
  return_var->value = (char*) malloc(1*sizeof(char));
  return_var->length = 1;

  *(return_var->value) = 0;

  while(args != NULL)
  {
    UserVariable* value = (UserVariable*)(((Node*)(args->data))->data);

    if(!value->length)
    {
      fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, value->identifier);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else {;}
    *(return_var->value) += 1;

    switch(value->datatype)
    {
      case STRING_TYPE_CODE:
        printStr(value->value);
        break;
      case STREAM_TYPE_CODE:
        printHexDump(value->value, value->length);
        break;
      case IP4_TYPE_CODE:
        printIp4(value->value);
        break;
      case IP6_TYPE_CODE:
        printIp6(value->value);
        break;
      case MAC_TYPE_CODE:
        printMac(value->value);
        break;
      case PACKET_TYPE_CODE:;
        PacketObject* temp = (PacketObject*)(((PacketValue*)(value->value))->packet);
        assemblePacket(temp);
        printHexDump((char*) temp->assmbld_packet_struct, temp->packet_len);
        break;
      case BIT_TYPE_CODE:
        printBit(numberTolong(value->value, value->length));
        break;
      case NUMBER_TYPE_CODE:
        printNum(numberTolong(value->value, value->length));
        break;
      default:
        fprintf(stderr, ERROR"Can't print data of type <%s> !\nPrint Not Yet Defined for this type.\n"RESET,value->identifier);
        free(return_var->value);
        free(return_var);
        return NULL;
      
    }
    args = (Node*)args->right;
  }
  fprintf(stdout, USR "\n" RESET);
  return return_var;
}

UserVariable* Log(Node* args)
{
/*  
  //DO LOGGING//
*/
  return print(args);
}

