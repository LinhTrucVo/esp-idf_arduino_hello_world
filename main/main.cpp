//file: main.cpp
#include "Arduino.h"

#if CONFIG_IDF_TARGET_ESP32C3
  #define LED_BUILTIN 8
#else
  #define LED_BUILTIN 2
#endif

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("begin");
}

void loop(){
    Serial.println("hello world");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
}