

#include "Tcpip.h"
#include <nids.h>
int main() {

	nids_params.filename ="./../doc/connect.pcapng";

	string pcap_filter = "port 5432";
	nids_params.pcap_filter = (char*)pcap_filter.c_str();
	nids_chksum_ctl dnot_csum = { NULL,NULL,0 ,NULL };

	nids_register_chksum_ctl(&dnot_csum,1);

	if (!nids_init()) {
		printf("error\n");
		return -1;
	}
	struct tcp_stream *a_tcp;
	nids_register_tcp((void*)tcp_callback);
	nids_run();

	return 0;
}





//     char p_error[PCAP_ERRBUF_SIZE];
// 	pcap_t *pcap_handle = pcap_open_offline("../doc/connect.pcapng", p_error);
// 	//   /root/Desktop/Finally
// 	if (pcap_handle == NULL)
// 		{cout <<" p_error\n";
// 		return 0;}
// 	char filter_app[] = "port 5432";
// 	struct bpf_program filter;
// 	if (pcap_compile(pcap_handle, &filter, filter_app, 1, 0) == -1)
// 		cout << "pcap_geterr(pcap_handler)";
// 	if (pcap_setfilter(pcap_handle, &filter)<0)
// 		cout << "fffffffffffff\n";
// 	header_eth h_eth ;
// 	header_ip h_ip ;
// 	header_tcp h_tcp ;
// 	PgSqlProtocol A;
// 	const char *pktdata = 0;   
// 	struct pcap_pkthdr protocol_header;
// 	bpf_u_int32 ph_len ;

// 	int logtf=1;
// 	int test=10;
// 	while(test--){
// 		pktdata = (char*)pcap_next(pcap_handle, &protocol_header);
// 		logtf+=1;
// 		if(protocol_header.len<70)
// 			continue;
// 		pcap_tcp_ip(pktdata, &h_eth, &h_ip, &h_tcp);
		
// 		ph_len = protocol_header.len-len_all;
// 		//cout<<(int)h_ip.des[3]<<endl;
// 		pktdata+=len_all;
// 		if(h_ip.des[3]==0x01){
// 			if(A.CompleteToDB((char*)pktdata, ph_len)){
// 				A.ParseToDB();
// 			}
// 		}
// 		if(h_ip.des[3]==0x82){
// 			if(A.CompleteFromDB((char*)pktdata, ph_len))
// 				A.ParseFromDB();
// 		}
// 	}
// 	cout << "\nhello world\n";	
// 	pcap_close(pcap_handle);
// }
