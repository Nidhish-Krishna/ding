#define ETH_HEAD_LEN 14
#define ETH_TAIL_LEN 4

short ether_payload_len;

struct ether_head
{
  unsigned char dest_mac[6];
  unsigned char src_mac[6];
  unsigned char eth_type[2];
};


struct ether_tail
{
  unsigned char crc[4];
};
