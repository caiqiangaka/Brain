
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "logs.h"
#include "usr_module.h"


#include "config_module.h"

#define USR_MODULE_ERR(e)	USR_ERR(MANG_MODULE,e)

LOCAL USR_MODULE_T *usr_module_arr[MODULE_MAX_NUMBER];




STATUS usr_module_register(USR_MODULE_T * pModule)
{
	if(pModule == NULL ||pModule->module_id==0 || pModule->name ==NULL)
		return USR_MODULE_ERR(E_PARAM);

	if(usr_module_arr[pModule->module_id] !=NULL)
		return USR_MODULE_ERR(E_EXIST);
	
	usr_module_arr[pModule->module_id] =  pModule;

	return OK;
}



STATUS usr_module_init(void)
{
	char *file_name;

	
	file_name = get_running_confing_name();
	load_module_init(file_name);

	
	return OK;
}






STATUS save_as_module(char *file_name)
{

	char tmp_dir[256];
	char conf_dir[256];
	char backup_dir[256];
	
	FILE *file;
	CONFIG_DATA_T *root;
	char *out;
	UINT8	module_id;

	STATUS ret = OK;
	
	int 	tmp_fd;



	if(file_name ==NULL)
		return USR_MODULE_ERR(E_PARAM);
	

	/*file name and dir*/
	sprintf(tmp_dir,"%s/%s",CONFI_DIR,"tmp_XXXXXX");
	sprintf(conf_dir,"%s/%s",CONFI_DIR,file_name);
	sprintf(backup_dir,"%s/%s",CONFI_DIR,".backup");


	/*make temp file*/
	tmp_fd = mkstemp (tmp_dir);
	if(tmp_fd <0)
	{
		//printf("mkstemp fd :%d\r\n",tmp_fd);
		return USR_MODULE_ERR(E_FAIL);
	}

	root = create_object();

	for(module_id=NULL_MODULE;module_id<MODULE_NUM;module_id++)
	{
		if(usr_module_arr[module_id] ==NULL ||usr_module_arr[module_id]->name ==NULL \
			||usr_module_arr[module_id]->module_save ==NULL)
		{
			continue;
		}

		//printf("Save %-38s    .........",usr_module_arr[module_id]->name);
		/*init ALL user  init  function*/
		if(usr_module_arr[module_id]->module_save !=NULL)
			ret = (*(usr_module_arr[module_id]->module_save))(root);
		
		//printf("[ %s ] \r\n",ret==OK ? "OK":"fail");
		
	}
	out=json_print(root);
	json_delete(root);
 
	/*write data to film  and close*/
	write (tmp_fd,  out, strlen(out));
	close(tmp_fd);


	/*unlink backup file dir*/
	unlink (backup_dir);

	/*link to config file*/
	link (conf_dir, backup_dir);
	

	/*sync data to device iO*/
	sync ();

	/*unlink config  file dir*/
	unlink (conf_dir);
	
	/*link to config file*/
	if (link (tmp_dir, conf_dir) != 0)
	{
		/*if link new file fail backup  old film*/
		link (backup_dir, tmp_dir);
		free(out);	
	       unlink (tmp_dir);
	       return USR_MODULE_ERR(E_FAIL);
	}

	/*unlink config  file dir*/
  	if (unlink (tmp_dir) != 0)
  	{
  		sync ();
  		free(out);
  		return USR_MODULE_ERR(E_NORSRC);
  	}

	/*sync data to device iO*/
	sync ();


	/*change file authority*/
	chmod(conf_dir,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	
	//file_name = get_running_confing_name();	
	//rename(tmp_dir,conf_dir);
	
	free(out);
	
	return ret;	
}

STATUS save_module(void)
{
	char *file_name;

	
	file_name = get_running_confing_name();
	save_as_module(file_name);


	return OK;
	
}


STATUS load_module_init(char *file_name)
{
	char tmp_dir[256];

	FILE *file;
	CONFIG_DATA_T *root;
	long len;
	char *data;
	UINT8	module_id;
	int  	flag=0;

	STATUS ret_all,ret = OK;

	printf(".........................start user module .....................\r\n\r\n");



	sprintf(tmp_dir,"%s/%s",CONFI_DIR,file_name);



	if((file=fopen(tmp_dir,"rb"))==NULL)
	{
		root =NULL;
		flag = 1;
	}
	else
	{
	
		fseek(file,0,SEEK_END);
		len=ftell(file);
		fseek(file,0,SEEK_SET);
		data=(char*)malloc(len+1);
		fread(data,1,len,file);
		fclose(file);
		flag = 0;

		root = json_parse(data);
	}

	
	for(module_id=NULL_MODULE;module_id<MODULE_NUM;module_id++)
	{
		if(usr_module_arr[module_id] ==NULL ||usr_module_arr[module_id]->name ==NULL \
			||usr_module_arr[module_id]->module_init ==NULL)
		{
			continue;
		}

		printf("Init %-38s    .........",usr_module_arr[module_id]->name);
		/*init ALL user  init  function*/
		if(usr_module_arr[module_id]->module_init !=NULL)
			ret = (*(usr_module_arr[module_id]->module_init))(root);

		printf("[ %s ] \r\n",ret==OK ? "OK":"fail");

		/*if load config error  default it*/
		if(ret !=OK && usr_module_arr[module_id]->module_default !=NULL)
			ret |=(*(usr_module_arr[module_id]->module_default))();

		ret_all |=ret;
		
		
	}

	
	json_delete(root);

	if(flag==0)
		free(data);

	//if(ret_all !=OK)
	//	save_module();
	
	return ret_all;
}

STATUS reset_module(void)
{
	default_module();
	save_module();

	return OK;
}

STATUS default_module(void)
{

	UINT8	module_id;


	STATUS ret = OK;



	for(module_id=NULL_MODULE;module_id<MODULE_NUM;module_id++)
	{
		if(usr_module_arr[module_id] ==NULL ||usr_module_arr[module_id]->name ==NULL \
			||usr_module_arr[module_id]->module_default ==NULL)
		{
			continue;
		}

		//printf("Default %-38s    .........",usr_module_arr[module_id]->name);
		/*init ALL user  init  function*/
		if(usr_module_arr[module_id]->module_default !=NULL)
			ret = (*(usr_module_arr[module_id]->module_default))();
		
		//printf("[ %s ] \r\n",ret==OK ? "OK":"fail");
		
	}

	//save_module();
		
	return ret;
}

const char * get_usr_module_name(UINT8 module_id)
{
	if(module_id<NULL_MODULE ||module_id>MODULE_NUM 
		|| usr_module_arr[module_id] == NULL)
		return NULL;
	
	return usr_module_arr[module_id]->name;
}

STATUS usr_module_set(UINT8 module_id,UINT8 log_level)
{
	if(usr_module_arr[module_id] == NULL ||  log_level >=LOG_LVL_NUM)
		return USR_MODULE_ERR(E_PARAM);

	usr_module_arr[module_id]->logs_level_flag=log_level;
	return OK;
}

UINT8  usr_module_get(UINT8 module_id)
{
	if(usr_module_arr[module_id] != NULL && module_id <MODULE_NUM )
		return usr_module_arr[module_id]->logs_level_flag; 
	
	return ERROR;
}


STATUS  usr_module_dump(UINT8 module_id)
{

	if(module_id<NULL_MODULE ||module_id>MODULE_NUM)
		return USR_MODULE_ERR(E_PARAM);
		
	if(usr_module_arr[module_id] != NULL && usr_module_arr[module_id]->module_dump != NULL)
	{
		(*usr_module_arr[module_id]->module_dump)();
	}
	
	return OK;
}

