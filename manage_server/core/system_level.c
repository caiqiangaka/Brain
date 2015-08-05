
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include "system_level.h"


LOCAL   struct timeval  poll_timeout;

STATUS  timeout(void)
{
	struct timeval current_time;

	/*get system time*/
	gettimeofday(&current_time,NULL);

	if(current_time.tv_sec >poll_timeout.tv_sec || \
		(current_time.tv_sec ==poll_timeout.tv_sec && \
		current_time.tv_usec > poll_timeout.tv_usec))
		return TRUE;
	else
		return FALSE;
}

void set_poll_timeout(ULONG time)
{
	struct timeval current_time;

	/*get system time*/
	gettimeofday(&current_time,NULL);

	poll_timeout.tv_sec = current_time.tv_sec + time/1000;
	poll_timeout.tv_usec = current_time.tv_usec + time%1000*1000;
}







