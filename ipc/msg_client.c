#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <signal.h>

#include "type.h"
#include "unix_communicate_driver.h"

#include "test.h"

LOCAL  int	 finished = 0;
LOCAL  int	 test_number = 0;

LOCAL  void sigHandler(int signo)
{
    finished = 1;
}

int main()
{
	signal(SIGTERM, sigHandler);
	
	communicate_driver_init();

	if (finished)
        	finished = 0;
	while(!finished)
	{

	}
	
	return 0;
}
