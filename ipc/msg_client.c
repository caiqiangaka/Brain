#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "type.h"
#include "unix_communicate_driver.h"
#include "udp_communicate_driver.h"

#include "test_demo.h"

LOCAL  int	 finished = 0;
LOCAL  int	 test_number = 0;

LOCAL  void sigHandler(int signo)
{
    finished = 1;
}

int main()
{
	TEST_DEMO_T stInfo;
	STATUS status = OK;
	UINT32 i = 0;
	
	signal( SIGTSTP, sigHandler ); // CTRL + Z
	
	communicate_driver_init();

	udp_socket_set_ip("192.168.1.106");

	if (finished)
        finished = 0;
		
	memset( &stInfo, 0, sizeof( stInfo ) );
	while(!finished)
	{
		stInfo.u32TestVal = i++;
		status = get_test_demo_ipc_lib( &stInfo );
		if ( OK != status )
		{
			printf( "get_test_demo_ipc_lib return error!\n" );
		}
		else
		{
			printf( "u32TestVal : %d, u32Status : %d\n", stInfo.u32TestVal, stInfo.u32Status );
		}
		usleep(1000000);
	}
	
	return 0;
}
