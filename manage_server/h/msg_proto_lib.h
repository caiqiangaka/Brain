
#ifndef	_MSG_PROTO_LIB_H_
#define	_MSG_PROTO_LIB_H_


#include <stdio.h>
#include  "type.h"



#ifdef __cplusplus
extern "C" {
#endif


#define  IPC_PROTO_BUF_LARGE   1024*4



typedef int	 (*IPC_SE_OPEN_T)(void);				/*ipc session open function*/
typedef STATUS (*IPC_SE_CLOSE_T)(void);			/*ipc session close function*/
typedef ssize  (*IPC_SE_RECV_T)(void *buf,int len);		/*ipc session receive function*/
typedef ssize  (*IPC_SE_SEND_T)(void *buf,int len);		/*ipc session send function*/


typedef struct ipc_driver_session
{
	char name[32];						/*driver name*/
	int	fd;							

	UINT8  enable;						/*0:enable 1:disable*/
	
	IPC_SE_OPEN_T	ipc_open;			/*open ipc  Communicate*/
	IPC_SE_CLOSE_T  ipc_close;			/*close ipc Communicate*/
	IPC_SE_RECV_T   ipc_receive;			/*receive message */
	IPC_SE_SEND_T   ipc_send;			/*send  message */

	struct ipc_driver_session  *next;

	void  	*data;						/*user data point*/
	
}IPC_DRIVER_SESSION_T;
#define IPC_DRIVER_SESSION_SIZE   sizeof(IPC_DRIVER_SESSION_T)



/*Client name*/
enum
{
	SNMP_CLIENT	= 1,		
	CLI_CLIENT,			
	WEB_CLIENT				
};

/* IPC protocol frame header structure */
typedef struct
{
	UINT8	client_type;		/* client  device ID */
	UINT8	ver;				/* IPC protocol version number */
	UINT8	segs;			/* number of following messages */
	char	msgs[0];			/* frame contents */
}__attribute__ ((packed))  IPC_FRM_T;

/*frame header length*/
#define IPC_FRM_HDR_SIZE   sizeof(IPC_FRM_T)


/* IPC protocol message header structure */
typedef struct
{
	UINT16	type;		/* message type */
	UINT16	len;			/* message length, including this header */
}__attribute__ ((packed)) IPC_MSG_HDR_T;

#define  IPC_MSG_HDR_SIZE   sizeof(IPC_MSG_HDR_T)

typedef enum
{
	HOST_CMD_CLASS_PASS_THR = 0,		/* Pass-through message   */
	HOST_CMD_CLASS_CONFIG,			/* Configuration message   */
	HOST_CMD_CLASS_CONFIG_NEW,		/* Configuration message  new */
	CMD_CLASS_NUM						/* number of command class */
}HOST_CMD_CLASS_T;












#define HOST_STATUS_SIZE	4

#define  TYPE_TO_SEND_BUF_VALUE(point,type1)   (*((type1 *)(point+1)))


#define  RESP_STATUS(status)    (memcpy(pbuf,(char *)&status,4))

#define TYPE_TO_TYPE(point,type1)   ((type1 *)(point+1))



#define TO_TYPE(point,type1)   ((type1 *)(point))
#define TYPE_TO_SEND_BUF(point,type1)   ((type1 *)(point+4))
#define TYPE_TO_REV_BUF(point,type1)   ((type1 *)(point+1))




#define BUF_TO_FRM_HEAD(buf)      	 ((IPC_FRM_T *)(buf))
#define BUF_TO_PRTO_HEAD(buf)      	 ((IPC_MSG_HDR_T *)(buf+IPC_FRM_HDR_SIZE))
#define BUF_TO_CMD_HEAD(buf)      	 ((HOST_CMD_T *)(buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE))
#define BUF_TO_CMD_DATA_HEAD(buf,type) 		((type *)(buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE))

/*!!!!!!!!*/
//#define BUF_LEN(len)		(len +IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE+HOST_STATUS_SIZE)
#define BUF_LEN(len)		(len +IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_SIZE+4)

//#define BUF_LEN(len)      (len +1)






#define BUF_TO_CMD_HEAD_NEW(buf)      	 ((HOST_CMD_NEW_T *)(buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE))
#define BUF_TO_CMD_DATA_HEAD_NEW(buf,type) 		((type *)(buf+IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE))
#define BUF_LEN_NEW(len)		(len +IPC_FRM_HDR_SIZE+IPC_MSG_HDR_SIZE+HOST_CMD_NEW_SIZE+4)













IMPORT  STATUS host_msg_proto_module_register(void);
IMPORT  ssize  ipc_msg_receive(void *buf,ssize len);
IMPORT  ssize  ipc_msg_send(ssize len);
IMPORT   STATUS ipc_msg_proto_parser(void);
IMPORT  char *  host_cmd_send_buf_get(UINT16 cmd_len,UINT8 flag,UINT16 type,UINT16 corr_tag);



IMPORT char *  host_cmd_send_buf_get_new(UINT16 cmd_len,UINT8 flag,char *name,UINT16 corr_tag);

IMPORT  STATUS ipc_driver_register(IPC_DRIVER_SESSION_T  *p_ipc_driver_sess);


 
#ifdef __cplusplus
}
#endif


#endif
