#ifndef WIFI_CONFIG_H_  
#define WIFI_CONFIG_H_

#include "openssl/ssl.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "passwords_file.h"

/* 
    Set SSID, Password, retries allowed.
*/
#define EXAMPLE_ESP_MAXIMUM_RETRY  10
extern const char *TAG;

/* FreeRTOS event group to signal when we are connected*/
extern EventGroupHandle_t s_wifi_event_group;

//static const char *TAG = "wifi station";

extern int s_retry_num;


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


/**
 * @brief  The event handler defines what to do in every event 
 * 
 * @param arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);

/**
 * @brief  wifi initialization function
 * 
 */
void wifi_init_sta(void);
                                

#endif