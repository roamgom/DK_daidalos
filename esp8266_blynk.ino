#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SimpleTimer.h>

// Blynk Auth Key
const char auth[] = "f237b763a5e04ba1ba3fd95e1b1500cc";
// Wifi SSID / PW
const char ssid[] = "Gomtting";
const char pass[] = "Gomtting1029";

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

// PIN SETTING / DHT Sensor
#define DUSTPOWER 16       // D0
#define DHTPIN 14          // D5
#define DHTTYPE DHT22   // DHT 22 (sensor type)

int ledPin = 2;   // Builtin PIN
int DUSTPIN = 0;  // A0 to read DUST VALUE

// DUST Calculate Variable
float voltage = 0;
float calcVoltage = 0;
float dustDensity = 0;

// TimeSet
const int startTime = 280;
const int delayTime = 40;

// Define DHT
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
float humidity, tempC, tempF;

// Send time (V5)
void sendUptime() {
  Blynk.virtualWrite(V5, millis() / 1000);
}

// Send Temperature (V12-V14)
void sendTempSensor() {
  humidity = dht.readHumidity();
  tempC = dht.readTemperature(false);
  tempF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V12, humidity);
  Blynk.virtualWrite(V13, tempC);
  Blynk.virtualWrite(V14, tempF);


  float Fahrenheit = dht.computeHeatIndex(tempF, humidity);        // 화씨 열지수(체감온도)를 계산합니다.
  float Celcius = dht.computeHeatIndex(tempC, humidity, false);    // 섭씨 열지수(체감온도)를 계산합니다.


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

// Send DustDensity(V15)
void sendDustSensor() {
  digitalWrite(DUSTPOWER, HIGH);
  delayMicroseconds(delayTime);

  voltage = analogRead(DUSTPIN);

  digitalWrite(DUSTPOWER, LOW);

  calcVoltage = voltage * (5.0 / 1024.0);
  dustDensity = 1000.0 * (0.172 * calcVoltage - 0.1);

  Blynk.virtualWrite(V15, dustDensity);

  Serial.print("DUST Density : ");
  Serial.println(dustDensity);
}


void setup() {
  delayMicroseconds(startTime);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  pinMode(DUSTPOWER, OUTPUT);
  Serial.begin(115200);
    
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  Serial.println("DHTxx test!");
  timer.setInterval(1000L, sendUptime);
  timer.setInterval(1000L, sendTempSensor);
  timer.setInterval(1000L, sendDustSensor);
}

void loop() {
  delayMicroseconds(startTime);
  
  Blynk.run();
  timer.run();

  delayMicroseconds(delayTime);

// Moved to Upper side
/*
  //DustDensity
  digitalWrite(DUSTPOWER, HIGH);
  delayMicroseconds(delayTime);

  voltage = analogRead(DUSTPIN);

  digitalWrite(DUSTPOWER, LOW);

  calcVoltage = voltage * (5.0 / 1024.0);
  dustDensity = 1000.0 * (0.172 * calcVoltage - 0.1);

  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V15, dustDensity);

  float Fahrenheit = dht.computeHeatIndex(tempF, humidity);        // 화씨 열지수(체감온도)를 계산합니다.
  float Celcius = dht.computeHeatIndex(tempC, humidity, false);    // 섭씨 열지수(체감온도)를 계산합니다.


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

  Serial.print("DUST Density : ");
  Serial.println(dustDensity);
  */
}