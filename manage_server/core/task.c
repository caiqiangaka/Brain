/*task.c -  task   operation	*/
/*
 * Copyright(c) 2009 OPHYLINK COMMUNICATION TECHNOLOGY LTD
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable OPHYLINK license agreement.
 */

 
/*
modification history
--------------------
2015/3/14	sea.gu Created.  
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#include  <pthread.h>

#include "task.h"




LOCAL 	TASK_KEY_T 	task_pool_mem_key;




STATUS  task_create(TASK_ID_T *p_task_id,TASK task_func,void *arg)
{
	STATUS ret =OK;


	ret = pthread_create(p_task_id,NULL,task_func,arg);

	return ret;
}


STATUS  task_delete(void *data)
{
	STATUS ret =OK;


	pthread_exit(data);

	return ret;
}


STATUS  task_wait(TASK_ID_T  task_id)
{
	STATUS ret =OK;


	ret = pthread_join(task_id,NULL);

	return ret;
}





STATUS init_task_pool_mem(TASK_POOL_MEM_RELEASE func)
{
	STATUS ret =OK;
	
	ret = pthread_key_create( &task_pool_mem_key, func );
	
	return ret;
}




void  * get_task_pool_mem(void)
{
	return pthread_getspecific( task_pool_mem_key );
}


STATUS  set_task_pool_mem(const void *value)
{
	STATUS ret =OK;


	ret = pthread_setspecific( task_pool_mem_key, value );

	return ret;
}






STATUS  task_mutex_init(TASK_MUTEX_T  *mutex)
{
	STATUS ret =OK;


	ret = pthread_mutex_init(mutex,NULL);

	return ret;
}

STATUS  task_mutex_lock(TASK_MUTEX_T  *mutex)
{
	STATUS ret =OK;


	ret = pthread_mutex_lock(mutex);

	return ret;
}


STATUS  task_mutex_unlock(TASK_MUTEX_T  *mutex)
{
	STATUS ret =OK;


	ret = pthread_mutex_unlock(mutex);

	return ret;
}



