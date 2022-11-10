 #include "sensor_config.h"
 #include <stdio.h>
 #include <string.h>
 #include "cJSON.h"



/*
Sensor Variables

*/
const char *TAG = "sensor";
float localTemperature;
float localHumidity;

int dispositivoId = 2;
int Luz1=0;
int Luz2=0;

char nombre[25]="IOT_DEVICE";   
char ubicacion[25]="Casa";   

/*
* Sensor parameters init
*/
void initCaller(){
    //generating topics for reporting
    sprintf(topicCallerEvent,"/Beds/Caller-events");
    
}

/*
* Sensor parameters init
*/

void initSensor(){
    //generating topics for reporting
    sprintf(topicDataSensor,"/topic/%d",dispositivoId);
    strcat(topicDataSensor,"/sensor");       
    sprintf(topicParamSensor,"/topic/%d",dispositivoId);  
    strcat(topicParamSensor,"/parametros");  

    sprintf(topicDataDesconexion,"/topic/%d",dispositivoId);  
    strcat(topicDataDesconexion,"/status");  
    
    gpio_set_direction( GPIO_LED_1, GPIO_MODE_OUTPUT );
    gpio_set_direction( GPIO_LED_2, GPIO_MODE_OUTPUT );  

    gpio_set_level( GPIO_LED_1, 0 );
    gpio_set_level( GPIO_LED_2, 0 );



};


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
//printf(string);
sprintf(buffer,string);

//Free the memory
cJSON_Delete(my_json);

}

//processing incoming messages
void processData(char *topic,char *message){
    printf("processing incoming\n");
    printf("%s:\n",message);
    const cJSON *message_json = cJSON_Parse(message);
    cJSON *name= NULL;
    cJSON *place= NULL;

    if(message_json==NULL){printf("Error\n");}
    name= cJSON_GetObjectItemCaseSensitive(message_json, "nombre");
    place = cJSON_GetObjectItemCaseSensitive(message_json, "ubicacion");
   //checkin the message, we can know if it contains leds values or sensor parameters
    if ((cJSON_IsString(name) && (name->valuestring != NULL))&&((cJSON_IsString(place) && (place->valuestring != NULL))))
    {
        //we have a parameter json
        printf("Checking device name \"%s\"\n", name->valuestring);
        snprintf(nombre,24,"%s",name->valuestring);   
        snprintf(ubicacion,24,"%s",place->valuestring);
    }
    else{
        //change light values
        cJSON *led1 = cJSON_GetObjectItemCaseSensitive(message_json, "Led1");
        cJSON *led2 = cJSON_GetObjectItemCaseSensitive(message_json, "Led2");
        if (!cJSON_IsNumber(led1) || !cJSON_IsNumber(led2))
        {printf("Error\n");
        }
        else{
            if((led1->valuedouble>1)||(led2->valuedouble>1)||(led1->valuedouble<0)||(led2->valuedouble<0)){return;}
            Luz1=led1->valuedouble;
            Luz2=led2->valuedouble;
            if(Luz1>0){gpio_set_level( GPIO_LED_1,1);}
            else{gpio_set_level( GPIO_LED_1,0);}            
            if(Luz2>0){gpio_set_level( GPIO_LED_2,1);}
            else{gpio_set_level( GPIO_LED_2,0);}            
        }
    }

    return;
}

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
