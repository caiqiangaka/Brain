
#ifndef _H_SYSTEM_LEVEL_H_
#define	_H_SYSTEM_LEVEL_H_


#include "type.h"
#include <sys/time.h>


#ifdef __cplusplus
extern "C" {
#endif



#define POLL_TIMEOUT     1000    /*ms */



typedef void  (*TIME_OUT_FUNC)(int);


IMPORT STATUS  timeout(void);
IMPORT void set_poll_timeout(ULONG time);

#ifdef __cplusplus
}
#endif

#endif

