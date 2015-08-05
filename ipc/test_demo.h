
#ifndef _TEST_DEMO_H_
#define _TEST_DEMO_H_

typedef struct
{
	UINT32 u32TestVal;
	UINT32 u32Status;
}__attribute__ ((packed)) TEST_DEMO_T;

#define TEST_DEMO_SIZE sizeof(TEST_DEMO_T)

#define TEST_DEMO_CMD_NAME "get_test_demo"
IMPORT STATUS get_test_demo_ipc_lib(TEST_DEMO_T *pInfo);

#endif /* _TEST_DEMO_H_ */
