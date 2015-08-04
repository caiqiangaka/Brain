#ifndef _UNIX_DOMAIN_SOCKET_H_
#define	_UNIX_DOMAIN_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>

#include "type.h"
#include "udp_communicate_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOCKET_MAX_LISTEN		5
#define UNIX_DOMAIN_DIR_NAME	"/tmp/.brain_manage_server"


typedef struct net_socket_drive
{
	int socket;
	int len;
	struct sockaddr_un address;
	char name[100];			
}SOCKET_DRIVE_T;

void  communicate_driver_init(void);

#ifdef __cplusplus
}
#endif


#endif

