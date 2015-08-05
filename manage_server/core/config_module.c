
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "usr_module.h"
#include "config_module.h"

LOCAL char config_name[128];



LOCAL STATUS set_config_name(char *file_name)
{
	char tmp_dir[256];
	
	FILE *file;
	CONFIG_DATA_T *root,*item;
	long len;
	char *data;
	char *out;

	sprintf(tmp_dir,"%s/%s",CONFI_DIR,START_CONFIG_FILE);
	
	file=fopen(tmp_dir,"wb+");
	root = create_object();

	add_string_to_object(root,"config name",file_name);


	out=json_print(root);
	json_delete(root);



	fwrite(out, strlen(out), 1, file);
		
	fclose(file);	
		
	return OK;
}

LOCAL STATUS default_config_name(void)
{
	STATUS ret = OK;
	
	ret = set_config_name(DEFAULT_CONFIG_FILE);
	if(ret ==OK)
	{
		strncpy(config_name,DEFAULT_CONFIG_FILE,128);
	}
	
	return ret;
}



LOCAL STATUS  load_config_name(void)
{
	char tmp_dir[256];
	
	FILE *file;
	CONFIG_DATA_T *root,*item;
	long len;
	char *data;
	char *out;

	sprintf(tmp_dir,"%s/%s",CONFI_DIR,START_CONFIG_FILE);

	if((file=fopen(tmp_dir,"rb"))==NULL)
	{
		default_config_name();
		
	}
	else
	{
		fseek(file,0,SEEK_END);
		
		len=ftell(file);
		fseek(file,0,SEEK_SET);
		
		data=(char*)malloc(len+1);
		fread(data,1,len,file);
		fclose(file);

		root = json_parse(data);
		
		/*parse error default config_name file*/
		if(root !=NULL)
		{
			/*config name object Does not exist default config name*/
			item =  get_object_item(root,START_CONFIG_NAME);
			if(item ==NULL)
				default_config_name();
			else
			{
				strncpy(config_name,ITEM_STRING_VALUE(item),128);
				if(strncmp(config_name,"",128)==0)
					default_config_name();
			}
		}
		else
		{
			
			default_config_name();
		}
		json_delete(root);
		free(data);
	}
	
	return OK;
}


STATUS remove_config_file_name(char *file_name)
{
	int  status;

	char tmp_dir[256];
	
	if(strncmp(file_name,config_name,128)==0)
		default_config_name();

	sprintf(tmp_dir,"%s/%s",CONFI_DIR,file_name);
	
	status = remove(tmp_dir);
	
	if(status !=0)
		return E_NAME;
	return OK;
}


char * get_running_confing_name(void)
{
		STATUS ret = OK;

		
		ret = load_config_name();
		if(ret ==OK)
			return config_name;
		else
			return NULL;
}



LOCAL STATUS check_film_name(char *open_dir,char *file_name )
{
	char cwd[CONF_DIR_MAX_NUMBER];
    	char current_dir[CONF_DIR_MAX_NUMBER];
		
	DIR    *dir;
    	struct    dirent    *ptr;
    	struct stat info;
		
	int heart_flag;
	

	/*check param */
	if(open_dir ==NULL || file_name ==NULL)
		return E_PARAM;

	/*get dir and change dir*/
	getcwd(current_dir, CONF_DIR_MAX_NUMBER);
	strncpy(cwd,open_dir,CONF_DIR_MAX_NUMBER);
	chdir(cwd);
	

	/*open file and ckeck film*/
	dir = opendir(cwd);
	heart_flag = 0;
	
	while((ptr = readdir(dir)) != NULL)
	{
	       stat(ptr->d_name,&info);

		if(S_ISREG (info.st_mode))
		{
			 if(strncmp(ptr->d_name,".",1) !=0  &&  strncmp(ptr->d_name,START_CONFIG_FILE,1) !=0 )
				if(strncmp(ptr->d_name,file_name,strlen(ptr->d_name)) ==0)
				{
					heart_flag =1;
					break;
			 	}				
			
		}
	}
	closedir(dir);
	chdir(current_dir);

	if(heart_flag ==0)
		return  E_NAME;

	
	return OK;
}

STATUS set_running_confing_name(char *file_name)
{
	
	STATUS ret = OK;


	ret = check_film_name(CONFI_DIR,file_name);
	if(ret !=OK)
		return E_NAME;

	
	ret = set_config_name(file_name);
	if(ret ==OK)
	{
		strncpy(config_name,file_name,CONF_DIR_MAX_NUMBER);
	}
	
	return ret;
}

