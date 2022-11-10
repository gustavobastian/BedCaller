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