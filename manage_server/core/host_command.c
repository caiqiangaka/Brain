
#include <stdio.h>
#include <string.h>

#include "type.h"
#include "host_cmd_if_lib.h"
#include "msg_proto_lib.h"
#include "host_command.h"
#include "logs.h"
#include "poll_command.h"
#include "usr_module.h"

#include "test_demo.h"

STATUS init_host_command_register(CONFIG_DATA_T *root )
{
	test_demo_host_cmd_register();
	
	return OK;
}

LOCAL USR_MODULE_T  host_command_module =
{
	HOST_MSG_CMD_MODULE,
	"host command module",
	LOG_LVL_NORMAL,
	init_host_command_register,
};

STATUS host_command_module_register(void)
{
	STATUS ret;
	
	ret = usr_module_register(&host_command_module);

	if(ret != OK)
		return HOST_COMMAND_ERR(E_REG);
	return OK;
		
}
