
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>



#include  "usr_module.h"
#include  "logs.h"

#include "unix_domain_socket.h"

#include "msg_proto_lib.h"
#include "network_commun.h"




LOCAL IPC_DRIVER_SESSION_T unix_dirver;

/*local  unix  socket session pcb*/
LOCAL NETWORK_SESSION_T   unix_ses;

LOCAL socklen_t  client_len;
LOCAL struct sockaddr_un client_address;




LOCAL int  unix_service_open(void)
{
	int server_len;

	struct sockaddr_un server_address; 



	strcpy(unix_ses.path_name,UNIX_DOMAIN_DIR_NAME); 

	unlink(unix_ses.path_name);										/* in case it already exists */ 


	if ((unix_ses.service_fd  = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		usr_log(LOG_LVL_CRITICAL, HOST_MSG_DRIVE_MODULE,
			"Host command drive socket open flail \r\n");
	   	return	ERROR; 
	}
	
	
	memset(&server_address, 0, sizeof(server_address)); 
	
  	server_address.sun_family = AF_UNIX; 
	strcpy(server_address.sun_path,unix_ses.path_name); 
	server_len=sizeof(server_address);

	if (bind(unix_ses.service_fd, (struct sockaddr *)&server_address, server_len) < 0)
  	{
  		usr_log(LOG_LVL_NORMAL, HOST_MSG_DRIVE_MODULE,
			"Host command drive bind  flail \r\n");
		close(unix_ses.service_fd);
		return	ERROR; 
  	}
	
	if(listen(unix_ses.service_fd,SOCKET_MAX_LISTEN)< 0)
	{
		usr_log(LOG_LVL_NORMAL, HOST_MSG_DRIVE_MODULE,
			"Host command drive listen  flail \r\n");
		close(unix_ses.service_fd);
		return	ERROR; 
	}

	return unix_ses.service_fd;
}


LOCAL STATUS  unix_socket_close(void)
{
   	close(unix_ses.client_fd);
	return OK;
}





LOCAL ssize   unix_socket_receive(void *buf ,ssize len)
{


	ssize size;


	unix_ses.client_fd=accept(unix_ses.service_fd,(struct sockaddr*)&client_address,&client_len);
	if(unix_ses.client_fd ==-1)
		return (-1);
	size =recv(unix_ses.client_fd, buf, len, 0);

	return size;
}



LOCAL ssize  unix_socket_write(void *buf,ssize len)
{
	int size;

	size = send(unix_ses.client_fd, buf, len, 0);
	unix_socket_close();
	return size;
	
}

IMPORT  STATUS unix_socket_drive_init(void )
{
	STATUS ret =OK;

	strncpy(unix_dirver.name,"unix domain",32); 
	
	unix_dirver.ipc_open = unix_service_open;
	unix_dirver.ipc_close = unix_socket_close;
	unix_dirver.ipc_receive = unix_socket_receive;
	unix_dirver.ipc_send = unix_socket_write;

	//unix_dirver.data = (void *)&unix_ses;

	ret = ipc_driver_register(&unix_dirver);

	return ret;
}



