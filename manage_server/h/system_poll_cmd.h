
#ifndef _SYSTEM_POLLCMD_H_
#define	_SYSTEM_POLLCMD_H_


#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SYS_COMPONENT_DEV_NAME = 0,
	SYS_COMPONENT_IPADDR,
	SYS_COMPONENT_REBOOT,
	SYS_NETGUI_TCPPORT_NUM,
	SYS_SAVE_CURRENT,
	SYS_RESET_TO_FACTORY,  // 5
	SYS_WORKING_MODE,
	SYS_VERSION,
	SYS_PN,
	SYS_SN,
	SYS_MANUFACTURE_INFO, // 10
	SYS_PN_NEW,
	SYS_SN_NEW,
	SYS_VENDOR_INFO,
	SYS_MODEL_NAME,
	SYS_PCBA_SN
}SYSTEM_FUNC_CMD_E;

IMPORT STATUS get_sysversion_version(void);
IMPORT STATUS get_sys_pn(void);
IMPORT STATUS get_sys_sn(void);
IMPORT STATUS get_manufacture_info(void);
IMPORT STATUS set_component_reboot(void);
IMPORT STATUS set_save_currentset(void);
IMPORT STATUS set_reset2factory(void);
IMPORT STATUS get_sys_version(void);

#ifdef __cplusplus
}
#endif


#endif //_SYSTEM_POLLCMD_H_

