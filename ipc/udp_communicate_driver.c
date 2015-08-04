#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>

#include "communicate_lib.h"
#include "udp_communicate_driver.h"
#include "type.h"


UINT32 ipaddress;
LOCAL char ip_address[16];

LOCAL IPC_DRIVER_SESSION_T udp_dirver;
LOCAL UDP_SOCKET_DRIVE_T socket_drive;

STATUS  udp_service_open(void)
{
	return OK;
}

STATUS  udp_socket_close(void)
{
   	return close(socket_drive.socket);	  
}

STATUS udp_socket_set_ip(const char* ip_addr)
{
	if ( NULL == ip_addr )
	{
		return ERROR;
	}
	
	memset( ip_address, 0, sizeof( ip_address ) );
	memcpy( ip_address, ip_addr, sizeof( ip_address ) - 1 );
	
	return OK;
}

STATUS udp_socket_connect_test(void)
{
	if ((socket_drive.socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
	   	return ERROR_E; 
	}
	
	socket_drive.address.sin_family = AF_INET;
	socket_drive.address.sin_port = htons(LOCAL_UDP_PORT);
	inet_pton(AF_INET,ip_address,&socket_drive.address.sin_addr.s_addr);
	bzero(&(socket_drive.address.sin_zero),8);

	if(connect(socket_drive.socket,(struct sockaddr *)&socket_drive.address,sizeof(struct sockaddr))<0)
	{
		udp_socket_close();
		return  ERROR_E;
	}
	
	udp_socket_close();
	
	return OK;
}

ssize udp_socket_receive(void *buf ,ssize len)
{
	int fd_select;
	fd_set fd_read;
	struct timeval time;
	int size;

#if 1

	FD_ZERO(&fd_read);
	FD_SET(socket_drive.socket, &fd_read);

	time.tv_sec = 1;
	time.tv_usec = 0;

	fd_select = select(socket_drive.socket+1, &fd_read, NULL, NULL, &time);
	if(fd_select)
	{
		size = recv(socket_drive.socket, buf, len, 0);
		udp_socket_close();
		return size;
	}
	udp_socket_close();
	return ERROR_E;
	
#else

	size = recv(socket_drive.socket, buf, len, 0);
	udp_socket_close();

	return size;
#endif
}

ssize  udp_socket_write(void *buf,ssize len)
{
	int size;

	if ((socket_drive.socket= socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("udp_socket_write create fail \r\n");
	   	return ERROR_E; 
	}

	socket_drive.address.sin_family = AF_INET;
	socket_drive.address.sin_port = htons(LOCAL_UDP_PORT);
	inet_pton(AF_INET,ip_address,&socket_drive.address.sin_addr.s_addr);
	//socket_drive.address.sin_addr.s_addr =ipaddress;
	bzero(&(socket_drive.address.sin_zero),8);

#if  1
	if(connect(socket_drive.socket,(struct sockaddr *)&socket_drive.address,sizeof(struct sockaddr))<0)
	{
		perror("connect F_SETFL");
		udp_socket_close();
		return  ERROR_E;
	}
	
	size = send(socket_drive.socket, buf, len, 0);
#endif
	//size = sendto(socket_drive.socket,buf,len,0,(struct sockaddr*)&socket_drive.address,sizeof(struct sockaddr));

	return size;
}

IMPORT  STATUS udp_socket_drive_init(void )
{
	STATUS ret =OK;

	strncpy(udp_dirver.name,"udp socket",32); 
	
	udp_dirver.ipc_open = udp_service_open;
	udp_dirver.ipc_close = udp_socket_close;
	udp_dirver.ipc_receive = udp_socket_receive;
	udp_dirver.ipc_send = udp_socket_write;

	ret = ipc_driver_register(&udp_dirver);

	return ret;
}



