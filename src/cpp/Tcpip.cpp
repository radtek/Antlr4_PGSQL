#include "PgSqlstdafx.h"
#include "Tcpip.h"
#include <nids.h>

void pcap_tcp_ip(const char *data, header_eth *h_eth, header_ip *h_ip, header_tcp *h_tcp) {
	u_int i = 0, from = 0;
	for (i = 0; i < 6; i++) {
		h_eth->des[i] = data[from];
		h_eth->sou[i] = data[from + 6 + i];
		from++;
	}
	from += 6;
	h_eth->type = To_short_from(data, from);

	h_ip->ver_len = data[from++];
	h_ip->dsf = data[from++];;
	h_ip->total_len = To_short_from(data, from);
	h_ip->ident = To_short_from(data, from);
	h_ip->flags = data[from++];
	h_ip->fragment = data[from++];
	h_ip->time_live = data[from++];
	h_ip->protocol = data[from++];
	h_ip->header_check = To_short_from(data, from);
	for (i = 0; i < 4; i++) {
		h_ip->des[i] = data[from];
		h_ip->sou[i] = data[from + 4 + i];
		from++;
	}
	from += 4;

	h_tcp->souport = data[from++];
	h_tcp->desport = data[from++];
	h_tcp->seq = To_int_from(data, from);
	h_tcp->ack = To_int_from(data, from);
	h_tcp->flags = data[from++];
	h_tcp->winsize = data[from++];
	h_tcp->checksum = data[from++];
	h_tcp->urgnet_pointer = data[from++];

}


void tcp_callback(struct tcp_stream *a_tcp, void ** uknow)
{
	PgSqlProtocol *temp = new PgSqlProtocol;
	PgSqlProtocol *temp1 = new PgSqlProtocol;
	char buf[1024];
	bool complete;

	if (a_tcp->nids_state == NIDS_JUST_EST)
	{
		a_tcp->client.collect++; // 我们需要客户端接收到的数据.......
		a_tcp->server.collect++; // 我们需要服务器接收到的数据.......
		cout << "TCP connect\n";
		return;
	}
	if (a_tcp->nids_state == NIDS_CLOSE)
	{
		// 连接已经正常结束
		cout << "TCP close\n";
		return;
	}
	if (a_tcp->nids_state == NIDS_RESET)
	{
		// 连接已经通过RST关闭。
		cout << "TCP reset\n";
		return;
	}

	if (a_tcp->nids_state == NIDS_DATA)
	{

		struct half_stream *hlf;

		if (a_tcp->client.count_new)
		{
			//客户端的新数据
			hlf = &a_tcp->client;

			if (hlf->count_new >= 2) {
				complete = temp1->CompleteFromDB(hlf->data, hlf->count_new);
			}
		}
		else
		{
			hlf = &a_tcp->server;
			complete = temp->CompleteToDB(hlf->data, hlf->count_new);
		}
	}
	return;
};