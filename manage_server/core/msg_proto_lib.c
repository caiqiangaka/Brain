
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "host_cmd_if_lib.h"

#include "logs.h"
#include "msg_proto_lib.h"
#include "usr_module.h"


#define	HOST_MSG_PRTO_ERR(x) USR_ERR(HOST_MSG_PROTO_MODULE, x)


/*ipc message buf  v0.01*/
LOCAL char  msg_receive_buf[IPC_PROTO_BUF_LARGE];
LOCAL char  msg_send_buf[IPC_PROTO_BUF_LARGE];

fd_set  task_readfd;

/*ipc pcb  driver list*/
LOCAL IPC_DRIVER_SESSION_T *ipc_driver_ses_list = NULL;
LOCAL IPC_DRIVER_SESSION_T *cur_ipc_driver_ses = NULL;

LOCAL STATUS host_msg_proto_dump()
{
	IPC_DRIVER_SESSION_T *pCur = NULL;

	pCur = ipc_driver_ses_list;

	printf("IBCP  device  inforamtion\r\n");
	while(pCur != NULL)
	{
		printf("devic name: %s\r\n",pCur->name);
		pCur = pCur->next;
	}
	
	return OK;
}

LOCAL  STATUS host_msg_proto_init(CONFIG_DATA_T *root)
{
	IPC_DRIVER_SESSION_T *pCur = NULL;
	int fd;
	STATUS status =OK;

	pCur = ipc_driver_ses_list;
	
	if(pCur == NULL)
		return HOST_MSG_PRTO_ERR(E_PARAM);

	FD_ZERO(&task_readfd);
	while(pCur != NULL)
	{
		if(pCur->ipc_open != NULL)
		{
			usr_log(LOG_LVL_DEBUG, HOST_MSG_PROTO_MODULE,
					"init  run ipc driver name:%s \r\n",pCur->name);
			fd = (*pCur->ipc_open)();
			if(fd < 0)
			{
				usr_log(LOG_LVL_MAJOR, HOST_MSG_PROTO_MODULE,
					"init  run ipc driver name:%s fail\r\n",pCur->name);
				status = ERROR;
			}
			else
			{
				pCur->fd = fd;
				FD_SET(fd, &task_readfd);
			}
		}
		
		pCur = pCur->next;
	}
	
	return status;
}

/*receive ipc host command*/
 ssize  ipc_msg_receive(void *buf,ssize len)
{
	IPC_DRIVER_SESSION_T *pCur = NULL;
	struct timeval time;
	ssize  size = -1;

	time.tv_sec = 0;
	time.tv_usec = 100000;

	pCur = ipc_driver_ses_list;
	if(pCur == NULL)
		return ERROR;

	while(pCur != NULL)
	{
		FD_SET(pCur->fd,&task_readfd);
		pCur = pCur->next;
	}

	pCur = ipc_driver_ses_list;
	if(select(FD_SETSIZE,&task_readfd,NULL,NULL,&time)>0)
	{
		while(pCur != NULL)
		{
			if(FD_ISSET(pCur->fd,&task_readfd)>0)
			{
				cur_ipc_driver_ses = pCur;
				size = (*cur_ipc_driver_ses->ipc_receive)(buf,len);
				return size;
			}
			pCur = pCur->next;
		}
	}

	return size;
}


/*send ipc host command*/
ssize ipc_msg_send(ssize len)
{
	ssize size;

	if(cur_ipc_driver_ses == NULL)
		return ERROR;

	/*running   receive function*/
	if( cur_ipc_driver_ses->ipc_send != NULL)
		size = (*cur_ipc_driver_ses->ipc_send)(msg_send_buf,len);

	if(size > 0)
		debug_packets(LOG_LVL_DEBUG,HOST_MSG_PROTO_MODULE,"send",msg_send_buf,size);
	
	return size;
}

LOCAL STATUS ipc_pss_through_proc(void)
{
	return OK;
}

STATUS ipc_msg_proto_parser(void)
{
	IPC_FRM_T		*pHdr = NULL;
	IPC_MSG_HDR_T	*pMsg = NULL;
	HOST_CMD_T		*pCmd = NULL;
	HOST_CMD_NEW_T *pCmd_new = NULL;
	int	segs, len;
	int size;
	STATUS ret = OK;
	
	if((size=ipc_msg_receive((void *)msg_receive_buf,IPC_PROTO_BUF_LARGE)) <0)		
		return  HOST_MSG_PRTO_ERR(E_NORSRC);

	pHdr = (IPC_FRM_T *)msg_receive_buf;

	if ( 0 )
	{
	
	}
	else	
	{
		debug_packets(LOG_LVL_DEBUG,HOST_MSG_PROTO_MODULE,"recv",msg_receive_buf,size);
	
		usr_log(LOG_LVL_DEBUG, HOST_MSG_PROTO_MODULE,
			"Receive packet from  %d   with %d msg(s)\r\n", pHdr->client_type, pHdr->segs);

		len = IPC_FRM_HDR_SIZE;

		for(segs = 0; segs < pHdr->segs; segs++)
		{
			/* for safety, we check the total length */
			if(len > IPC_PROTO_BUF_LARGE)
			{
				usr_log(LOG_LVL_NORMAL, HOST_MSG_PROTO_MODULE,
					"Invalid length in received packet\r\n");

				return HOST_MSG_PRTO_ERR(E_TOOMUCH);
			}
					
			/* locate the message */
			pMsg = (IPC_MSG_HDR_T *)(msg_receive_buf + len);


			/* dispatch IPC the message */
			switch(pMsg->type)
			{
			case HOST_CMD_CLASS_PASS_THR:
				ret |= ipc_pss_through_proc();
				break;
			case HOST_CMD_CLASS_CONFIG:
				pCmd = (HOST_CMD_T *)(msg_receive_buf + len+IPC_MSG_HDR_SIZE );
				ret |= host_cmd_proc(pCmd,TRUE);
				break;
			case HOST_CMD_CLASS_CONFIG_NEW:
				pCmd_new = (HOST_CMD_NEW_T *)(msg_receive_buf + len+IPC_MSG_HDR_SIZE );
				ret |= host_cmd_proc_new(pCmd_new,TRUE);
				break;					
			default:
				usr_log(LOG_LVL_DETAIL, HOST_MSG_PROTO_MODULE,
					"Invalid packet type(%d)!\r\n", pMsg->type);
				break;	
			}
			len += pMsg->len+IPC_MSG_HDR_SIZE;
		}
	}
	
	return ret;
}

