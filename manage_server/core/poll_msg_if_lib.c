
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include  "logs.h"
#include "usr_module.h"

#include "poll_msg_if_lib.h"


#define	POLL_IF_ERR(x)	USR_ERR(POLL_IF_MODUlE, x)

LOCAL POLL_FUNC_NODE_T *poll_cmd_hook_list[POLL_TYPE_NUM];
LOCAL POLL_FUNC_NODE_T  *pNode_list = NULL;

LOCAL POLL_SESSION_T  poll_ses;

LOCAL char  poll_receive_buf[POLL_MAX_BUF];
LOCAL char  poll_send_buf[POLL_MAX_BUF];

LOCAL UINT8	 poll_dynamic_flag = POLL_DYNAMIC_ENABLE;
LOCAL UINT8	 poll_send_flag = POLL_SEND_ENABLE;


LOCAL POLL_ERROR_STATISTICS_T  poll_packets_statistics;


POLL_ERROR_STATISTICS_T * get_poll_packets_statistics(void)
{
	return &poll_packets_statistics;
}


LOCAL STATUS poll_interface_dump()
{
	POLL_FUNC_NODE_T *pNode = NULL;
	UINT8 type;

	type = POLL_TYPE_NUM;
	while(type --)
	{
		//printf("-----------message-----------\r\n");
		for(pNode = poll_cmd_hook_list[type];
			pNode != NULL;
			pNode = pNode->next)
		{
			if( pNode->func != NULL)
			{
				printf("poll node type:%d   name:   %s\r\n",type,pNode->name);
			}
		}
	}

	
	return OK;
}
LOCAL STATUS poll_interface_init(CONFIG_DATA_T *root)
{
	STATUS ret =OK;

	/*clear all  poll  packets statistics*/
	memset(((void *)&poll_packets_statistics),0,POLL_ERROR_STATISTICS_SIZE);


	/*init  poll run engine flags*/
	if(poll_dynamic_flag != POLL_DYNAMIC_ENABLE)
		poll_dynamic_flag = POLL_DYNAMIC_ENABLE;
	if(poll_send_flag != POLL_SEND_ENABLE)
		poll_send_flag = POLL_SEND_ENABLE;

	ret = (*poll_ses.poll_open)();
	
	return ret;
}


STATUS poll_hook_add(UINT8 type,POLL_FUNC_T func,const char *name)
{
	POLL_FUNC_NODE_T *pNode = NULL, *pPrev = NULL, *pCur = NULL;

	if(type >= POLL_TYPE_NUM || func == NULL)
		return POLL_IF_ERR(E_PARAM);

	pNode = (POLL_FUNC_NODE_T *)malloc(sizeof(POLL_FUNC_NODE_T));
	if(pNode == NULL)
		return POLL_IF_ERR(E_NORSRC);

	pNode->func = func;
	strncpy(pNode->name,name,50);
	
	pCur = poll_cmd_hook_list[type];
	while(pCur != NULL)
	{
		pPrev = pCur;
		pCur = pCur->next;
	}

	usr_log(LOG_LVL_DEBUG, POLL_IF_MODUlE,
		"Add a hook entry, type = %#x, func = %p\r\n",
		type, (int)func);
	
	if(pPrev == NULL)
		poll_cmd_hook_list[type] = pNode;
	else
		pPrev->next = pNode;
	
	pNode->next = pCur;	


	return OK;
}


STATUS poll_hook_run(UINT8 type, BOOL wait)
{
	POLL_FUNC_NODE_T *pNode = NULL;
	STATUS ret = OK;


	if(poll_dynamic_flag == POLL_DYNAMIC_DISABLE)
		return  POLL_IF_ERR(E_FAIL);

	if(type >= POLL_TYPE_NUM)
		return POLL_IF_ERR(E_PARAM);
	if(wait ==TRUE)
	{	/*true  mode*/
		for(pNode = poll_cmd_hook_list[type];
			pNode != NULL;
			pNode = pNode->next)
		{
			if( pNode->func != NULL)
			{
				/* combine the all the return codes */
				ret |= (*pNode->func)();
			}
		}
	}
	else
	{	/*false  mode*/
		if(pNode_list ==NULL)
		{
			pNode_list =poll_cmd_hook_list[type];
			if(pNode_list ==NULL)
				return ret;
		}
		
		if( pNode_list->func != NULL)
		{
			/* combine the all the return codes */
			ret= (*pNode_list->func)();
		}
		
		pNode_list = pNode_list->next;
	}

	return ret;	
}


 ssize poll_msg_receive(void *buf,ssize len)
{
	ssize size;

	size =  (*poll_ses.poll_receive)(buf,len);
	return size;
}

 /*send ipc host command*/
ssize  poll_msg_send(void *buf,ssize len,ssize recv_buf_len)
{
	ssize size;

	debug_packets(LOG_LVL_DEBUG,POLL_IF_MODUlE,"send",poll_send_buf,len);

	/*send poll buffer  msessage*/
	size = (*poll_ses.poll_send)(buf,len);
	/*send status statisics*/
	if(size < 0)
	{
		poll_packets_statistics.tx_continuous_error_packet++;
		poll_packets_statistics.tx_error_packet++;
		return ERROR;
	}
	else
		poll_packets_statistics.tx_continuous_error_packet =0;
	
	poll_packets_statistics.tx_packets++;
	
	/*receive   poll message*/
	size = poll_msg_receive(poll_receive_buf,recv_buf_len);
	/*receive status statisics*/
	if(size < 0)
	{
		poll_packets_statistics.rx_continuous_error_packet++;
		poll_packets_statistics.rx_error_packet++;
	}
	else
	{
		poll_packets_statistics.rx_continuous_error_packet=0;
		poll_packets_statistics.rx_packets++;
	}


	debug_packets(LOG_LVL_DEBUG,POLL_IF_MODUlE,"recv",poll_receive_buf,size);

	
	return  size;
}



