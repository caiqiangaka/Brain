
#ifndef _H_LOGS_H_
#define	_H_LOGS_H_


#include "type.h"


#ifdef __cplusplus
extern "C" {
#endif


#define	USR_LOG_ERR(x) 	USR_ERR(LOGS_MODULE, x)

#define   LOG_MAX_BUF_NUM		512
#define   LOG_PREFUX_BUF_NUM	320
#define   LOG_HEARD_NUM		180
#define   LOG_TRACK_BUF_NUM	4*1024
#define  LOG_TRACE_PORT			38127
#define  LOG_TRACE_MAX_SERVER	4

/*config name */
#define  CONF_USR_LOG_NAME				"usr log"
#define  CONF_TRACE_STATUS_ITME_NAME		"trace status"
#define  CONF_USR_LOG_MODE_ITME_NAME	"usr log mode"
#define  CONF_TRACE_IPADDRESS_ITME_NAME	"trace ip address"

/*usr log mode*/
#define    LOG_CONSOLE_MODE     0x00000001		/*console mode*/
#define    LOG_TRACE_MODE         0x00000002		/*TRACE mode*/
#define    LOG_FILM_MODE           0x00000004		/*film mode*/


/* user log level definition */
enum
{
	LOG_LVL_CRITICAL	= 0,			/* critical event */
	LOG_LVL_MAJOR,					/* important event */
	LOG_LVL_NORMAL,				/* general event */
	LOG_LVL_DETAIL,				/* trivial event*/
	LOG_LVL_DEBUG,					/* debug message,  output in debug mode */
	
	LOG_LVL_NUM
};

/* user log trace status  */
enum
{
	LOG_TRACE_DISABLE	= 0,			/* usr log trace disable */
	LOG_TRACE_ENABLE,				/*  usr log trace enable */
};






typedef     struct  usr_log_trace_info{
         int             status;				/*1 :enable   0: disable*/
	  int		     log_mode;			/*1:console mode 2:udp trace mode */
         int             trace_address[4];		/*trap ip address*/
}USR_LOG_TRACE_INFO_T;
#define  USR_LOG_TRACE_INFO_SIZE   sizeof(USR_LOG_TRACE_INFO_T)



//#ifndef LOSG_MODLUE
//#define  usr_log(level,module,fmt, ...) 
//#else
IMPORT STATUS usr_log(UINT8 level,UINT8 module, char *fmt, ...);
IMPORT STATUS device_card_log(UINT8 slot_id,UINT8 level,UINT8 device_type, char *fmt, ...);
IMPORT STATUS debug_packets(UINT8 level,UINT8 module, const char *name,char *buf, UINT16 len);
IMPORT STATUS ibcp_proto_debug_packets(UINT8 slot_id,UINT8 proto_type,UINT16 cmd_type,const char *name,char *buf, UINT16 len);
//#endif

IMPORT USR_LOG_TRACE_INFO_T *get_usr_log_data(void);

IMPORT STATUS usr_log_module_register(void);
#ifdef __cplusplus
}
#endif

#endif
