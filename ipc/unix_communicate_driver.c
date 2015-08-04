#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include "communicate_lib.h"
#include "unix_communicate_driver.h"

LOCAL IPC_DRIVER_SESSION_T unix_dirver;
LOCAL int unix_domain_socket_init = 0;
LOCAL SOCKET_DRIVE_T socket_drive;

STATUS unix_service_open(void)
{
	return OK;
}

STATUS unix_socket_close(void)
{
   	return close(socket_drive.socket);	  
}

ssize unix_socket_receive(void *buf ,ssize len)
{
	int fd_select;
	fd_set fd_read;
	struct timeval time;
	int size;

	FD_ZERO(&fd_read);
	FD_SET(socket_drive.socket, &fd_read);

	time.tv_sec = 1;
	time.tv_usec = 0;

	fd_select = select(socket_drive.socket+1, &fd_read, NULL, NULL, &time);
	if(fd_select)
	{
		size = recv(socket_drive.socket, buf, len, 0);
		unix_socket_close();
		return size;
	}
	unix_socket_close();
	return  ERROR_E;
}

ssize unix_socket_write(void *buf,ssize len)
{
	int size;

	if ((socket_drive.socket= socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		printf("unix_socket_write create fail \r\n");
	   	return ERROR_E; 
	}

	memset(&socket_drive.address, 0, sizeof(socket_drive.address)); 
	socket_drive.address.sun_family = AF_UNIX; 
	strcpy(socket_drive.address.sun_path,UNIX_DOMAIN_DIR_NAME); 
	socket_drive.len =  sizeof(socket_drive.address);
	
	if(connect(socket_drive.socket,(struct sockaddr *)&socket_drive.address,socket_drive.len)<0)
	{
		unix_socket_close();
		perror("connect F_SETFL");
		return ERROR_E;
	}
	
	size = send(socket_drive.socket, buf, len, 0);

	return size;
	
}

IMPORT STATUS unix_socket_drive_init(void )
{
	STATUS ret =OK;

	strncpy(unix_dirver.name,"unix domain",32); 
	
	unix_dirver.ipc_open = unix_service_open;
	unix_dirver.ipc_close = unix_socket_close;
	unix_dirver.ipc_receive = unix_socket_receive;
	unix_dirver.ipc_send = unix_socket_write;

	ret = ipc_driver_register(&unix_dirver);

	return ret;
}

void communicate_driver_init(void)
{
	if(unix_domain_socket_init == 0)
	{
		unix_domain_socket_init = 1;

		//unix_socket_drive_init();
		udp_socket_drive_init();
	}
}
