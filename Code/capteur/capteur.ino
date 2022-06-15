#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include "Ubidots.h"

const char* UBIDOTS_TOKEN = "BBFF-Zwa18gDt0RRhwIwn8lwNuMRnGJJiAM";
const char* ssid = "samyPC";
const char* password = "123456789";

#define GATE 4
#define ADC A0
#define Threshold 12

unsigned long currentMillis(0),lastMillis(0),lastMillis1(0);
float analogReading(0);
int percent(0);
bool threshold(0);
bool isHIGH = false;

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);


void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();
  
  pinMode(GATE, OUTPUT);
  pinMode(ADC, INPUT);
  pinMode(Threshold, INPUT);
}

void loop() {
  ArduinoOTA.handle();

  currentMillis = millis();
  
  if (currentMillis - lastMillis >= 500){
    analogReading = analogRead(ADC);
    percent = analogReading * 0.09765625;
    threshold = digitalRead(Threshold);

    ubidots.add("Gaz level", percent);// Change for your variable name  
    ubidots.add("threshold", threshold);
  
    bool bufferSent = false;
    bufferSent = ubidots.send();

    if (bufferSent) {
      Serial.println("Values sent by the device");
    }
    lastMillis = currentMillis;
  }

  if(isHIGH == true){
    if (currentMillis - lastMillis1 >= 60000){
      analogWrite(GATE, 71);
      isHIGH = false;
    lastMillis1 = currentMillis;
    }
  }
  else{
    if (currentMillis - lastMillis1 >= 90000){
      analogWrite(GATE, 255);
      isHIGH = true;
    lastMillis1 = currentMillis;
    }
  }
}
