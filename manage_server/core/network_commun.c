
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "msg_proto_lib.h"
#include "usr_module.h"
#include "logs.h"

#include "network_commun.h"

#include "unix_domain_socket.h"
#include "socket_udp.h"

LOCAL STATUS network_commnu_drive_init(CONFIG_DATA_T *root)
{
	//unix_socket_drive_init();
	udp_unix_socket_drive_init();
	
	return OK;
}

LOCAL USR_MODULE_T host_msg_diver_module =
{
	HOST_MSG_DRIVE_MODULE,
	"host message  dirver  module",
	LOG_LVL_NORMAL,
	network_commnu_drive_init,
};

STATUS host_msg_dirver_module_register(void)
{
	STATUS ret;

	ret = usr_module_register(&host_msg_diver_module);

	if(ret != OK)
		return HOST_MSG_DRIVER_ERR(E_REG);
	return OK;
		
}

