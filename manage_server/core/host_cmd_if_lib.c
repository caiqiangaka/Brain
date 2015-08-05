
#include <stdio.h>
#include <string.h>


#include "type.h"
#include "usr_module.h"
#include "logs.h"
#include "usr_module.h"
#include "host_cmd_if_lib.h"

#define	HOST_MSG_IF_ERR(x)	USR_ERR(HOST_MSG_IF_MODULE, x)

#define	HOST_CMD_HASH_SIZE	256
LOCAL HOST_CMD_NODE_ID	host_cmd_hash_tbl[HOST_CMD_HASH_SIZE];

#define host_cmd_hash(type)	\
	((UINT8)(((type) >> 1) & 0xff))

#define	HOST_CMD_HASH_NEW_SIZE	1024
LOCAL HOST_CMD_NODE_NEW_ID	host_cmd_hash_new_tbl[HOST_CMD_HASH_NEW_SIZE];

LOCAL STATUS host_msg_if_dump()
{
	return OK;
}

LOCAL void host_cmd_insert( HOST_CMD_NODE_T *pCmd)
{
	HOST_CMD_NODE_T *pNode = NULL, *pPrev = NULL;

	if(pCmd != NULL && pCmd->next != NULL)
	{
		/* if the node's link pointer to the next node is not NULL,
		 * it perhaps indicates this node has been registered, or
		 * this node has been corrupted.
		 */
		usr_log(LOG_LVL_NORMAL, HOST_MSG_IF_MODULE,
			"Host command node perhaps is already regitered!\r\n");
		
		return;
	}

	pNode = host_cmd_hash_tbl[host_cmd_hash(pCmd->type)];
	while(pNode != NULL)
	{
		if(pNode->type == pCmd->type)
		{
			/* command node is already linked */
			return;
		}

		pPrev = pNode;
		pNode = pNode->next;
	}

	/* the node isn't linked, insert it */
	if(pPrev == NULL)
		host_cmd_hash_tbl[host_cmd_hash(pCmd->type)] = pCmd;
	else
		pPrev->next = pCmd;
}

STATUS host_cmd_register_arr( HOST_CMD_NODE_T cmdArr[], int count)
{
	if(cmdArr == NULL)
		return HOST_MSG_IF_ERR(E_PARAM);
	
	while(--count >= 0)
		host_cmd_insert(&cmdArr[count]);

	return OK;		
}

STATUS host_cmd_register( HOST_CMD_NODE_T *pCmd)
{
	host_cmd_insert(pCmd);
	
	return OK;		
}

LOCAL HOST_CMD_FUNC host_cmd_func_get( UINT16 type)
{
	HOST_CMD_NODE_T *pNode = NULL;
	HOST_CMD_FUNC	pFunc = NULL;

	for(pNode = host_cmd_hash_tbl[host_cmd_hash(type)];
		pNode != NULL;
		pNode = pNode->next)
	{
		if(pNode->type == type)
		{
			pFunc = pNode->func;
			break;
		}
	}

	return pFunc;
}

STATUS host_cmd_proc(HOST_CMD_T *pCmd, BOOL wait)
{
	HOST_CMD_FUNC pFunc = NULL;
	
	if(pCmd == NULL)
		return HOST_MSG_IF_ERR(E_PARAM);

	pFunc = host_cmd_func_get(pCmd->type);
	if(pFunc != NULL)
	{
		usr_log(LOG_LVL_DEBUG, HOST_MSG_IF_MODULE,
			"Process self-defined command(T%#x/N%#x)\r\n",
			pCmd->type, pCmd->corrTag);

		return (*pFunc)(pCmd, wait);
	}
	
	return HOST_MSG_IF_ERR(E_NORSRC);
}

/*        hash name 		*/

#ifndef BITSPERBYTE
    #define BITSPERBYTE     ((int) (8 * sizeof(char)))
#endif

#ifndef BITS
    #define BITS(type)      ((int) (BITSPERBYTE * (int) sizeof(type)))
#endif

