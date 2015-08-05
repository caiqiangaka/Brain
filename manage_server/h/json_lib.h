
#ifndef _H_JSON_LIB_H_
#define	_H_JSON_LIB_H_



#define _CJSON_LIB_ 1

//defined(_CJSON_LIB_)


#ifdef  _MY_JSON_LIB_

	const char *get_error_ptr(void);

	/*json  operation interface*/
	void 	json_delete(CONFIG_DATA_T *c);
	CONFIG_DATA_T 	*create_array(void);
	CONFIG_DATA_T 	*create_object(void);
	char 	*json_print(CONFIG_DATA_T *item);	
	CONFIG_DATA_T 	*json_parse(const char *value);


	/*JSON VALUE*/
	#define ITEM_INT_VALUE(item)					item->valueint
	#define ITEM_DOUBLE_VALUE(item)				item->valuedouble
	#define ITEM_STRING_VALUE(item)				item->valuestring


	/*get json operation interface*/
	int    	get_array_size(CONFIG_DATA_T *array);
	CONFIG_DATA_T 	*get_object_item(CONFIG_DATA_T *object,const char *string);
	CONFIG_DATA_T 	*get_array_item(CONFIG_DATA_T *array,int item);
					


	/*add json operation interface*/	
	void   	 add_item_to_object(CONFIG_DATA_T *object,const char *string,CONFIG_DATA_T *item);
	void		 add_null_to_object(CONFIG_DATA_T *object,const char *string);
	void		 add_true_to_object(CONFIG_DATA_T *object,const char *string);
	void		 add_false_to_object(CONFIG_DATA_T *object,const char *string);
	void		 add_bool_to_object(CONFIG_DATA_T *object,const char *string,int b);
	void		 add_number_to_object(CONFIG_DATA_T *object,const char *string,double b);
	void		 add_string_to_object(CONFIG_DATA_T *object,const char *string,char *name);
	void		 add_item_to_array(CONFIG_DATA_T *object,CONFIG_DATA_T *item);



#else 

	#include "cJSON.h"

	#define CONFIG_DATA_T	struct cJSON



	#define get_error_ptr() 			cJSON_GetErrorPtr()

	/*json file operation interface*/
	#define create_array() 		cJSON_CreateArray()
	#define create_object() 		cJSON_CreateObject()
	#define json_print(item) 		cJSON_Print(item)	
	#define json_delete(item)		cJSON_Delete(item)
	#define json_parse(value)		cJSON_Parse(value)


	/*CONFIG_DATA_T VALUE*/
	#define ITEM_INT_VALUE(item)					item->valueint
	#define ITEM_DOUBLE_VALUE(item)				item->valuedouble
	#define ITEM_STRING_VALUE(item)				item->valuestring


	/*get json operation interface*/
	#define get_object_item(object,item) 				cJSON_GetObjectItem(object,item)
	#define get_array_size(array)						cJSON_GetArraySize(array)
	#define get_array_item(array,index) 				cJSON_GetArrayItem(array,index)	
				


	/*add json operation interface*/	
	#define add_item_to_object(object,name,item) 		cJSON_AddItemToObject(object,name,item)
	#define add_null_to_object(object,name) 			cJSON_AddNullToObject(object,name)
	#define add_true_to_object(object,name) 			cJSON_AddTrueToObject(object,name)
	#define add_false_to_object(object,name) 			cJSON_AddFalseToObject(object,name)
	#define add_bool_to_object(object,name,b) 		cJSON_AddBoolToObject(object,name,b)
	#define add_number_to_object(object,name,n)		cJSON_AddNumberToObject(object,name,n)
	#define add_string_to_object(object,name,s)		cJSON_AddStringToObject(object,name,s)


	#define add_item_to_array(array,item) 			cJSON_AddItemToArray(array,item)

	#define create_int_array(unmber,count)			cJSON_CreateIntArray(unmber,count)
	#define create_string_array(string,count)			cJSON_CreateStringArray(string,count)
	


#endif

#endif

