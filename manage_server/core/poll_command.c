
#include <stdio.h>
#include "type.h"
#include "poll_msg_if_lib.h"
#include "usr_module.h"
#include  "logs.h"

#include "poll_command.h"

STATUS   init_poll_command_register(CONFIG_DATA_T *root)
{
	return OK;
}

LOCAL USR_MODULE_T  poll_command_module=
{
	POLL_CMD_MODUlE,
	"poll command module",
	LOG_LVL_NORMAL,
	init_poll_command_register,
};

STATUS poll_command_module_register(void)
{
	STATUS ret;
	ret = usr_module_register(&poll_command_module);

	if(ret != OK)
		return POLL_COMMAND_ERR(E_REG);
	return OK;
		
}



