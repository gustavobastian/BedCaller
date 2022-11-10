#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

#include <stdint.h>  
#include "driver/gpio.h"


/*TAG*/
extern const char *TAG;

/*Leds*/
#define GPIO_LED_1     27
#define GPIO_LED_2     26



extern int dispositivoId;
extern int Luz1;
extern int Luz2;

extern char nombre[];
extern char ubicacion[];


extern float localTemperature;
extern float localHumidity;





char topicDataSensor[100];
char topicDataDesconexion[100];
char topicParamSensor[100];
char topicCallerEvent[100];



/*
* Funcion para inicializacion<
*/
void initSensor();
void generateJson(char *buffer,int dispositivoId, char *nombre,char *ubicacion,int luz1, int luz2 ,float temp, float humedad);
void processData(char *topic,char *message);
void generateJsonKey(char *buffer,int key);
void initCaller();
#endif