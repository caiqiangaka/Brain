
#include "type.h"
#include "communicate_lib.h"

#include "test_demo.h"

STATUS get_test_demo_ipc_lib(TEST_DEMO_T *pInfo)
{
	return ipc_communicate_with_server( (void*)pInfo, TEST_DEMO_SIZE, TEST_DEMO_CMD_NAME );
}