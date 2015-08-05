
/* 
	open source .............  crc16 
*/




#ifndef  _H_CRC_H_
#define	_H_CRC_H_


#include "type.h"


#ifdef __cplusplus
extern "C" {
#endif


//IMPORT UINT16  crc16(UINT8 * frame, UINT16 len );
IMPORT UINT16 crc16(UINT8* dataIn, int length);
#ifdef __cplusplus
}
#endif

#endif
