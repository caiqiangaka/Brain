
#ifndef 	_NETWORK_COMMUN_H_
#define	_NETWORK_COMMUN_H_


#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif


#define	HOST_MSG_DRIVER_ERR(x) 	USR_ERR(HOST_MSG_DRIVE_MODULE, x)


typedef struct
{
	int	service_fd;			/* service socket fd */
	int	client_fd;			/* client socket fd */
	char path_name[100];		/*unix socket path name*/
}NETWORK_SESSION_T;
#define NETWORK_SESSION_SIZE   sizeof(NETWORK_SESSION_T)






IMPORT STATUS host_msg_dirver_module_register(void);


#ifdef __cplusplus
}
#endif


#endif

