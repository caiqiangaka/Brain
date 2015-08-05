#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg_proto_lib.h"
#include "poll_command.h"
#include "host_command.h"
#include "poll_msg_if_lib.h"
#include "host_cmd_if_lib.h"
#include "unix_domain_socket.h"
#include "system_level.h"
#include "task.h"

UINT32 gConsoleChannelNum = POLL_COMM_CHANNEL_COM3;

LOCAL  int	 running = 0;
LOCAL  void sigHandler(int signo)
{
	running = 1;
	
}

LOCAL void init_module()
{
	usr_log_module_register();
	host_msg_dirver_module_register();
	host_msg_proto_module_register();
	host_msg_if_module_register();
	host_command_module_register();
}

void task_thread()
{
	/*****   Single   process   mode   *****/
	if(timeout())
	{
		/*set  FALSE*/
		poll_hook_run(POLL_TYPE_DYNAMIC_MSG,FALSE);
		set_poll_timeout(POLL_TIMEOUT);
	}
}

int main(int argc, char *argv[])
{
	/*init signal*/
	signal(SIGCLD, SIG_IGN);
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	signal(SIGPIPE, SIG_IGN);

	if(argc == 2)
	{	
		if(strcmp(argv[1],"0")==0)
		{
			gConsoleChannelNum = POLL_COMM_CHANNEL_COM0;
		}
		else if(strcmp(argv[1],"1")==0)
		{
			gConsoleChannelNum = POLL_COMM_CHANNEL_COM1;
		}
		else if(strcmp(argv[1],"2")==0)
		{
			gConsoleChannelNum = POLL_COMM_CHANNEL_COM2;
		}
		else if(strcmp(argv[1],"3")==0)
		{
			gConsoleChannelNum = POLL_COMM_CHANNEL_COM3;
		}
		else
		{
			gConsoleChannelNum = POLL_COMM_CHANNEL_COM3;
		}
	}
	else
	{
		gConsoleChannelNum = POLL_COMM_CHANNEL_COM3;
	}
	printf("Console channel num = %d\r\n",gConsoleChannelNum);

	init_module();
	usr_module_init();

	set_poll_timeout(POLL_TIMEOUT);
	poll_hook_run(POLL_TYPE_STATIC_MSG,TRUE);
	
	if (running)
		running = 0;
		
	while (!running) {
		task_thread();
		ipc_msg_proto_parser();
	}

	return 1;

}
