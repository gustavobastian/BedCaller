/* 

	Simple Matrix Keyboard reader

*/

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "keyboard.h"

#include "cJSON.h"

int keyValue=0;

const char *TAG = "keyboard";

int columnNumber=0;
int rowNumber=0;
int keyboard[]=
{
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8
};
int keyStatus[]=
{
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED
};
int prevKeyStatus[]=
{
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED
};	
/*
* Keyboard parameters init
*/

int keyboardInit()
{
	printf("Keyboard configuration init\n");
    //COLUMNS FOR KEYBOARD
    gpio_set_direction( GPIO_COLUMN_1, GPIO_MODE_OUTPUT );
    gpio_set_direction( GPIO_COLUMN_2, GPIO_MODE_OUTPUT );
    gpio_set_direction( GPIO_COLUMN_3, GPIO_MODE_OUTPUT );
    gpio_set_direction( GPIO_COLUMN_4, GPIO_MODE_OUTPUT );
    gpio_set_level( GPIO_COLUMN_1, 0 );
    gpio_set_level( GPIO_COLUMN_2, 0 );
    gpio_set_level( GPIO_COLUMN_3, 0 );
    gpio_set_level( GPIO_COLUMN_4, 0 );

    //ROWS FOR KEYBOARD
    gpio_set_direction( GPIO_ROW_1, GPIO_MODE_INPUT );
    gpio_set_direction( GPIO_ROW_2, GPIO_MODE_INPUT );
    gpio_set_direction( GPIO_ROW_3, GPIO_MODE_INPUT );
    gpio_set_direction( GPIO_ROW_4, GPIO_MODE_INPUT );
	
	printf("Keyboard configuration done\n");
	return 1;
}

int keyboardRefresh( void )
{
	keyValue=0;
	columnNumber++;
	//powering columns
	if(columnNumber==1)
	{		
		gpio_set_level( GPIO_COLUMN_1, 1 );
    	gpio_set_level( GPIO_COLUMN_2, 0 );
    	gpio_set_level( GPIO_COLUMN_3, 0 );
    	gpio_set_level( GPIO_COLUMN_4, 0 );
	}
	else if(columnNumber==2)
	{
		gpio_set_level( GPIO_COLUMN_1, 0 );
    	gpio_set_level( GPIO_COLUMN_2, 1 );
    	gpio_set_level( GPIO_COLUMN_3, 0 );
    	gpio_set_level( GPIO_COLUMN_4, 0 );
	}
	else if(columnNumber==3)
	{
		gpio_set_level( GPIO_COLUMN_1, 0 );
    	gpio_set_level( GPIO_COLUMN_2, 0 );
    	gpio_set_level( GPIO_COLUMN_3, 1 );
    	gpio_set_level( GPIO_COLUMN_4, 0 );
	}
	else if(columnNumber==4)
	{		
		gpio_set_level( GPIO_COLUMN_1, 0 );
    	gpio_set_level( GPIO_COLUMN_2, 0 );
    	gpio_set_level( GPIO_COLUMN_3, 0 );
    	gpio_set_level( GPIO_COLUMN_4, 1 );
	}
	

	
	if(gpio_get_level(GPIO_ROW_2)==1)
	{
		rowNumber=0;	
		printf("key:%x\n",keyboard[columnNumber]);	
		keyValue=keyboard[columnNumber];
	}
	if(gpio_get_level(GPIO_ROW_3)==1)
	{
		rowNumber=1;		
		printf("key:%x\n",keyboard[4+columnNumber]);	
		keyValue=keyboard[4+columnNumber];
	}
	
	if(columnNumber==4)
	{
		columnNumber=0;
	}

	return keyValue;
}

/*
* Keyboard parameters init
*/
void initCaller()
{
    //generating topics for reporting
    sprintf(topicCallerEvent,"/Beds/Caller-events");    
}
/*
* Generating JSON message
*/

void generateJsonKey(char *buffer,int key)
{
	cJSON *my_json;
	cJSON *keyId = NULL;
	char *string = NULL;    
	my_json = cJSON_CreateObject();
	if (my_json == NULL)
	{
			return;
	}
	keyId = cJSON_CreateNumber(key);
	if (keyId == NULL)
	{
			return;
	}    
	//Populate my_json
	//my_json.
	cJSON_AddItemToObject(my_json, "callerId",keyId);
	//Convert my_json to char array, for sending in an API perhaps
	string = cJSON_Print(my_json);
	//printf(string);
	sprintf(buffer,string);
	//Free the memory
	cJSON_Delete(my_json);
}
