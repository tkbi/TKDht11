// TkDht11.ino; use with TkDht11.cpp and TkDht11.h
// Thanks to the work of :
// MIT license
// written by Adafruit Industries
// modified for Spark Core by RussGrue

#include "tkdht11.h"


// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11		// DHT 11 
//#define DHTTYPE DHT22		// DHT 22 (AM2302)
//#define DHTTYPE DHT21		// DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// publishme.ino -- Spark Publishing Time and DHT 
unsigned long lastTime = 0UL;
char publishStringTimeDht[63];
unsigned DhtTemp;
unsigned DhtHumid;
unsigned DhtCelvin;


DHT dht(DHTPIN, DHTTYPE);

void setup() {
	Serial.begin(9600); 
	Serial.println("DHTxx test!");

	dht.begin();
}

void loop() {
// Wait a few seconds between measurements.
//	delay(2000); // because it is used a timing in the loop for pushing data in the cloud

//Time every 15 second, will published up in the cloud with the real time data of Humid, Temp (C) and Celvin

    unsigned long now = millis();
    //Every 15 seconds publish uptime
    if (now-lastTime>15000UL) {
        lastTime = now;
        // now is in milliseconds
        unsigned nowSec = now/1000UL;
        unsigned sec = nowSec%60;
        unsigned min = (nowSec%3600)/60;
        unsigned hours = (nowSec%86400)/3600;
        sprintf(publishStringTimeDht,"%u:%u:%u: \Humid =\%u ;\Temp (C) =\%u ; \Celvin =\%u",hours,min,sec,DhtHumid,DhtTemp,DhtCelvin);
        Spark.publish("TimeDHT",publishStringTimeDht);
        }
// Reading temperature or humidity takes about 250 milliseconds!
//getDHT()
// Sensor readings may also be up to 2 seconds 'old' (its a 
// very slow sensor)
	float h = dht.getHumidity();
// Read temperature as Celsius
	float t = dht.getTempCelcius();
// Read temperature as Farenheit
	float f = dht.getTempFarenheit();
  
// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(t) || isnan(f)) {
		Serial.println("Failed to read from DHT sensor!");
		return;
	}

// Compute heat index
// Must send in temp in Fahrenheit!
//SerialPrint and picking the data for publishing on the cloud
	float hi = dht.getHeatIndex();
	float dp = dht.getDewPoint();
	float k = dht.getTempKelvin();

	Serial.print("Humid: "); 
	Serial.print(h);
	DhtHumid = (h);
	Serial.print("% - ");
	Serial.print("Temp: "); 
	Serial.print(t);
	DhtTemp = (t);
	Serial.print("*C ");
	Serial.print(f);
	Serial.print("*F ");
	Serial.print(k);
	//There are a prblem with getting DhtCelvin = (k), which is "0", I'll work on it
	DhtCelvin = (k);
	Serial.print("*K - ");
	Serial.print("DewP: ");
	Serial.print(dp);
	Serial.print("*C - ");
	Serial.print("HeatI: ");
	Serial.print(hi);
	Serial.println("*C");
	Serial.println(Time.timeStr());
}
