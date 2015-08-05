
#ifndef _H_POLL_COMMAND_H_
#define	_H_POLL_COMMAND_H_

#include "type.h"


#ifdef __cplusplus
extern "C" {
#endif


#define PORT_INFO_BUNDLE  255

#define	POLL_COMMAND_ERR(x)	USR_ERR(POLL_CMD_MODUlE, x)

typedef enum
{
	GET	= 0x00,		/*get  operation*/
	SET	= 0x40		/*set operation*/
}POLL_TYPE_T;


typedef enum
{
	OSL2060	= 1,			/*OSL 2060   device*/
	OLP			= 2,			/*OLP  device*/
}POLL_DEVICE_TYPE_T;



/*OSL2060    device  command type*/
typedef enum
{
POLL_RESERVER		= 0x00,
	POLL_IIC_OPER  		= 1,
	POLL_DELAYLINE_INFO =7,
	POLL_PORT_INFOR 	= 8,
	ACCESS_APS_INFO		= 9,
	POLL_SYS_INFO 		= 10,
	ACCESS_EDFA_INFO	= 11,
	ACCESS_CPUADC_INFO	= 12,
	ACCESS_DDCM_SWITCH	= 13,
	ACCESS_OLPSFP_INFO	= 14,
	ACCESS_OLPPORT_INFO	= 15,
	PHD_VOL_VALUE		= 16,
	ACCESS_FIELDINIT_INFO = 17,
	POLL_SYSSTATUS_INFO = 18,

}POLL_CMD_OSL2060_TYPE_T;



typedef struct   osl2060_set_test
{
	UINT8	parm1;
	UINT8	parm2;
	UINT8	parm3;
}OSL2060_SET_TEST_T;



typedef struct poll_osl2060_port_infor
{
	UINT8	port_num;
	UINT8	method;
}POLL_OSL2060_PORT_INFOR_T;

#define  POLL_OSL2060_PORT_INFOR_SIZE   sizeof(POLL_OSL2060_PORT_INFOR_T)





typedef enum em_module_type
{
	MODULE_TYPE_SFP=0,
	MODULE_TYPE_SFPPLUS=1
} module_type;

typedef enum em_wave_type
{
	MODULE_WAVE_NO_WDM=0,
	MODULE_WAVE_CWDM,
	MODULE_WAVE_DWDM,
}wave_type;





IMPORT STATUS poll_command_module_register(void);


#ifdef __cplusplus
}
#endif

#endif
