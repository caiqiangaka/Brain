
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "usr_module.h"
#include "msg_proto_lib.h"

#include "logs.h"

/*trace network  data*/
LOCAL int  log_trace_fd;
LOCAL struct sockaddr_in log_trace_addr;

LOCAL USR_LOG_TRACE_INFO_T usr_log_trace_infor;

USR_LOG_TRACE_INFO_T *get_usr_log_data(void)
{
	return &usr_log_trace_infor;
}

LOCAL void log_send(char *buf,INT32 len,UINT32 ipaddress)
{
	log_trace_fd = socket(AF_INET,SOCK_DGRAM,0);
		
	if (log_trace_fd < 0)
	{
		perror("can't creat socket\n");
		return;
	}

	log_trace_addr.sin_family = AF_INET;
	log_trace_addr.sin_addr.s_addr = ipaddress;
	log_trace_addr.sin_port = htons(LOG_TRACE_PORT);

	if (sendto(log_trace_fd,buf,len,0,(struct sockaddr*)&log_trace_addr,sizeof(log_trace_addr)) < 0)
	{
		perror("can't sendto\n");	 
	}

	close(log_trace_fd);
}

LOCAL STATUS log_trace(char *buf,INT32 len)
{
	int i;
	
	if(buf == NULL)
		return USR_LOG_ERR(E_PARAM);

	if(usr_log_trace_infor.status == LOG_TRACE_ENABLE)
	{
		for(i=0;i<LOG_TRACE_MAX_SERVER;i++)
		{
			if(usr_log_trace_infor.trace_address[i] != 0)
				log_send(buf,len,usr_log_trace_infor.trace_address[i] );
		}
	}

	return OK;
}

LOCAL  STATUS  usr_log_proc(char *buf,INT32 len)
{
	if(usr_log_trace_infor.status == LOG_TRACE_DISABLE )
		return OK;
	
	if(usr_log_trace_infor.log_mode&LOG_CONSOLE_MODE)
		printf("%s",buf);

	if(usr_log_trace_infor.log_mode&LOG_TRACE_MODE)
		log_trace(buf,len);

	return OK;
}

STATUS usr_log(UINT8 level,UINT8 module, char *fmt, ...)
{
	va_list args;
	char    prefix[LOG_PREFUX_BUF_NUM];
	char    head_buf[LOG_HEARD_NUM];
	char    log_buf[LOG_MAX_BUF_NUM];
	UINT8   log_level;


	log_level =  usr_module_get(module);

	if(level >= LOG_LVL_NUM ||log_level >   LOG_LVL_NUM)
		return E_PARAM;

	if(level > log_level )
		return OK;

	va_start(args, fmt);
	vsprintf(prefix,fmt,args);
	va_end(args);

	sprintf(head_buf,"[ID:%d level:%d name:%s  ]",module,level,get_usr_module_name(module));
	sprintf(log_buf,"%s   %s\r\n",head_buf,prefix);

	/*usr log proc*/
	usr_log_proc(log_buf,strlen(log_buf));

	return OK;
}

STATUS debug_packets(UINT8 level,UINT8 module, const char *name,char *buf, UINT16 len)
{
	char log_buf[LOG_TRACK_BUF_NUM];
	UINT8 log_level;
	int i;
	char *buf_point;

	log_level =  usr_module_get(module);

	if(level >= LOG_LVL_NUM ||log_level >   LOG_LVL_NUM)
		return E_PARAM;

	if(level > log_level )
		return OK;

	if(len >IPC_PROTO_BUF_LARGE)
		return E_PARAM;

	buf_point = log_buf;
	sprintf(log_buf,"[ID:%d level:%d name:%s ] %s  len:%d ",module,level,get_usr_module_name(module),name,len);
	buf_point =buf_point + strlen(log_buf);

	for(i=0;i<len;i++)
	{
		sprintf(buf_point,"%02X-",buf[i]);
		buf_point=buf_point+3;	
	}
	sprintf(buf_point,"\r\n");

	/*usr log proc*/
	usr_log_proc(log_buf,strlen(log_buf));
	
    return OK;
}

LOCAL STATUS usr_log_module_init(CONFIG_DATA_T *root)
{
	CONFIG_DATA_T *pObject,*pitem,*pArray_value;
	int log_trace_ip_number;
	int i;
	STATUS ret = OK;

	if(root == NULL)
		return USR_LOG_ERR(E_PARAM);

	pObject = get_object_item(root,CONF_USR_LOG_NAME);
	if(pObject)
	{
		/*init usr log  trace status*/
		pitem=get_object_item(pObject,CONF_TRACE_STATUS_ITME_NAME);
		if(pitem)
		{
			if(ITEM_INT_VALUE(pitem))
				usr_log_trace_infor.status = LOG_TRACE_ENABLE;
			else
				usr_log_trace_infor.status = LOG_TRACE_DISABLE;
		}
		else
			return USR_LOG_ERR(E_PARAM);

		/*init usr log  mode*/
		pitem=get_object_item(pObject,CONF_USR_LOG_MODE_ITME_NAME);
		if(pitem)
		{
			usr_log_trace_infor.log_mode = ITEM_INT_VALUE(pitem);
		}
		else
			return USR_LOG_ERR(E_PARAM);
		
		/*init usr log trace ip address */
		pitem=get_object_item(pObject,CONF_TRACE_IPADDRESS_ITME_NAME);
		if(pitem)
		{
			log_trace_ip_number = get_array_size(pitem);

			for(i=0;i<log_trace_ip_number;i++)
			{
				pArray_value = get_array_item(pitem,i);
				usr_log_trace_infor.trace_address[i] = ITEM_INT_VALUE(pArray_value);
			}
		}
		else
			return USR_LOG_ERR(E_PARAM);
	}
	else
		return USR_LOG_ERR(E_PARAM);

	return OK;
}

LOCAL STATUS usr_log_module_default(void)
{
	int i;
	
	usr_log_trace_infor.status = LOG_TRACE_ENABLE;
	usr_log_trace_infor.log_mode = LOG_CONSOLE_MODE;

	for(i=0;i<LOG_TRACE_MAX_SERVER;i++)
	{
		usr_log_trace_infor.trace_address[i] = 0;
	}

	return OK;
}

LOCAL STATUS usr_log_module_save(CONFIG_DATA_T *root)
{
	CONFIG_DATA_T *pObject,*pitem;
	int i;

	/*create usr log item*/
	add_item_to_object(root, CONF_USR_LOG_NAME, pObject=create_object());

	/*save usr log  trace status*/
	add_number_to_object(pObject,CONF_TRACE_STATUS_ITME_NAME,usr_log_trace_infor.status);
	/*save usr log  mode*/
	add_number_to_object(pObject,CONF_USR_LOG_MODE_ITME_NAME,usr_log_trace_infor.log_mode);

	/*save usr log trace ip address*/
	add_item_to_object(pObject,CONF_TRACE_IPADDRESS_ITME_NAME,create_int_array(usr_log_trace_infor.trace_address,LOG_TRACE_MAX_SERVER));
	
	return OK;
}

LOCAL USR_MODULE_T  usr_log_module=
{
	LOGS_MODULE,
	"usr logs  module",
	LOG_LVL_NORMAL,
	usr_log_module_init,
	NULL,
	usr_log_module_save,
	usr_log_module_default
};


STATUS usr_log_module_register(void)
{
	STATUS ret;
	
	ret = usr_module_register(&usr_log_module);

	if(ret != OK)
		return USR_LOG_ERR(E_REG);
	return OK;
		
}


