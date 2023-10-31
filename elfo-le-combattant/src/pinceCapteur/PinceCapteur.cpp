#include <PinceCapteur/PinceCapteur.h>

void Pince::setupPince(uint8_t pin){
    this->inputPin = pin;
}
void Pince::pinceOuvert(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,0);
    delay(100);
    SERVO_Disable(this->inputPin);
}
void Pince::Pinceferme(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,90);
}