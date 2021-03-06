#include "common.h"
#include "arcs_net.h"
#include "comms.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_ADDR "192.168.1.44"

struct sockaddr_in remote_sock;
int sockfd;
int pack_num = 0;

char *addrstr() {
	return r_args->r_addr == NULL ? DEFAULT_ADDR : r_args->r_addr;
}

void connect_comms() {
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		slog(100, SLOG_FATAL, "Failed to create socket");
		exit(EXIT_FAILURE);
	}

	memset((char *)&remote_sock, 0, sizeof(remote_sock));
	remote_sock.sin_family = AF_INET;
	remote_sock.sin_port = htons(REMOTE_PORT);

	remote_sock.sin_addr.s_addr = inet_addr(addrstr());
}

void disconnect_comms() { close(sockfd); }

void update_comms(struct js_state js_state, comm_mode_t mode, bool use_viewer,
				  char *viewer_ip) {
	struct pack_dtr p;
	p.pack_num = pack_num++;
	p.js_state = js_state;
	p.mode = mode;
	p.use_viewer = use_viewer;
	if (use_viewer) {
		strcpy(p.viewer_ip, viewer_ip);
	}
	if (sendto(sockfd, &p, sizeof(struct pack_dtr), 0,
			   (struct sockaddr *)&remote_sock, sizeof(remote_sock)) == -1) {
		slog(300, SLOG_ERROR, "Failed to send packet %d to %s: %s", p.pack_num,
			 addrstr(), strerror(errno));
	}
}
