#include "DHT.h"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#define DHTPIN 8     // Digital pin connected to DHT11
int sensorValue = 0; //Analog yl69 input value;

void setup() {
  // initialize serial:
   Serial.begin(300);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  dht.begin();
  Serial.println("Initializing... Please wait.");

  pinMode(A0, INPUT);
  delay(2000);
}

void loop() {
  
  //Read humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  int sensorValue = analogRead(A0);
  
  String sendstring = "Humidity is " + String(h) + "%     Temprature is " + String(t) + "C    Yl69 humidity is " + String(sensorValue);
  Serial.println(sendstring);
  delay(1000);
    //}
  //}
}
