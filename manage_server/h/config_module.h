
#ifndef 	_H_CONFIG_MODULE_H_
#define	_H_CONFIG_MODULE_H_


#include "type.h"


#ifdef __cplusplus
extern "C" {
#endif


#define CONF_DIR_MAX_NUMBER		128
#define CONFI_DIR               "/conf"
#define START_CONFIG_NAME		"config name"
#define START_CONFIG_FILE		".start_film"
#define DEFAULT_CONFIG_FILE		"config"



IMPORT 	char * get_running_confing_name(void);
IMPORT 	STATUS set_running_confing_name(char *file_name);

IMPORT  STATUS remove_config_file_name(char *file_name);


#ifdef __cplusplus
}
#endif

#endif
