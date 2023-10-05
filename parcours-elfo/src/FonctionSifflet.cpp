#include <Arduino.h>
#include <LibRobus.h>

//pin A6,A7

void sifflet(){
    int pinAmbiant = A6;
    //int pin5Khz = A7;
    
    int value = analogRead(pinAmbiant);
    Serial.begin(9600);
    Serial.println(value);
}