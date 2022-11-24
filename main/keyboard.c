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
int keyboard[]={0,1,2,3,4,5,6,7,8};
int keyStatus[]={
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED};
int prevKeyStatus[]={
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED,
	RELEASED};	
/*
* Keyboard parameters init
*/

int keyboardInit(){
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

int keyboardRefresh( void ){
	keyValue=0;
	columnNumber++;
	//powering columns
	if(columnNumber==1){
		//printf("Keyboard column 1\n");
		gpio_set_level( GPIO_COLUMN_1, 1 );
    	gpio_set_level( GPIO_COLUMN_2, 0 );
    	gpio_set_level( GPIO_COLUMN_3, 0 );
    	gpio_set_level( GPIO_COLUMN_4, 0 );
	}
	else if(columnNumber==2){
		//printf("Keyboard column 2\n");
		gpio_set_level( GPIO_COLUMN_1, 0 );
    	gpio_set_level( GPIO_COLUMN_2, 1 );
    	gpio_set_level( GPIO_COLUMN_3, 0 );
    	gpio_set_level( GPIO_COLUMN_4, 0 );
	}
	else if(columnNumber==3){
		//printf("Keyboard column 3\n");
		gpio_set_level( GPIO_COLUMN_1, 0 );
    	gpio_set_level( GPIO_COLUMN_2, 0 );
    	gpio_set_level( GPIO_COLUMN_3, 1 );
    	gpio_set_level( GPIO_COLUMN_4, 0 );
	}
	else if(columnNumber==4){
		//printf("Keyboard column 4\n");
		gpio_set_level( GPIO_COLUMN_1, 0 );
    	gpio_set_level( GPIO_COLUMN_2, 0 );
    	gpio_set_level( GPIO_COLUMN_3, 0 );
    	gpio_set_level( GPIO_COLUMN_4, 1 );
		
	}
	//reading rows(ONLY 2 AND 3)
	
	if(gpio_get_level(GPIO_ROW_2)==1){
		rowNumber=0;
		//printf("%d|%d\n",rowNumber,columnNumber);
		printf("key:%x\n",keyboard[columnNumber]);	
		keyValue=keyboard[columnNumber];
		}
	
	if(gpio_get_level(GPIO_ROW_3)==1){
		rowNumber=1;
		//printf("%d|%d\n",rowNumber,columnNumber);
		printf("key:%x\n",keyboard[4+columnNumber]);	
		keyValue=keyboard[4+columnNumber];
	}
	
	if(columnNumber==4){columnNumber=0;}

	/*if(keyStatus[keyValue]==RELEASED){keyStatus[keyValue]=PRESSED;}*/

	/*for(unsigned int i=0;i<9;i++){
		if((keyStatus[i]==PRESSED)&&(keyValue==i)){
			printf("Se soltó:%d\n",keyValue);
		}
	}*/

	return keyValue;
}

/*
* Keyboard parameters init
*/
void initCaller(){
    //generating topics for reporting
    sprintf(topicCallerEvent,"/Beds/Caller-events");    
}


/*
* Generating JSON message
*/

void generateJsonKey(char *buffer,int key){
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


//generating json for sending status data

void generateJson(char *buffer,int dispositivoId, char *nombre,char *ubicacion, int luz1, int luz2, float temp, float humedad){
cJSON *my_json;
cJSON *deviceId = NULL;
cJSON *name = NULL;
cJSON *ubicacionLocal = NULL;
cJSON *TempLocal = NULL;
cJSON *HumLocal = NULL;
cJSON *luz1Local = NULL;
cJSON *luz2Local = NULL;
char *string = NULL;    
my_json = cJSON_CreateObject();
if (my_json == NULL)
    {
        return;
    }
deviceId = cJSON_CreateNumber(dispositivoId);
    if (deviceId == NULL)
    {
        return;
    }    
name = cJSON_CreateString(nombre);
    if (name == NULL)
    {
        return;
    }  
ubicacionLocal = cJSON_CreateString(ubicacion);
    if (ubicacionLocal == NULL)
    {
        return;
    }        


luz1Local = cJSON_CreateNumber(luz1);
luz2Local = cJSON_CreateNumber(luz2);

int tmpLocal= temp/1;
TempLocal = cJSON_CreateNumber(tmpLocal);
int humLocal= humedad/1;
HumLocal = cJSON_CreateNumber(humLocal);
//Populate my_json
//my_json.
cJSON_AddItemToObject(my_json, "dispositivoId", deviceId);
cJSON_AddItemToObject(my_json, "nombre", name);
cJSON_AddItemToObject(my_json, "ubicacion", ubicacionLocal);
cJSON_AddItemToObject(my_json, "luz1", luz1Local);
cJSON_AddItemToObject(my_json, "luz2", luz2Local);
cJSON_AddItemToObject(my_json, "temperatura", TempLocal);
cJSON_AddItemToObject(my_json, "humedad", HumLocal);

//Convert my_json to char array, for sending in an API perhaps
string = cJSON_Print(my_json);
sprintf(buffer,string);

//Free the memory
cJSON_Delete(my_json);

}
