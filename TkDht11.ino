// This #include statement was automatically added by the Particle IDE.
#include "HttpClient/HttpClient.h"

// This #include statement was automatically added by the Particle IDE.
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
unsigned int VDhtTemp;
unsigned int VDhtHumid;
unsigned int VDhtCelvin;
unsigned int VDhtFarenh;
unsigned int VDhtHeati;

DHT dht(DHTPIN, DHTTYPE);
// Value ID's for www.ubidots.com to show the data
#define VARIABLE_ID01 "put here your ID which you have set in ubidots for variable 01"// HUMID
#define VARIABLE_ID02 "put here your ID which you have set in ubidots for variable 01"// Temp
#define VARIABLE_ID03 "put here your ID which you have set in ubidots for variable 01"// Celvin
#define TOKEN "put here your TOKEN which you have got while settings in ubidots for the token"

    HttpClient http;

    // Headers currently need to be set at init, useful for API keys etc.
    http_header_t headers[] = {
        { "Content-Type", "application/json" },
        { NULL, NULL } // NOTE: Always terminate headers will NULL
    };

    http_request_t request;
    http_response_t response;

void setup() {
	Serial.begin(9600); 
	Serial.println("DHTxx test!");
    dht.begin();
    request.hostname = "things.ubidots.com";
    request.port = 80;
   // request.path = "/api/v1.6/variables/"VARIABLE_ID02"/values?token="TOKEN;
}

void loop() {
// Wait a few seconds between measurements.
//Time every 15 second

    unsigned long now = millis();
    //Every 15 seconds publish uptime
    if (now-lastTime>15000UL) {
        lastTime = now;
        // now is in milliseconds
        unsigned nowSec = now/1000UL;
        unsigned sec = nowSec%60;
        unsigned min = (nowSec%3600)/60;
        unsigned hours = (nowSec%86400)/3600;
        sprintf(publishStringTimeDht,"%u:%u:%u: \RF =\%u  ;\T (C) =\%u ; \T (K) =\%u",hours,min,sec,VDhtHumid,VDhtTemp,VDhtCelvin);
        Spark.publish("TimeDHT",publishStringTimeDht);
        
        //ubidots, your can sign in www.ubidots.com, it's free of charts
        // Send sensor value (Humid RF, Temp (C), Celvin(K) for e.g.

        Serial.println("Sending data ...");
	// first value to send to ubidots
        request.path = "/api/v1.6/variables/"VARIABLE_ID01"/values?token="TOKEN;
  
        request.body = "{\"value\":" + String(VDhtHumid) + "}";        
        // second value to send to ubidots
        request.path = "/api/v1.6/variables/"VARIABLE_ID02"/values?token="TOKEN;

        request.body = "{\"value\":" + String(VDhtTemp) + "}";
	// third value to send to ubidots
        request.path = "/api/v1.6/variables/"VARIABLE_ID03"/values?token="TOKEN;

        request.body = "{\"value\":" + String(VDhtCelvin) + "}";

        // Post request
        http.post(request, response, headers);
        Serial.println(response.status);
        Serial.println(response.body);

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
// Delay auf 2 min
//delay(120000);

// Compute heat index
// Must send in temp in Fahrenheit!
	float hi = dht.getHeatIndex();
	float dp = dht.getDewPoint();
	float k = dht.getTempKelvin();

	Serial.print("Humid: "); 
	Serial.print(h);
	VDhtHumid = (h);
	Serial.print("% - ");
	Serial.print("Temp: "); 
	Serial.print(t);
	VDhtTemp = (t);
	Serial.print("*C ");
	Serial.print(f);
	VDhtFarenh = (f);
	Serial.print("*F ");
	Serial.print(k);
	VDhtCelvin = (k);
	Serial.print("*K - ");
	Serial.print("DewP: ");
	Serial.print(dp);
	Serial.print("*C - ");
	Serial.print("HeatI: ");
	Serial.print(hi);
	VDhtHeati = (hi);
	Serial.println("*C");
	Serial.println(Time.timeStr());

}
