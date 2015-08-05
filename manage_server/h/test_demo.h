
#ifndef _TEST_DEMO_H_
#define _TEST_DEMO_H_

typedef struct
{
	UINT32 u32TestVal;
	UINT32 u32Status;
}__attribute__ ((packed)) TEST_DEMO_T;

#define TEST_DEMO_SIZE sizeof(TEST_DEMO_T)

IMPORT STATUS test_demo_host_cmd_register( void );

#endif /* _TEST_DEMO_H_ */
