
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
//#include <linux/clk.h>


#include  "msg_proto_lib.h"
#include  "usr_module.h"
#include  "logs.h"
#include "network_commun.h"

#include "socket_udp.h"


LOCAL IPC_DRIVER_SESSION_T udp_dirver;

LOCAL NETWORK_SESSION_T   udp_ses;


LOCAL  struct sockaddr_in servAddr;
LOCAL  struct sockaddr_in cliAddr;
LOCAL UINT32 cliLen = sizeof(struct sockaddr_in);


LOCAL int  udp_service_open(void)
{
	   
	   int i=1;
	   
	   /*socket creation*/
	   udp_ses.service_fd= socket(AF_INET,SOCK_DGRAM,0);
	   if (udp_ses.service_fd < 0)
	   {
			usr_log(LOG_LVL_CRITICAL, HOST_MSG_DRIVE_MODULE,
				"upp socket open flail \r\n");
		   	return   ERROR; 
	   }
	   
	   /*bind local server port*/
	   servAddr.sin_family = AF_INET;
	   servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	   servAddr.sin_port = htons(LOCAL_UDP_PORT);
	   if (bind(udp_ses.service_fd,(struct sockaddr *)&servAddr,sizeof(servAddr)) < 0)
	   {
			usr_log(LOG_LVL_CRITICAL, HOST_MSG_DRIVE_MODULE,
				"upp socket bind  flail \r\n");
			close(udp_ses.service_fd);
		   	return  ERROR ; 
	   }

	   
   	   return udp_ses.service_fd;
	
}


LOCAL STATUS  udp_socket_close(void)
{

	return OK;
}

LOCAL ssize   udp_socket_receive(void *buf ,ssize len)
{

   	ssize size;
   	

   	size = recvfrom(udp_ses.service_fd,buf,len,0,(struct sockaddr *)&cliAddr,&cliLen);


	return size;
}



LOCAL ssize  udp_socket_write(void *buf,ssize len)
{
	ssize size;
	size = sendto(udp_ses.service_fd, buf, len, 0,(struct sockaddr *)&cliAddr,cliLen);
	return size;
	
}


IMPORT STATUS  udp_unix_socket_drive_init(void )
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


