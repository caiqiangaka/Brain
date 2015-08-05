
#ifndef	_USR_MODULE_H
#define	_USR_MODULE_H


#include "type.h"
#include "json_lib.h"

/*
		[31   ......    16]   [15   ....    0]
		-------------   ----------
                 module ID             error
*/
#define	USR_ERR(m,e)        (((m & 0xff)<<16) |e)           

#define MODULE_MAX_NUMBER     60

/* module number definition */
enum
{
	NULL_MODULE		= 0,		/* reserved number */

	/* module id 1~2 */		
	MANG_MODULE,				/*  module manager */
	LOGS_MODULE,				/* common user message log */

	/*module id 3~6*/
	HOST_MSG_DRIVE_MODULE,		/*host drive module*/
	HOST_MSG_PROTO_MODULE,		/* message proto module */
	HOST_MSG_IF_MODULE,			/* host command common interface module */
	HOST_MSG_CMD_MODULE,		/* host command function module*/



	/*module id 7~9*/
	POLL_DRIVE_MODUlE,			/* poll drive module  */
	POLL_IF_MODUlE,			/* poll command message interface module  */
	POLL_CMD_MODUlE,			/* poll command message  module  */

	/*system func id */
	SYS_FUNC_MODULE,			/*system function module */
	
	MODULE_NUM
};

/* common error number definition */
enum
{
	E_INDEX	= 1,		/* invlid index */
	E_NAME,			/* invalid name */
	E_PARAM,			/* invalid parameter(s) */
	E_NORSRC,			/* no resource */
	E_REG,				/* register failed */
	E_AGAIN,			/* do more than once */
	E_NOTREADY,		/* not ready */
	E_EXIST,			/* already exist */
	E_NOSUCH,			/* no such instance */
	E_CORRUPT,			/* corrupted */
	E_TOOMUCH,			/* too much instances */
	E_NOTSUPPORT,		/* feature not support */
	E_FAIL,				/* general failure */
	E_NODATA,			/* no get data */
	E_NOAUTH,			/* No authorization */
	E_UNKOWN,			/* unkown failure */
	
	/* E_COMMON is not assignable number, it is the start
	 * of module specific error number.
	 */
	E_COMMON,
};

typedef STATUS (*MODULE_INIT_T)(CONFIG_DATA_T *);			/*module init function*/
typedef STATUS (*MODULE_DUMP_T)(void);			/*module DUMP function*/
typedef STATUS (*MODULE_SAVE_T)(CONFIG_DATA_T *);			/*module save function*/
typedef STATUS (*MODULE_DEFAULT_T)(void);		/*module default function*/

/*usr module managment*/
typedef struct usr_module
{
	UINT8	module_id;			/*usr module ID*/
	const	char *name;			/*usr module name*/
	UINT8	logs_level_flag;			/*usr module logs level*/

	MODULE_INIT_T  		module_init;			/*moudle init*/
	MODULE_DUMP_T 		module_dump;    	/*moudle dump*/
	MODULE_SAVE_T 		module_save;     	/*module config save*/
	MODULE_DEFAULT_T 	module_default;		/*module default*/
}USR_MODULE_T;



IMPORT STATUS usr_module_register(USR_MODULE_T * pModule);
IMPORT STATUS usr_module_init(void);
IMPORT STATUS save_module(void);
IMPORT STATUS save_as_module(char *file_name);
IMPORT STATUS reset_module(void);
IMPORT STATUS default_module(void);
IMPORT const char * get_usr_module_name(UINT8 module_id);
IMPORT STATUS usr_module_set(UINT8 module_id,UINT8 log_level);
IMPORT UINT8  usr_module_get(UINT8 module_id);
IMPORT STATUS  usr_module_dump(UINT8 module_id);

#endif