LOCAL  STATUS  host_cmd_hash_new(char *name)
{
    int	 	len;
    UINT32 	sum;
    int     i;

    i   = 0;
    sum = 0;
    len = 0;
	
    while (*name && len<HASH_NAME_LEN_MAX) 
    {
        sum += (((int) *name++) << i);
        i = (i + 7) % (BITS(int) - BITSPERBYTE);
		len++;
    }
	
    return sum % HOST_CMD_HASH_NEW_SIZE;
}

STATUS host_cmd_register_arr_new( HOST_CMD_NODE_NEW_T cmdArr[], int count)
{
	if(cmdArr == NULL)
		return HOST_MSG_IF_ERR(E_PARAM);
	
	while(--count >= 0)
		host_cmd_register_new(&cmdArr[count]);

	return OK;		
}

LOCAL void host_cmd_insert_new( HOST_CMD_NODE_NEW_T *pCmd)
{
	HOST_CMD_NODE_NEW_T *pNode = NULL, *pPrev = NULL;
	UINT32 hash_key;

	if(pCmd != NULL && pCmd->next != NULL)
	{
		/* if the node's link pointer to the next node is not NULL,
		 * it perhaps indicates this node has been registered, or
		 * this node has been corrupted.
		 */
		usr_log(LOG_LVL_NORMAL, HOST_MSG_IF_MODULE,
			"Host command node perhaps is already regitered!\r\n");
		
		return;
	}

	hash_key = host_cmd_hash_new(pCmd->name);
	pNode = host_cmd_hash_new_tbl[hash_key];
	while(pNode != NULL)
	{
		if (strncmp(pNode->name, pCmd->name,HASH_NAME_LEN_MAX) == 0) 
		{
			/* command node is already linked */
			return;
		}

		pPrev = pNode;
		pNode = pNode->next;
	}

	/* the node isn't linked, insert it */
	if(pPrev == NULL)
		host_cmd_hash_new_tbl[hash_key] = pCmd;
	else
		pPrev->next = pCmd;

}

STATUS host_cmd_register_new( HOST_CMD_NODE_NEW_T *pCmd)
{
	host_cmd_insert_new(pCmd);
	return OK;		
}

LOCAL HOST_CMD_FUNC_NEW  host_cmd_func_get_new( char *name)
{
	HOST_CMD_NODE_NEW_T *pNode = NULL;
	HOST_CMD_FUNC_NEW pFunc = NULL;
	UINT32 hash_key;

	if(name == NULL)
		return NULL;

	hash_key = host_cmd_hash_new(name);
	
	for(pNode = host_cmd_hash_new_tbl[hash_key];
		pNode != NULL;
		pNode = pNode->next)
	{
		if (strncmp(pNode->name, name,HASH_NAME_LEN_MAX) == 0) 
		{
			pFunc = pNode->func;
			break;
		}
	}

	return pFunc;
}

STATUS host_cmd_proc_new(HOST_CMD_NEW_T *pCmd, BOOL wait)
{
	HOST_CMD_FUNC_NEW pFunc = NULL;
	
	if(pCmd == NULL)
		return HOST_MSG_IF_ERR(E_PARAM);

	pFunc = host_cmd_func_get_new(pCmd->name);
	if(pFunc != NULL)
	{
		usr_log(LOG_LVL_DEBUG, HOST_MSG_IF_MODULE,
			"Process self-defined command(name:%s   N%#x)\r\n",
			pCmd->name, pCmd->corrTag);

		return (*pFunc)(pCmd, wait);
	}
	return HOST_MSG_IF_ERR(E_NORSRC);
}

LOCAL STATUS   host_msg_if_init(CONFIG_DATA_T *root )
{
	return OK;
}

LOCAL USR_MODULE_T host_msg_if_module =
{
	HOST_MSG_IF_MODULE,
	"host message interface module",
	LOG_LVL_MAJOR,
	host_msg_if_init,
	host_msg_if_dump
};

STATUS host_msg_if_module_register(void)
{
	STATUS ret;
	ret = usr_module_register(&host_msg_if_module);

	if(ret != OK)
		return HOST_MSG_IF_ERR(E_REG);
	return OK;
		
}

