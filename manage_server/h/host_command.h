
#ifndef _H_HOST_COMMAND_H_
#define	_H_HOST_COMMAND_H_



#ifdef __cplusplus
extern "C" {
#endif



#include "type.h"
#include "host_cmd_if_lib.h"


#define	HOST_COMMAND_ERR(x)	USR_ERR(HOST_MSG_CMD_MODULE, x)









IMPORT STATUS host_command_module_register(void);



#ifdef __cplusplus
}
#endif

#endif
