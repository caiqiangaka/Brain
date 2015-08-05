
#ifndef  _H_CONSOLE_H_
#define	_H_CONSOLE_H_



#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include  "type.h"

#ifdef __cplusplus
extern "C" {
#endif



#define MAX_TEXT 512
#define RCV_BUF_SIZE 256
#define TAR_PORT_DATA 256

#define MAX_DIVS_DATA_LEN TAR_PORT_DATA + sizeof(char)*3 + sizeof(int)*1
#define MSG_FROM_DEVICE 1
#define IF_PKT_FIXED_HEADER_LENGTH 	5


typedef struct port_info{
	int baud_rate;
	int port_fd;
	char parity;
	char stop_bit;
	char flow_ctrl;
	char data_bits;
}*pport_info;

/* message data struct  */
typedef struct{
	char rw_cmdtype;
	char cmd_num;
	char len_hight;
	char len_low;
	char data[MAX_DIVS_DATA_LEN];
	char chk_sum;
}s_cmd_info;




IMPORT STATUS poll_dirver_module_register(void);


		IMPORT STATUS  console_open();
		IMPORT STATUS close_port();
		IMPORT ssize  send_data(void *data, int data_len);
		IMPORT ssize  receive_data(void *data, int data_len);

#ifdef __cplusplus
}
#endif

#endif
