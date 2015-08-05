
#ifndef _H_HOST_CMD_IF_LIB_H_
#define _H_HOST_CMD_IF_LIB_H_


#include  "type.h"


#ifdef __cplusplus
extern "C" {
#endif


/* host interface command header */
typedef struct
{
	UINT16	type;		/* command type */
	UINT16	corrTag;	/* correlation tag*/
	UINT16	len;			/* number of bytes of following data*/
}__attribute__ ((packed))  HOST_CMD_T;

#define  HOST_CMD_SIZE   sizeof(HOST_CMD_T)




/* host interface command process function prototype */
typedef STATUS (*HOST_CMD_FUNC)(HOST_CMD_T *pCmd, BOOL wait);





/* host command node */
typedef struct host_cmd_node
{
	UINT16			type;	/* host command type */
	HOST_CMD_FUNC	func;	/* process function pointer */
	struct host_cmd_node	*next;	/* pointer to next node */
}HOST_CMD_NODE_T, *HOST_CMD_NODE_ID;
#define  HOST_CMD_NODE_SIZE   sizeof(HOST_CMD_NODE_T)



/* host interface command header new */
typedef struct
{
	char		name[64];	/* command name */
	UINT16	corrTag;	/* correlation tag*/
	UINT16	len;			/* number of bytes of following data*/
}__attribute__ ((packed))  HOST_CMD_NEW_T;

#define  HOST_CMD_NEW_SIZE   sizeof(HOST_CMD_NEW_T)



/* host interface command process function prototype */
typedef STATUS (*HOST_CMD_FUNC_NEW)(HOST_CMD_NEW_T *pCmd, BOOL wait);



#define HASH_NAME_LEN_MAX		64
/*host command node new*/
typedef struct host_cmd_node_new {
    char  	name[HASH_NAME_LEN_MAX];			/*host command name*/
    HOST_CMD_FUNC_NEW	func;				/* process function pointer */
    struct host_cmd_node_new  *next;                    /* pointer to next node */
} HOST_CMD_NODE_NEW_T, *HOST_CMD_NODE_NEW_ID;
#define  HOST_CMD_NODE_NEW_SIZE   sizeof(HOST_CMD_NODE_NEW_T)



IMPORT STATUS host_msg_if_module_register(void);
IMPORT STATUS host_cmd_register_arr(HOST_CMD_NODE_T cmdArr[], int count);
IMPORT STATUS host_cmd_register( HOST_CMD_NODE_T *pCmd);
IMPORT STATUS host_cmd_proc(HOST_CMD_T *pCmd, BOOL wait);

IMPORT STATUS host_cmd_register_arr_new(HOST_CMD_NODE_NEW_T cmdArr[], int count);
IMPORT STATUS host_cmd_register_new( HOST_CMD_NODE_NEW_T *pCmd);
IMPORT STATUS host_cmd_proc_new(HOST_CMD_NEW_T *pCmd, BOOL wait);


#ifdef __cplusplus
}
#endif

#endif

