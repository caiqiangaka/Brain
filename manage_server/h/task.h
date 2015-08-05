
#ifndef 	_TASK_H_
#define	_TASK_H_


#include "type.h"
#include  <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif




typedef void * (*TASK)(void*);
typedef void  (*TASK_POOL_MEM_RELEASE)(void*);


#define TASK_ID_T 		pthread_t
#define TASK_KEY_T 		pthread_key_t
#define TASK_MUTEX_T	pthread_mutex_t

#define DATA_MUTEX_T	TASK_MUTEX_T  mutex;
#define DATA_MUTEX_SIZE sizeof(TASK_MUTEX_T)










IMPORT STATUS  task_create(TASK_ID_T *p_task_id,TASK task_func,void *arg);
IMPORT STATUS  task_delete(void *data);
IMPORT STATUS  task_wait(TASK_ID_T task_id);

IMPORT STATUS init_task_pool_mem(TASK_POOL_MEM_RELEASE func);
IMPORT void  * get_task_pool_mem(void);
IMPORT STATUS  set_task_pool_mem(const void *value);



IMPORT STATUS  task_mutex_init(TASK_MUTEX_T  *mutex);
IMPORT STATUS  task_mutex_lock(TASK_MUTEX_T  *mutex);
IMPORT STATUS  task_mutex_unlock(TASK_MUTEX_T  *mutex);

#define TASK_MUTEX_VALUE(value) (&(value.mutex))
#define TASK_MUTEX_INIT(value) task_mutex_init(TASK_MUTEX_VALUE(value))
#define TASK_MUTEX_LOCK(value) task_mutex_lock(TASK_MUTEX_VALUE(value))
#define TASK_MUTEX_UNLOCK(value) task_mutex_unlock(TASK_MUTEX_VALUE(value))





#ifdef __cplusplus
}
#endif


#endif

