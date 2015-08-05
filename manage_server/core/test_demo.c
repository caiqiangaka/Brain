
#include <string.h>

#include "type.h"

#include "host_cmd_if_lib.h"
#include "msg_proto_lib.h"

#include "test_demo.h"

LOCAL STATUS get_test_demo_data( UINT8* pRecv, UINT8* pSend, UINT16 cmd_len )
{
	STATUS status = OK;
	TEST_DEMO_T stInfo;
	
	/* Data IN */
	memset( &stInfo, 0, sizeof( stInfo ) );
	memcpy( &stInfo, pRecv, cmd_len );
	
	printf( "get_test_demo_data : %d\n", stInfo.u32TestVal );
	
	/* Data OUT */
	stInfo.u32Status = 1000;
	memcpy( pSend, &stInfo, sizeof( stInfo ) );
	
	return OK;
}

LOCAL STATUS get_test_demo_host_cmd( HOST_CMD_NEW_T *pCmd, BOOL wait )
{
	UINT32 i = 0;
	char* pbuf = NULL;
	UINT16 cmd_len = 0;
	STATUS status = OK;
	UINT8* pRecv = NULL;
	UINT8* pSend = NULL;
	
	/* get cmd len */
	cmd_len = TEST_DEMO_SIZE;
	
	/* get recv buffer */
	pRecv = TYPE_TO_REV_BUF( pCmd, UINT8 );
	
	/* get send buffer */
	pbuf = host_cmd_send_buf_get_new( cmd_len, HOST_CMD_CLASS_CONFIG_NEW, pCmd->name, pCmd->corrTag );
	pSend = TYPE_TO_SEND_BUF( pbuf, UINT8 );
	
	/* fill data */
	status = get_test_demo_data( pRecv, pSend, cmd_len );
	
	/* send command */
	RESP_STATUS( status );
    ipc_msg_send( BUF_LEN_NEW( cmd_len ) );
	
	return status;
}

LOCAL HOST_CMD_NODE_NEW_T test_demo_host_cmd_arr[] = {
	{ "get_test_demo", get_test_demo_host_cmd, NULL },
};
LOCAL int test_demo_host_cmd_number = sizeof( test_demo_host_cmd_arr ) / sizeof( test_demo_host_cmd_arr[0] );

STATUS test_demo_host_cmd_register( void )
{
	host_cmd_register_arr_new( test_demo_host_cmd_arr, test_demo_host_cmd_number );

	return OK;
}