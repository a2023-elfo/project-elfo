#include <brasCapteur/BrasCapteur.h>

void BrasEtCapteur::setupBrasEtCapteur(uint8_t pin){
    this->inputPin = pin;
}
void BrasEtCapteur::batonSortieGauche(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,0);
    delay(100);
    SERVO_Disable(this->inputPin);
}
void BrasEtCapteur::batonSortieDroit(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,170);
    delay(100);
    SERVO_Disable(this->inputPin);
}
void BrasEtCapteur::batonRange(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,90);
    delay(100);
    SERVO_Disable(this->inputPin);
}