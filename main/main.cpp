//file: main.cpp
#include "Arduino.h"

void setup(){
  Serial.begin(115200);
  pinMode(8, OUTPUT);
  while(!Serial){
    ; // wait for serial port to connect
  }
}

void loop(){
    Serial.println("loop");
    digitalWrite(8, !digitalRead(8));
    delay(100);
}