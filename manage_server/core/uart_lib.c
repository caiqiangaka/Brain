


#include <stdlib.h>


#include "type.h"
#include "console.h"



#include "uart_lib.h"





STATUS uart_close()
{
	return close_port();
}

STATUS uart_open()
{
	return console_open();
}


ssize uart_send_data(UINT8 IN slotId, void IN *data, UINT32 IN data_len)
{
	return send_data(data,  data_len);
}


ssize uart_receive_data(void OUT *data, UINT32 IN data_len)
{
	return receive_data(data, data_len);
}