char *  host_cmd_send_buf_get(UINT16 cmd_len,UINT8 flag,UINT16 type,UINT16 corr_tag)
{
	IPC_FRM_T *pFrame = NULL;
	IPC_MSG_HDR_T *pProto = NULL;
	HOST_CMD_T * pCmd = NULL;
	UINT16 buf_len;

	buf_len = cmd_len+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE + HOST_STATUS_SIZE+HOST_STATUS_SIZE;

	memset(msg_send_buf, 0, buf_len);

	/*fill  IPC protocol frame header */
	pFrame = BUF_TO_FRM_HEAD(msg_send_buf);
	pFrame->client_type = CLI_CLIENT;
	pFrame->ver = 0x01;
	pFrame->segs = 1;

	/*fill  IPC  protocol  header */
	pProto = BUF_TO_PRTO_HEAD(msg_send_buf);
	pProto->type = flag;
	pProto->len = buf_len-IPC_FRM_HDR_SIZE;
	
	/*fill  IPC  command header */
	pCmd = BUF_TO_CMD_HEAD(msg_send_buf);
	pCmd->type = type;
	pCmd->corrTag = corr_tag;
	pCmd->len = cmd_len+HOST_STATUS_SIZE;

	//return msg_send_buf;
	return (msg_send_buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE);
}

STATUS ipc_driver_register(IPC_DRIVER_SESSION_T  *p_ipc_driver_sess)
{
	IPC_DRIVER_SESSION_T *pPrev = NULL;
	IPC_DRIVER_SESSION_T *pCur = NULL;

	if(p_ipc_driver_sess == NULL)
		return HOST_MSG_PRTO_ERR(E_PARAM);

	usr_log(LOG_LVL_DEBUG, HOST_MSG_PROTO_MODULE,
		"register ipc driver name:%s \r\n",p_ipc_driver_sess->name);
	
	pCur = ipc_driver_ses_list;
	if(pCur == NULL)
	{
		ipc_driver_ses_list = p_ipc_driver_sess;
		p_ipc_driver_sess->next = NULL;
		return OK;
	}
	
	while(pCur != NULL)
	{
		if((strcmp(pCur->name,p_ipc_driver_sess->name)) == 0)
			return HOST_MSG_PRTO_ERR(E_EXIST);

		pPrev = pCur;
		pCur = pCur->next;
	}

	pPrev->next = p_ipc_driver_sess;
	p_ipc_driver_sess->next = pCur;	

	return OK;
}

char *  host_cmd_send_buf_get_new(UINT16 cmd_len,UINT8 flag,char *name,UINT16 corr_tag)
{
	IPC_FRM_T *pFrame =NULL;
	IPC_MSG_HDR_T *pProto = NULL;
	HOST_CMD_NEW_T * pCmd = NULL;
	UINT16 buf_len;

	buf_len = cmd_len+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE + HOST_STATUS_SIZE+HOST_STATUS_SIZE;

	memset(msg_send_buf, 0, buf_len);

	/*fill  IPC protocol frame header */
	pFrame = BUF_TO_FRM_HEAD(msg_send_buf);
	pFrame->client_type = CLI_CLIENT;
	pFrame->ver = 0x01;
	pFrame->segs = 1;

	/*fill  IPC  protocol  header */
    pProto = BUF_TO_PRTO_HEAD(msg_send_buf);
	pProto->type = flag;
	pProto->len = buf_len-IPC_FRM_HDR_SIZE;
	
	/*fill  IPC  command header */
	pCmd = BUF_TO_CMD_HEAD_NEW(msg_send_buf);
	strncpy(pCmd->name,name,HASH_NAME_LEN_MAX);
	pCmd->corrTag = corr_tag;
	pCmd->len = cmd_len+HOST_STATUS_SIZE;

	//return msg_send_buf;
	return (msg_send_buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE);
}

LOCAL USR_MODULE_T  host_msg_proto_module=
{
	HOST_MSG_PROTO_MODULE,
	"host message protocol module",
	LOG_LVL_MAJOR,
	host_msg_proto_init,
	host_msg_proto_dump
};

 STATUS host_msg_proto_module_register(void)
{
	STATUS ret;
	
	ret = usr_module_register(&host_msg_proto_module);
	if(ret != OK)
		return HOST_MSG_PRTO_ERR(E_REG);
	return OK;
}

