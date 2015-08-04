#include <stdio.h>
#include <string.h>
#include "type.h"
#include "unix_communicate_driver.h"
#include "communicate_lib.h"

/*ipc message buf  v0.01*/
LOCAL char  msg_receive_buf[IPC_PROTO_BUF_LARGE];
LOCAL char  msg_send_buf[IPC_PROTO_BUF_LARGE];

/*ipc pcb*/
LOCAL IPC_DRIVER_SESSION_T *ipc_driver_ses_list = NULL;
LOCAL IPC_DRIVER_SESSION_T *cur_ipc_driver_ses = NULL;

LOCAL UINT16 ipc_corr_tag_num = 0 ;

UINT16 get_corr_tag(void)
{
	ipc_corr_tag_num++;
	
	return ipc_corr_tag_num;	
}

char *  host_cmd_send_buf_get(UINT16 *buf_len,UINT16 size,UINT16 flag,UINT16 type)
{
	IPC_FRM_T *pFrame =NULL;
	IPC_MSG_HDR_T *pProto = NULL;
	HOST_CMD_T * pCmd = NULL;

	*buf_len = size+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE;

	memset(msg_send_buf, 0, *buf_len);

	/*fill  IPC protocol frame header */
	pFrame = BUF_TO_FRM_HEAD(msg_send_buf);
	pFrame->client_type = CLI_CLIENT;
	pFrame->ver = 0x01;
	pFrame->segs = 1;

	/*fill  IPC  protocol  header */
	pProto = BUF_TO_PRTO_HEAD(msg_send_buf);
	pProto->type = flag;
	pProto->len = *buf_len-IPC_FRM_HDR_SIZE;
	
	/*fill  IPC  command header */
	pCmd = BUF_TO_CMD_HEAD(msg_send_buf);
	pCmd->type = type;
	pCmd->corrTag =get_corr_tag();
	pCmd->len = size;

	//return msg_send_buf;
	return (msg_send_buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE);
}

/*receive ipc host command*/
 ssize  ipc_msg_receive(void *buf,ssize len)
{
	ssize size;

	if(cur_ipc_driver_ses == NULL)
	{
		cur_ipc_driver_ses = ipc_driver_ses_list;
		if(cur_ipc_driver_ses == NULL)
			return ERROR_E;
	}
	
	if(cur_ipc_driver_ses->name != NULL && cur_ipc_driver_ses->ipc_receive != NULL)
		size = (*cur_ipc_driver_ses->ipc_receive)(buf,len);
	
	return size;
}

/*send ipc host command*/
ssize ipc_msg_send(void *buf,ssize len)
{
	ssize size;

	if(cur_ipc_driver_ses == NULL)
	{
		cur_ipc_driver_ses = ipc_driver_ses_list;
		if(cur_ipc_driver_ses == NULL)
			return ERROR_E;
	}
	
	if(cur_ipc_driver_ses->name != NULL && cur_ipc_driver_ses->ipc_send != NULL)
		size =  (*cur_ipc_driver_ses->ipc_send)(buf,len);	
	//return  (*ipc_session.send)(buf,len);

	return size;
}

STATUS ipc_host_cmd_send(UINT16 send_len,int timeout)
{
	ssize size;
	
	timeout = timeout;
	
	size = ipc_msg_send(msg_send_buf,send_len);
	if(size > 0)
	{
		size = ipc_msg_receive(msg_receive_buf,IPC_PROTO_BUF_LARGE);
	}
	
	return size;
}

char* host_cmd_rec_buf_get(void)
{
	return (msg_receive_buf + IPC_FRM_HDR_SIZE + IPC_MSG_HDR_SIZE + HOST_CMD_SIZE);
}

STATUS ipc_driver_register(IPC_DRIVER_SESSION_T* p_ipc_driver_sess)
{
	IPC_DRIVER_SESSION_T* pPrev = NULL;
	IPC_DRIVER_SESSION_T* pCur = NULL;

	if(p_ipc_driver_sess == NULL)
		return ERROR_E;

	pCur = ipc_driver_ses_list;
	if(pCur == NULL)
	{
		ipc_driver_ses_list = p_ipc_driver_sess;
		p_ipc_driver_sess->next = NULL;
		return OK;
	}

	while(pCur != NULL)
	{
		if((strcmp(pCur->name, p_ipc_driver_sess->name)) == 0)
			return ERROR_E;

		pPrev = pCur;
		pCur = pCur->next;
	}


	pPrev->next = p_ipc_driver_sess;
	p_ipc_driver_sess->next = pCur;	

	return OK;
}

char* host_cmd_send_buf_get_new(UINT16 *buf_len,UINT16 size,UINT16 flag,const char *name)
{
	IPC_FRM_T *pFrame =NULL;
	IPC_MSG_HDR_T *pProto = NULL;
	HOST_CMD_NEW_T * pCmd = NULL;

	*buf_len = size+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE;

	memset(msg_send_buf, 0, *buf_len);

	/*fill  IPC protocol frame header */
	pFrame = BUF_TO_FRM_HEAD(msg_send_buf);
	pFrame->client_type = CLI_CLIENT;
	pFrame->ver = 0x01;
	pFrame->segs = 1;

	/*fill  IPC  protocol  header */
	pProto = BUF_TO_PRTO_HEAD(msg_send_buf);
	pProto->type = flag;
	pProto->len = *buf_len-IPC_FRM_HDR_SIZE;
	
	/*fill  IPC  command header */
	pCmd = BUF_TO_CMD_HEAD_NEW(msg_send_buf);
	strncpy(pCmd->name,name,HASH_NAME_LEN_MAX);
	pCmd->corrTag = get_corr_tag();
	pCmd->len = size;

	//return msg_send_buf;
	return (msg_send_buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE);
}

char *  host_cmd_rec_buf_get_new(void)
{
	return (msg_receive_buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE);
}

STATUS ipc_communicate_with_server( void* pCmd, UINT16 cmd_len, const char* cmd_name )
{
	char*  pbuf;
	UINT16 size;
	STATUS status = OK;

	/* check param */
	if ( NULL == pCmd )
	{
		return E_NODATA;
	}

	/* get ipc command send buffer and fill frame header */
	pbuf = host_cmd_send_buf_get_new( &size, cmd_len, HOST_CMD_CLASS_CONFIG_NEW, cmd_name );

	/* fill send command data */
	memcpy( pbuf, TYPE_TO_TYPE( pCmd, char ), cmd_len );

	/* send command data and wait resp */
	status = ipc_host_cmd_send( size, TRUE );
	if ( 0 > status )
	{
		// usr_log( "get_frame_slot_base_info_ipc_lib recv failed!" );
		return E_NODATA;
	}

	/* get resp command data */
	pbuf = host_cmd_rec_buf_get_new();
	RESP_STATUS( status );
	if ( OK != status )
	{
		return status;
	}

	memcpy( TYPE_TO_TYPE( pCmd, char ), TYPE_TO_TYPE( pbuf + 4, UINT8 ), cmd_len );

	return OK;
}
