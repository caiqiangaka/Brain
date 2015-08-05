
#ifndef _H_UART_LIB_H_
#define _H_UART_LIB_H_

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif


// return value:
//     OK    - success
//     ERROR - fail
STATUS uart_open();

// return value:
//     >0 - success
//     -1 - fail
ssize uart_send_data(UINT8 IN slotId, void IN *data, UINT32 IN data_len);

// return value:
//     >0 - success
//     -1 - fail
ssize uart_receive_data(void OUT *data, UINT32 IN data_len);

// return value:
//     OK    - success
//     ERROR - fail
STATUS uart_close();



#ifdef __cplusplus
}
#endif

#endif /* _H_UART_LIB_H_ */

