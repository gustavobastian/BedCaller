#ifndef MQTT_FUNCTIONS_H_  
#define MQTT_FUNCTIONS_H_

#include "mqtt_client.h"
#include "esp_log.h"
#include "keyboard.h"


// Set your local broker URI
#include "passwords_file.h"


#define MOSQUITO_USER_NAME            "Sensor2"
#define MOSQUITO_USER_PASSWORD        "Sensor2Pass"
esp_mqtt_client_handle_t client;

extern const char *TAG;

char buff[800]; // buffer for dataSend
char recBuff[800]; // buffer for process received Data
char recTopic[100]; // buffer for process received topic
extern int flagProcessMessage; // flag for proccessing received frames

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void log_error_if_nonzero(const char *message, int error_code);

#endif