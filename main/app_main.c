/* WiFi station Example */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "driver/gpio.h"
#include <stdint.h>  
#include "openssl/ssl.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "DHT11.h"
#include "sensor_config.h"

#include "wifi_config.h"

#include "mqtt_functions.h"

#include "keyboard.h"


/**
 * @brief task that reads values from the sensor
 * 
 * @param arg 
 */

static void sensor_task(void* arg)
{
    int ret = 0;
    
    
    while(true) {
        
        ret = readDHT();
        errorHandler(ret);
        
       // printf("DAIoT Counter_Task - Counts: %d\n", cnt++);
       
        if(ret==0){
        //sprintf(buff,"dispositivoId:%d, temperatura:,%.1f humedad:%.1f",dispositivoId,getTemperature(),getHumidity());   
        generateJson(buff,dispositivoId,nombre,ubicacion,Luz1,Luz2,getTemperature(),getHumidity());
        esp_mqtt_client_publish(client, topicDataSensor, buff, 0, 0, 0);}
        
        vTaskDelay(30000 / portTICK_RATE_MS);
        //vTaskDelay(2500 / portTICK_RATE_MS);
        }
}

/**
 * @brief simple task for processing frames received from server
 * 
 * @param arg 
 */

static void comm_task(void* arg)
{   while(true){
       if(flagProcessMessage==1)
        {processData(recTopic,recBuff);
            flagProcessMessage=0;
        }
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}

/**
 * @brief simple task for keyboard polling
 * 
 * @param arg 
 */

static void keyb_task(void* arg)
{   while(true){
      int d = keyboardRefresh();
      if(d!=0){
        printf("sending event\n");
        generateJsonKey(buff,d);
        printf("%s\n",topicCallerEvent);
        esp_mqtt_client_publish(client, topicCallerEvent, buff, 0, 0, 0);

      }
      vTaskDelay(200 / portTICK_RATE_MS);
    }
}

void app_main(void)
{
    uint8_t base_mac_addr[6] = {0};
    esp_err_t ret2 = ESP_OK;
    
    
    flagProcessMessage=0;
    
     
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    //ret2 = esp_efuse_mac_get_default(base_mac_addr);
    //printf( "wifi_Mac_address:%d %d\n", base_mac_addr[0],base_mac_addr[1]);
    //dispositivoId=base_mac_addr[1]*256+base_mac_addr[0];
    initSensor();    
    initCaller();
    keyboardInit();
    wifi_init_sta(); // 5 seg delay
    printf("Waiting 5 sec\n");
    
    mqtt_app_start();
    vTaskDelay(1000 / portTICK_RATE_MS);

    //start counter task (IT RUNS WHILE IT IS NOT DELETED)
    //TaskHandle_t sensor_task_handle = NULL;    

    //xTaskCreate(sensor_task, "sensor_task", 2048, NULL, 10, &sensor_task_handle);

    //start communication task (IT RUNS WHILE IT IS NOT DELETED)
    TaskHandle_t communication_task_handle = NULL;    

    xTaskCreate(comm_task, "comm_task", 2048, NULL, 10, &communication_task_handle);

     //start keyboard task (IT RUNS WHILE IT IS NOT DELETED)
    TaskHandle_t keyb_task_handle = NULL;    

    xTaskCreate(keyb_task, "keyb_task", 2048, NULL, 10, &keyb_task_handle);
    
}
