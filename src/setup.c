#include "setup.h"
#include "functions.h"

#include "codes/node.h"
#include "codes/packet.h"
#include "codes/datatype.h"

#include "datastructures/bst.h"

static BinarySearchTree* arp_packet_tree;
static BinarySearchTree* ether_packet_tree;
static BinarySearchTree* ip4_packet_tree;


static void setupSymbols()
{
  symbol_tree = NULL;
  UserVariable* (*func) (Node *args);

  func = &ifcond;
  insertNode(&symbol_tree, createNode("if\0", FUNCTION_NODE, (char*) func));

  func = &loop;
  insertNode(&symbol_tree, createNode("loop\0", FUNCTION_NODE, (char*) func));

  func = &sendfunc;
  insertNode(&symbol_tree, createNode("send\0", FUNCTION_NODE, (char*) func));

  func = &recvfunc;
  insertNode(&symbol_tree, createNode("receive\0", FUNCTION_NODE, (char*) func));

  func = &container;
  insertNode(&symbol_tree, createNode("container\0", FUNCTION_NODE, (char*) func));

  func = &print;
  insertNode(&symbol_tree, createNode("print\0", FUNCTION_NODE, (char*) func));

  func = &Log;
  insertNode(&symbol_tree, createNode("log\0", FUNCTION_NODE, (char*) func));

  func = &cat;
  insertNode(&symbol_tree, createNode("cat\0", FUNCTION_NODE, (char*) func));

  func = &interfacefunc;
  insertNode(&symbol_tree, createNode("interface\0", FUNCTION_NODE, (char*) func));
}


static void setupMemAllocFuncs()
{
  datatype_tree = NULL;

  insertNode(&datatype_tree, createNode("ip4\0", IP4_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("ip4\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("ip6\0", IP6_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("ip6\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("mac\0", MAC_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("mac\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("bit\0", BIT_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("bit\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("number\0", NUMBER_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("number\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("packet\0", PACKET_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("packet\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("string\0", STRING_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("string\0", DATATYPE_NODE, NULL));

  insertNode(&datatype_tree, createNode("stream\0", STREAM_TYPE_CODE, NULL));
  insertNode(&symbol_tree, createNode("stream\0", DATATYPE_NODE, NULL));
}


static void setupPackets()
{
  packet_tree = NULL;
  arp_packet_tree = NULL;
  ether_packet_tree = NULL;
  ip4_packet_tree = NULL;

  insertNode(&arp_packet_tree, createNode("HW_TYPE\0", __ARP_HW_TYPE, NULL));
  insertNode(&arp_packet_tree, createNode("PROTO_TYPE\0", __ARP_PROTO_TYPE, NULL));
  insertNode(&arp_packet_tree, createNode("HW_ADDRESS_LEN\0", __ARP_HW_ADDRESS_LEN, NULL));
  insertNode(&arp_packet_tree, createNode("PROTO_ADDRESS_LEN\0", __ARP_PROTO_ADDRESS_LEN, NULL));
  insertNode(&arp_packet_tree, createNode("OPERATION\0", __ARP_OPERATION, NULL));
  insertNode(&arp_packet_tree, createNode("SRC_MAC\0", __ARP_SRC_MAC, NULL));
  insertNode(&arp_packet_tree, createNode("SRC_IP\0", __ARP_SRC_IP, NULL));
  insertNode(&arp_packet_tree, createNode("DEST_MAC\0", __ARP_DEST_MAC, NULL));
  insertNode(&arp_packet_tree, createNode("DEST_IP\0", __ARP_DEST_IP, NULL));
  insertNode(&arp_packet_tree, createNode("PAYLOAD\0", __ARP_PAYLOAD, NULL));

  insertNode(&ether_packet_tree, createNode("DEST_MAC\0", __ETHER_DEST_MAC, NULL));
  insertNode(&ether_packet_tree, createNode("SRC_MAC\0", __ETHER_SRC_MAC, NULL));
  insertNode(&ether_packet_tree, createNode("TYPE\0", __ETHER_TYPE, NULL));
  insertNode(&ether_packet_tree, createNode("PAYLOAD\0", __ETHER_PAYLOAD, NULL));
  insertNode(&ether_packet_tree, createNode("CRC\0", __ETHER_CRC, NULL));

  insertNode(&ip4_packet_tree, createNode("VERSION\0", __IP4_VERSION, NULL));
  insertNode(&ip4_packet_tree, createNode("HEADER_LEN\0", __IP4_HEADER_LEN, NULL));
  insertNode(&ip4_packet_tree, createNode("TYPE_OF_SERVICE\0", __IP4_TYPE_OF_SERVICE, NULL));
  insertNode(&ip4_packet_tree, createNode("TOTAL_LEN\0", __IP4_TOTAL_LEN, NULL));
  insertNode(&ip4_packet_tree, createNode("IDENTIFICATION\0", __IP4_IDENTIFICATION, NULL));
  insertNode(&ip4_packet_tree, createNode("FLAGS\0", __IP4_FLAGS, NULL));
  insertNode(&ip4_packet_tree, createNode("FRAG_OFFSET\0", __IP4_FRAG_OFFSET, NULL));
  insertNode(&ip4_packet_tree, createNode("TTL\0", __IP4_TTL, NULL));
  insertNode(&ip4_packet_tree, createNode("PROTOCOL\0", __IP4_PROTOCOL, NULL));
  insertNode(&ip4_packet_tree, createNode("CHECKSUM\0", __IP4_CHECKSUM, NULL));
  insertNode(&ip4_packet_tree, createNode("SRC_IP\0", __IP4_SRC_IP, NULL));
  insertNode(&ip4_packet_tree, createNode("DEST_IP\0", __IP4_DEST_IP, NULL));
  insertNode(&ip4_packet_tree, createNode("PAYLOAD\0", __IP4_PAYLOAD, NULL));

  insertNode(&packet_tree, createNode("IP4\0", __IP4, (char*) ip4_packet_tree));
  insertNode(&packet_tree, createNode("ETHER\0", __ETHER, (char*) ether_packet_tree));
  insertNode(&packet_tree, createNode("ARP\0", __ARP, (char*) arp_packet_tree));
}

void setup()
{
  setupSymbols();
  setupPackets();
  setupMemAllocFuncs();
}

void freeSetup()
{
  freeTree(symbol_tree);
  freeTree(datatype_tree);

  freeTree(arp_packet_tree);
  freeTree(ether_packet_tree);
  freeTree(ip4_packet_tree);
  freeTree(packet_tree);
}
