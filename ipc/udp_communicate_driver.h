#ifndef _SOCKET_UDP_H_
#define	_SOCKET_UDP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOCAL_UDP_PORT		 40088

typedef struct udp_socket_drive
{
	int socket;
	struct sockaddr_in address;	
}UDP_SOCKET_DRIVE_T;

IMPORT STATUS udp_socket_drive_init(void);
IMPORT STATUS udp_socket_set_ip(const char* ip_addr);
IMPORT STATUS udp_socket_connect_test(void);

#ifdef __cplusplus
}
#endif


#endif

