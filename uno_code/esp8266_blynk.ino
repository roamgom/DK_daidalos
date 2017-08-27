#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Blynk Auth Key
char auth[] = "...";
// Wifi SSID(Name) / PW
char ssid[] = "...";
char pass[] = "...";

// PIN SETTING
#define DHTPIN 2          // D4 on NodeMCU

// PIN NUMBER of NodeMCU
/*
static const uint8_t D0   = 16; 
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2; 
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
*/


#define DHTTYPE DHT22   // DHT 22 (sensor type)

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
float humidity, tempC, tempF;

void sendUptime(){
  Blynk.virtualWrite(V5, millis() / 1000);
}

void sendTempSensor(){
  humidity = dht.readHumidity();
  tempC = dht.readTemperature();
  tempF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(tempC) || isnan(tempF)){
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Connect to Blynk with Sensor by VirtualPin
  Blynk.virtualWrite(V12, humidity);
  Blynk.virtualWrite(V13, tempC);
  Blynk.virtualWrite(V14, tempF);
  
}



void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  Serial.println("DHTxx test!");
  timer.setInterval(1000L, sendUptime); 
  timer.setInterval(1000L, sendTempSensor);
}

void loop() {
  // Blynk APP start
  Blynk.run();
  timer.run();
  humidity = dht.readHumidity();
  tempC = dht.readTemperature();
  tempF = dht.readTemperature(true);

  // Error Catch on Monitor
   if (isnan(humidity) || isnan(tempC) || isnan(tempF)){
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Temperature Monitoring on PC
  float Fahrenheit = dht.computeHeatIndex(tempF, humidity);        // Calculate Fahrenheit
  float Celcius = dht.computeHeatIndex(tempC, humidity, false);    // Calculate Celsius

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" *C ");

  Serial.print(tempF);
  Serial.print(" *F\t");


  Serial.print("Heat index: ");

  Serial.print(Celcius);
  Serial.print(" *C ");

  Serial.print(Fahrenheit);
  Serial.println(" *F");

}