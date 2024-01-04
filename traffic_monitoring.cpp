// C++ program that monitors network traffic and blocks suspicious or malicious 
//traffic that may be used by ransomware to spread or communicate with the attackers.
#include <pcap.h>
#include <arpa/inet.h>

void block_traffic(const u_char *packet, int size) {
    //checking if the packet matches the pattern of a ransomware communication
    //if yes,blocking the traffic by dropping the packet
}

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    //calling the block_traffic function to check if the packet is suspicious or malicious
    block_traffic(packet, header->len);
}

int main() {
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    //opening the network interface for capturing packets
    //eth0
    handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, error_buffer);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device: %s\n", error_buffer);
        return 1;
    }

    //setting up a filter to capture only the traffic we are interested in
    struct bpf_program fp;
    //80?443
    char filter_exp[] = "tcp port 80 or tcp port 443";
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        fprintf(stderr, "Could not parse filter: %s\n", pcap_geterr(handle));
        return 1;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Could not set filter: %s\n", pcap_geterr(handle));
        return 1;
    }

    //starting capturing packets
    pcap_loop(handle, -1, packet_handler, NULL);

    //closing the handle
    pcap_close(handle);
    return 0;
}
