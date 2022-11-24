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

#include "wifi_config.h"

#include "mqtt_functions.h"

#include "keyboard.h"


/**
 * @brief simple task for keyboard polling
 * 
 * @param arg 
 */

static void keyb_task(void* arg)
{   
  while(true)
  {
      int d = keyboardRefresh();
      if(d!=0)
      {
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
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  //initSensor();    
  initCaller();
  keyboardInit();
  wifi_init_sta(); 
  printf("Waiting 5 sec\n");    
  mqtt_app_start();
  vTaskDelay(1000 / portTICK_RATE_MS);

  //start keyboard task (IT RUNS WHILE IT IS NOT DELETED)
  TaskHandle_t keyb_task_handle = NULL;    
  xTaskCreate(keyb_task, "keyb_task", 2048, NULL, 10, &keyb_task_handle);
    
}