char *  poll_cmd_rec_buf_get()
{
	return   (poll_receive_buf+POLL_MSG_HEADER_SIZE);
}


ssize  poll_send(UINT8 device,UINT8 oper_type,UINT8 cmd_type,void *buf,UINT16 cmd_len,UINT16 rev_cmd_len)
{
	POLL_MSG_HEADER_T *pPollHeader = NULL;
	UINT8 checksum=0;
	UINT16  buf_len;
	UINT16  msg_len;
	ssize size;
	char *pData;
	UINT16  rev_buf_len;
	
	UINT16 i;

	if (POLL_SEND_DISABLE == poll_send_flag)
		return ERROR;
	
       /******************************************************************************/
	/*                                                  PROTO  TYPE
	           2                 1                1              2                cmd_len              1			2
	header 0xaa55    cmdType     cmdNum    length      [data or (CU) ]     checksum     Tail   0x55aa

	---------------------------------------     --------------    --------     -----------
	               POLL_MSG_HEADER_SIZE                           cmd_len
	                         ----------------------------------------------------
	                                                         msg_len
	*/
	 /******************************************************************************/

	buf_len = cmd_len + POLL_MSG_HEADER_SIZE + 1+2; /*chksum 1 +  tail 1+1*/
	if(buf_len  >POLL_MAX_BUF)
		return ERROR;

	
	/*fill   poll  header*/
	pPollHeader = POLL_BUF_TO_FRM_HEAD(poll_send_buf);
	pPollHeader->magic =0x55AA;
	pPollHeader->device_cmd = ((oper_type &0xC0) |(device &0x3F));
	pPollHeader->type = cmd_type;

	msg_len = cmd_len+5;
	pPollHeader->len = swap16(msg_len);
	
	
	/*fill   data  */
	pData = (char *)buf;
	for(i=0;i<cmd_len;i++)
	{
		poll_send_buf[i+POLL_MSG_HEADER_SIZE] = *pData++;
	}

	/*chksum   and  fill*/
	checksum = pPollHeader->device_cmd + pPollHeader->type + msg_len;
	for(i=0;i<cmd_len;i++)
		checksum =checksum+poll_send_buf[i+POLL_MSG_HEADER_SIZE];
	poll_send_buf[cmd_len+POLL_MSG_HEADER_SIZE] = checksum;

	/*fill   tail*/
	poll_send_buf[buf_len-2] = 0x55;
	poll_send_buf[buf_len-1] = 0xaa;





	/*****************************************************************************************************/
	/*
		     2                 1                1              2           1                    recv data len                            1			2
	header 0xaa55    cmdType     cmdNum    length     status              [data or (CU) ]                        checksum     Tail   0x55aa
	-----------------------------------------------              ------------                        ---------------------
	                                     7                                                              rev_cmd_len                            3
	                                     ---------------------------------------------------------------------
	                                                                      rev_cmd_len + 10                         
	*/
	/****************************************************************************************************/
	
	rev_buf_len = rev_cmd_len+10;
	//printf("recv_buf_len1   %d  :%d\r\n",rev_cmd_len,rev_buf_len);
	
	size = poll_msg_send(poll_send_buf,buf_len,rev_buf_len);
	
	return size;
}

STATUS poll_engine_oper(UINT8 operation)
{
	switch ( operation )
	{
	case POLL_ENABLE:
		poll_dynamic_flag = POLL_DYNAMIC_ENABLE;
		poll_send_flag = POLL_SEND_ENABLE;
		break;
	case POLL_DISABLE:
		poll_dynamic_flag = POLL_DYNAMIC_DISABLE;
		poll_send_flag = POLL_SEND_DISABLE;
		break;
	case POLL_DYNAMIC_ENABLE:
		poll_dynamic_flag = POLL_DYNAMIC_ENABLE;
		break;
	case POLL_DYNAMIC_DISABLE:
		poll_dynamic_flag = POLL_DYNAMIC_DISABLE;
		break;
	case POLL_SEND_ENABLE:
		poll_send_flag = POLL_SEND_ENABLE;
		break;
	case POLL_SEND_DISABLE:
		poll_send_flag = POLL_SEND_DISABLE;
		break;
	default:
		return POLL_IF_ERR(E_PARAM);
		break;
	}

	return OK;
}

STATUS  poll_driver_register(
	POLL_SE_OPEN_T 	open,
	POLL_SE_CLOSE_T	close,
	POLL_SE_RECV_T receive,
	POLL_SE_SEND_T send
	)
{
	poll_ses.poll_open=open;
	poll_ses.poll_close=close;
	poll_ses.poll_receive=receive;
	poll_ses.poll_send=send;

	return OK;
}


LOCAL USR_MODULE_T  poll_if_module=
{
	POLL_IF_MODUlE,
	"poll interface Management module",
	LOG_LVL_NORMAL,
	poll_interface_init,
	poll_interface_dump
};

 STATUS poll_if_module_register(void)
{
	STATUS ret;
	
	ret = usr_module_register(&poll_if_module);

	if(ret != OK)
		return POLL_IF_ERR(E_REG);
	return OK;
		
}


