
#include "PgSqlProtocol.h"
//#include "PgSqlstdafx.h"
#define len_eth 14
#define len_ip 20
#define len_tcp 20
#define len_all 54 

typedef struct HEADER_ETH {
	u_char des[6];
	u_char sou[6];
	u_short type;
}header_eth;

typedef struct HEADER_IP {
	u_char ver_len;
	u_char dsf;
	u_short total_len;
	u_short ident;
	u_char flags;
	u_char fragment;
	u_char time_live;
	u_char protocol;
	u_short header_check;
	u_char des[4];
	u_char sou[4];
}header_ip;

typedef struct HEADER_TCP {
	u_short  souport;
	u_short  desport;
	u_int seq;
	u_int ack;
	u_short flags;
	u_short winsize;
	u_short checksum;
	u_short urgnet_pointer;
} header_tcp;

void pcap_tcp_ip(const char *data,
    header_eth *h_eth, 
    header_ip *h_ip,
	header_tcp *h_tcp);
	
void tcp_callback(struct tcp_stream *a_tcp, void ** uknow);