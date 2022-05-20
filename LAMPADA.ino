#include <ESP8266WiFi.h>   
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "G(8)Pedro"    
#define WIFI_PASS         "Pedro123"
#define APP_KEY           "259f50d7-3382-4b5b-8511-26e452db7453"
#define APP_SECRET       "a34a4f24-575d-4f7c-9916-303fa8d960fe-d90305fe-fcfd-44a3-a7ea-c2439d2195e5"

#define LampadaQuarto_ID      "62755aeb1d6a67083b4af45b"
#define LampadaQuarto D2  // O pino fisico onde está ligado
#define LampadaSala_ID      "627ea3daa6872f3639273014"
#define LampadaSala D3 // O pino fisico onde está ligado

#define BAUD_RATE         9600                // Se precisar, pode trocar o baud rate


void setupWiFi();
void setupSinricPro();


// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro(); 
  pinMode(LampadaSala, OUTPUT);
   pinMode(LampadaQuarto, OUTPUT);
   
}

void loop() {
  SinricPro.handle();
}

bool LampadaState(const String &deviceId, bool &state) {
  Serial.printf("A lampada foi %s\r\n", state?"ligada":"desligada");
  digitalWrite(LampadaQuarto, state);
  return true; // request handled properly

}

bool LampadaState2(const String &deviceId, bool &state) {
  Serial.printf("A lampada foi %s\r\n", state?"ligada":"desligada");
  digitalWrite(LampadaSala, state);
  return true; // request handled properly

}

// setup das conexões Wifi
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Conectando...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("Conectado!\r\n[WiFi]: Endereço de IP e %s\r\n", WiFi.localIP().toString().c_str());
}

// setup das funções para o SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro

  SinricProSwitch &mySwitch1 = SinricPro[LampadaQuarto_ID];
  mySwitch1.onPowerState(LampadaState);

    SinricProSwitch &mySwitch2 = SinricPro[LampadaSala_ID];
  mySwitch2.onPowerState(LampadaState2);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Conectado a nuvem SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Desconectado a nuvem SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
