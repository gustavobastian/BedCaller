# BedCaller

Simple bed caller simulator (ESP32) that uses MQTT to publish simulated bed calls.

DISCLAIMER: "this work was part of the end project for the IoT specialist diploma of UBA(December 2022). The repository owner makes no warranties or guarantees regarding the accuracy, completeness, or usefulness of the information provided. The user is solely responsible for any risks associated with using the repository's contents. Any use of the software or documentation is at the user's own risk. This project is not longer maintained."

Microcontroller Board:  [https://descubrearduino.com/esp32-modulo-esp32-wroom-gpio-pinout/](https://descubrearduino.com/esp32-modulo-esp32-wroom-gpio-pinout/)

Keyboard: [https://protosupplies.com/product/tactile-keypad-4x4-matrix/](https://protosupplies.com/product/tactile-keypad-4x4-matrix/)

The firmware use FreeRTOS: [https://www.freertos.org/](https://www.freertos.org/)

The system use MQTT websocket with no auth.

## How to compile

In order to compile the project, a file called "passwords_file.h" must be created inside ./main with the following content:

```bash
#ifndef PASSWORDS_H_  
#define PASSWORDS_H_

#define BROKER_URI "ws://xxxx.xxxx.xxxx.xxxx:9001"


#define EXAMPLE_ESP_WIFI_SSID      "MYNETWORK"
#define EXAMPLE_ESP_WIFI_PASS      "MYNETWORKPASS"

#endif
```

You must have installed esp-idf extension installed in VSCode. This project was compiled with esp-idf-v4.4.1 and with v4.4.5.

## Example of publication

```bash
topic : \Beds\Caller-events
message: {"callerId": 2}
```

## Interconection

<img src="./Doc/simulador.png" style="width:50%; padding-left:15%">
