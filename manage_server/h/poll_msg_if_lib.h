
#ifndef _POLL_MSG_IF_LIB_H_
#define _POLL_MSG_IF_LIB_H_

#include "type.h"



#ifdef __cplusplus
extern "C" {
#endif


#define POLL_MAX_BUF	300


enum
{
	POLL_ENABLE = 0,          /* All enable */
	POLL_DISABLE,             /* All disable */
	POLL_DYNAMIC_ENABLE,      /* Dynamic poll enable */
	POLL_DYNAMIC_DISABLE,     /* Dynamic poll disable */
	POLL_SEND_ENABLE,         /* set/get channel enable */
	POLL_SEND_DISABLE,        /* set/get channel disable */
	POLL_ENGINE_OPER_MAX_NUM
};



/*poll    error  status  Statistics*/
typedef struct poll_error_statistics_t
{
	UINT32	rx_packets;			/*The number of packets received*/				
	UINT32	tx_packets;			/*The number of packets sent */
	UINT32	rx_error_packet;	/*received error packet*/	
	UINT32   tx_error_packet;		/*sent error packet*/
	UINT32	rx_continuous_error_packet;	/*recived contunie error packets*/
	UINT32	tx_continuous_error_packet;	/*sent  Continuous error packets*/
}POLL_ERROR_STATISTICS_T;

#define  POLL_ERROR_STATISTICS_SIZE   sizeof(POLL_ERROR_STATISTICS_T)




/*Communication channel number for polling*/
enum
{
	POLL_COMM_CHANNEL_COM0 = 0,
	POLL_COMM_CHANNEL_COM1 ,
	POLL_COMM_CHANNEL_COM2,
	POLL_COMM_CHANNEL_COM3,
};

IMPORT UINT32 gConsoleChannelNum;

/*poll    proto    header*/
typedef struct poll_msg_header_t
{
	UINT16	magic;				/*magic   value*/				
	UINT8	device_cmd;			/*read or write   and   device type */
	UINT8	type;				/*command type*/	
	UINT16   len;					/*package   length*/
}POLL_MSG_HEADER_T;

#define  POLL_MSG_HEADER_SIZE   sizeof(POLL_MSG_HEADER_T)





#define POLL_TYPE_TO_TYPE(point,type1)   ((type1 *)(point))
#define POLL_BUF_TO_FRM_HEAD(buf)      	 ((POLL_MSG_HEADER_T *)(buf))


#if 0
typedef struct poll_type_2060buf_len_t
{
	UINT8	cmd_type;				/*2060 command type*/
	UINT16   buf_len;					/*command   buffer  length*/
}POLL_TYPE_2060BUF_LEN_T;
#endif



/*  register Command Message type */
enum
{
	POLL_TYPE_STATIC_MSG	= 0,	/* poll Static message   */
	POLL_TYPE_DYNAMIC_MSG,			/* poll Dynamic Message */
	POLL_TYPE_NUM,					/* number of defined type */
};


typedef STATUS (*POLL_FUNC_T)();

typedef struct poll_func_Node_t
{
	char 	name[50];				/*poll  function name*/
	POLL_FUNC_T	func;				/* call back function pointer */
	struct poll_func_Node_t	*next;	/* pointer to next node */
}POLL_FUNC_NODE_T;



typedef STATUS (*POLL_SE_OPEN_T)(void);		/*poll session open function*/
typedef STATUS (*POLL_SE_CLOSE_T)(void);		/*poll session close function*/
typedef ssize  (*POLL_SE_RECV_T)(void *buf,int len);		/*poll session receive function*/
typedef ssize  (*POLL_SE_SEND_T)(void *buf,int len);		/*poll session send function*/

typedef struct poll_session
{
	//char    name[100];	/*device name*/
	
	POLL_SE_OPEN_T	poll_open;			/* poll  open function pointer */
	POLL_SE_CLOSE_T  poll_close;			/* poll  close function pointer */
	POLL_SE_RECV_T	 poll_receive;		/* poll  receive function pointer */
	POLL_SE_SEND_T   poll_send;			/* poll  send function pointer */
}POLL_SESSION_T;


IMPORT POLL_ERROR_STATISTICS_T * get_poll_packets_statistics(void);
IMPORT STATUS poll_hook_add(UINT8 type,POLL_FUNC_T func,const char *name);
IMPORT STATUS poll_hook_run(UINT8 type, BOOL wait);
IMPORT char *  poll_cmd_rec_buf_get();
IMPORT ssize  poll_send(UINT8 device,UINT8 oper_type,UINT8 cmd_type,void *buf,UINT16 cmd_len,UINT16 rev_cmd_len);
IMPORT STATUS poll_engine_oper(UINT8 operation);
IMPORT STATUS  poll_driver_register(
	POLL_SE_OPEN_T 	open,
	POLL_SE_CLOSE_T	close,
	POLL_SE_RECV_T receive,
	POLL_SE_SEND_T send
	);
IMPORT STATUS poll_if_module_register(void);



#ifdef __cplusplus
}
#endif

#endif 
