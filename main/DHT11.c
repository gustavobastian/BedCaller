/* 

	DHT11 temperature sensor driver
	based on DHT22 from Leopoldo Zimperz

*/

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "DHT11.h"

// == global defines =============================================

static const char* TAG = "DHT";

int DHTgpio = 34;				// my default DHT pin = 22
float humidity = 0.;
float temperature = 0.;

// == set the DHT used pin=========================================

void setDHTgpio( int gpio )
{
	DHTgpio = gpio;
}

// == get temp & hum =============================================

float getHumidity() { return humidity; }
float getTemperature() { return temperature; }

// == error handler ===============================================

void errorHandler(int response)
{
	switch(response) {
	
		case DHT_TIMEOUT_ERROR :
			ESP_LOGE( TAG, "Sensor Timeout\n" );
			break;

		case DHT_CHECKSUM_ERROR:
			ESP_LOGE( TAG, "CheckSum error\n" );
			break;

		case DHT_OK:
			break;

		default :
			ESP_LOGE( TAG, "Unknown error\n" );
	}
}

/*-------------------------------------------------------------------------------
;
;	get next state 
;
;	Use interrupts.
;
;--------------------------------------------------------------------------------*/

int getSignalLevel( int usTimeOut, bool state )
{

	int uSec = 0;
	while( gpio_get_level(DHTgpio)==state ) {

		if( uSec > usTimeOut ) 
			return -1;
		
		++uSec;
		ets_delay_us(1);		// uSec delay
	}
	
	return uSec;
}

/*----------------------------------------------------------------------------
;
;	read DHT11 sensor

copy/paste from DHT11 from AOSONG Docu:

DATA: Hum = 16 bits, Temp = 16 Bits, check-sum = 8 Bits

Example: MCU has received 40 bits data from AM2302 as
0011 0101 0000 0000 0001 1000 0000 0000 0100 1101
16 bits RH data + 16 bits T data + check sum

1) we convert 8 bits RH high data from binary system to decimal system, 0011 1000 0000 0000 → 35 0
Binary system Decimal system: RH= 0011 0101 = 35H= 53%RH

2) we convert 16 bits T data from binary system to decimal system, 0001 1000 0000 0000 → 18 0
Binary system Decimal system: T=18H=24°C

When highest bit of temperature is 1, it means the temperature is below 0 degree Celsius. 
Example: 1000 0000 0110 0101, T= minus 10.1°C: 16 bits T data

3) Check Sum=0011 0101+0000 0000+0001 1000+0000 0000=0100 1101 Check-sum=the last 8 bits of Sum=0100 1101

Signal & Timings:

The interval of whole process must be beyond 2 seconds.

To request data from DHT:

1) Sent low pulse for > 18 ms (MILI SEC)
2) Sent high pulse for > 20~40 us (Micros).
3) When DHT detects the start signal, it will pull low the bus 80us as response signal, 
   then the DHT pulls up 80us for preparation to send data.
4) When DHT is sending data to MCU, every bit's transmission begin with low-voltage-level that last 50us, 
   the following high-voltage-level signal's length decide the bit is "1" or "0".
	0: 26~28 us
	1: 70 us

;----------------------------------------------------------------------------*/

#define MAXdhtData 5	// to complete 40 = 5*8 Bits

int readDHT()
{
int uSec = 0;

uint8_t dhtData[MAXdhtData];
uint8_t byteInx = 0;
uint8_t bitInx = 7;

	for (int k = 0; k<MAXdhtData; k++) 
		dhtData[k] = 0;

	// == Send start signal to DHT sensor ===========

	gpio_set_direction( DHTgpio, GPIO_MODE_OUTPUT );

	// pull down for at least 18 ms for a smooth and nice wake up 
	gpio_set_level( DHTgpio, 0 );
	ets_delay_us( 18000 );			//3000
	// change to input mode with pullup
	gpio_set_level( DHTgpio, 1 );
	gpio_set_direction( DHTgpio, GPIO_MODE_INPUT );		
	//wait for low  	
	uSec = getSignalLevel( 80, 0 );//85
	//	ESP_LOGI( TAG, "Response = %d", uSec );
	if( uSec<0 ) return DHT_TIMEOUT_ERROR; 

	// -- 80us up ------------------------
//	printf("..\n");
	uSec = getSignalLevel( 80, 1 );//85
//	ESP_LOGI( TAG, "Response = %d", uSec );

	if( uSec<0 ) return DHT_TIMEOUT_ERROR;

	// == No errors, read the 40 data bits ================
  //	printf("...\n");
	for( int k = 0; k < 40; k++ ) {//40

		// -- starts new data transmission with >50us low signal

		uSec = getSignalLevel( 50, 0 );//55
		if( uSec<0 ) {return DHT_TIMEOUT_ERROR;}

		// -- check to see if after >70us rx data is a 0 or a 1

		uSec = getSignalLevel(1000 , 1 );//75
		if( uSec<0 ) {return DHT_TIMEOUT_ERROR;}

		// add the current read to the output data
		// since all dhtData array where set to 0 at the start, 
		// only look for "1" (>28us us)
	
		if (uSec > 28) {//40
			dhtData[ byteInx ] |= (1 << bitInx);
			}
	
		// index to next byte

		if (bitInx == 0) { bitInx = 7; ++byteInx; }
		else bitInx--;
	}

	// == get humidity from Data[0]  ==========================

	humidity = dhtData[0]*1.0;
	
	// == get temp from Data[2] 
	
	temperature = dhtData[2]*1.0;
	

	if( dhtData[2] & 0x80 ) 			// negative temp, brrr it's freezing
		temperature *= -1;

	
	// == verify if checksum is ok ===========================================
	// Checksum is the sum of Data 8 bits masked out 0xFF
	
	if (dhtData[4] == ((dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3]) & 0xFF)) 
		{printf("t: %f\n",temperature/1.0);
		printf("h: %f\n",humidity/1.0);
		return DHT_OK;}

	else 
		return DHT_CHECKSUM_ERROR;
}

