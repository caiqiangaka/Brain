
#ifndef 	_UNIX_DOMAIN_SOCKET_H_
#define	_UNIX_DOMAIN_SOCKET_H_


#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif


#define  SOCKET_MAX_LISTEN		5

#define    UNIX_DOMAIN_DIR_NAME	"/tmp/.brain_manage_server"


IMPORT STATUS unix_socket_drive_init(void );


#ifdef __cplusplus
}
#endif


#endif

