
#include  "type.h"
#include "poll_msg_if_lib.h"
#include	 "logs.h"
#include "usr_module.h"

#include "console.h"

int port_fd;



#define	POLL_DRIVER_ERR(x)	USR_ERR(POLL_DRIVE_MODUlE, x)





int get_baud_rate(unsigned long int baud_rate)
{
	switch(baud_rate)
	{
		case 9600:
			return B9600;
		case 115200:
			return B115200;
		default:
			return -1;
	}
}

static int set_port(pport_info p_info)
{
	struct termios old_opt, new_opt;
	int baud_rate;

	memset(&old_opt, 0, sizeof(old_opt));
	memset(&new_opt, 0, sizeof(new_opt));

	cfmakeraw(&new_opt);
	tcgetattr(p_info->port_fd, &old_opt);

	baud_rate = get_baud_rate(p_info->baud_rate);

	cfsetispeed(&new_opt, baud_rate);
	cfsetospeed(&new_opt, baud_rate);

	new_opt.c_cflag |= CLOCAL;

	new_opt.c_cflag |= CREAD;

	switch(p_info->flow_ctrl)
	{
		case '0':
			new_opt.c_cflag &= ~CRTSCTS;
		break;

		case '1':
			new_opt.c_cflag |= CRTSCTS;
		break;

		case '2':
			new_opt.c_cflag |= IXON | IXOFF | IXANY;
		break;
	}

	new_opt.c_cflag &= ~CSIZE;

	switch(p_info->data_bits)
	{
		case '5':
			new_opt.c_cflag |= CS5;
		break;

		case '6':
			new_opt.c_cflag |= CS6;
		break;

		case '7':
			new_opt.c_cflag |= CS7;
		break;

		case '8':
			new_opt.c_cflag |= CS8;
		break;

		default:
			new_opt.c_cflag |= CS8;
	}

	switch(p_info->parity)
	{
		case '0':
			new_opt.c_cflag &= ~PARENB;
		break;

		case '1':
			new_opt.c_cflag |= PARENB;
			new_opt.c_cflag &= ~PARODD;
		break;

		case '2':
			new_opt.c_cflag |= PARENB;
			new_opt.c_cflag |= PARODD;
		break;
	}

	if(p_info->stop_bit == '2')
	{
		new_opt.c_cflag |= CSTOPB;
	}
	else
	{
		new_opt.c_cflag &= ~CSTOPB;
	}

	new_opt.c_oflag &= ~OPOST;

	new_opt.c_cc[VMIN] = 1;
	new_opt.c_cc[VTIME] = 1;

	tcflush(p_info->port_fd, TCIFLUSH);

	int result;
	result = tcsetattr(p_info->port_fd, TCSANOW, &new_opt);

	if(result == -1)
	{
		perror("Cannot set the serial port parameters! \n");
		return -1;
	}

	tcgetattr(p_info->port_fd, &old_opt);
	return result;
}


//LOCAL STATUS close_port()
STATUS close_port()
{
	int fd;
	fd = port_fd;
	printf("close port .. \n");
	close(fd);
}
LOCAL int open_port(char* port)
{
	int fd;
	if((fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK)) == -1)
	{
		perror("Cannot open the desired port! \n");
		return -1;
	}
	return fd;
}
//LOCAL STATUS  console_open()
STATUS  console_open()
{

	//printf("console_open \n");
	char ttyName[16] = {0};
	switch(gConsoleChannelNum)
	{
		case POLL_COMM_CHANNEL_COM0:
			strncpy(ttyName,"/dev/ttyS1",16);
			break;
		case POLL_COMM_CHANNEL_COM1:
			strncpy(ttyName,"/dev/ttyS2",16);
			break;
		case POLL_COMM_CHANNEL_COM2:
			strncpy(ttyName,"/dev/ttyS3",16);
			break;
		case POLL_COMM_CHANNEL_COM3:
			strncpy(ttyName,"/dev/ttyS4",16);
			break;
	}
	port_fd = open_port(ttyName);
	//port_fd = open_port("/dev/ttyS2");
	
	if(port_fd == -1)
	{
		printf("Exit\n");
		return -1;
	}

	struct port_info info;
	info.baud_rate = 115200;
	info.data_bits = 8;
	info.flow_ctrl = 2;
	info.port_fd = port_fd;
	info.stop_bit = 1;
	info.parity = 0;

	if(set_port(&info) == -1)
	{
		printf("set uart port err Program Exit \n");
		return -1;
	}

	return OK;
}


//LOCAL  ssize  send_data(void *data, int data_len)
ssize  send_data(void *data, int data_len)	
{
	int len = 0;
	int fd;

	fd =port_fd;

	tcflush(fd, TCIOFLUSH);

	len = write(fd, data, data_len);

	if(len == data_len)
	{
		return len;
	}
	else
	{
		tcflush(fd, TCOFLUSH);
		return -1;
	}
}


//LOCAL  ssize  receive_data(void *data, int data_len)
ssize  receive_data(void *data, int data_len)
{
	int len=0;
	int fs_sel;
	fd_set fs_read;

        int read_len=0;
	int fd;


	fd =port_fd;
	
	struct timeval time;


	FD_ZERO(&fs_read);
	FD_SET(fd, &fs_read);

	time.tv_sec = 1;
	time.tv_usec = 0;
console_read_data:
	fs_sel = select(fd+1, &fs_read, NULL, NULL, &time);

	if(fs_sel)
	{
	               
		read_len =read(fd, data, data_len);
		data = data+ read_len;
		len =len+ read_len;
		if(len < data_len)
			goto console_read_data;
		/*clear console buffer*/
	        //tcflush(fd, TCOFLUSH);

		//if(len == data_len)
			return len;
		//else
		//	return -1;
	}
	else
	{
		return -1;
	}

}

LOCAL STATUS console_drive_init(CONFIG_DATA_T *root)
{
	STATUS ret = OK;
	
	/*register console drive for poll module*/
	poll_driver_register(
		console_open,
		close_port,
		receive_data,
		send_data
		);
	/*OPEN CONSOLE*/
	//ret = console_open();

	return ret;
}

LOCAL USR_MODULE_T  poll_diver_module=
{
	POLL_DRIVE_MODUlE,
	"poll  dirver  module",
	LOG_LVL_NORMAL,
	console_drive_init,
};

STATUS poll_dirver_module_register(void)
{
	STATUS ret;

	
	ret = usr_module_register(&poll_diver_module);

	if(ret != OK)
		return POLL_DRIVER_ERR(E_REG);
	return OK;
		
}

