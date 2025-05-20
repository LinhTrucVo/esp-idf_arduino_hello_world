//file: main.cpp
#include "Arduino.h"

void setup(){
  pinMode(8, OUTPUT);

  Serial.begin(115200);
  Serial.println("begin");
}

void loop(){
    Serial.println("loop");
    digitalWrite(8, !digitalRead(8));
    delay(200);
}