#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Blynk Auth Key
char auth[] = "f237b763a5e04ba1ba3fd95e1b1500cc";
// Wifi SSID / PW
char ssid[] = "Gomtting";
char pass[] = "Gomtting1029";

// PIN SETTING
#define DHTPIN 2          // D4

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

  Blynk.virtualWrite(V12, humidity);
  Blynk.virtualWrite(V13, tempC);
  Blynk.virtualWrite(V14, tempF);
  
}



void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, "Gomtting", "Gomtting1029");
  dht.begin();
  Serial.println("DHTxx test!");
  timer.setInterval(1000L, sendUptime); 
  timer.setInterval(1000L, sendTempSensor);
}

void loop() {
  Blynk.run();
  timer.run();
  humidity = dht.readHumidity();
  tempC = dht.readTemperature();
  tempF = dht.readTemperature(true);

   if (isnan(humidity) || isnan(tempC) || isnan(tempF)){
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float Farenheit = dht.computeHeatIndex(tempF, humidity);        // 화씨 열지수(체감온도)를 계산합니다.
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

  Serial.print(Farenheit);
  Serial.println(" *F");

}