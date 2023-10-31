#include <brasCapteur/BrasCapteur.h>

void BrasEtCapteur::setupBrasEtCapteur(uint8_t pin){
    this->inputPin = pin;
}
void BrasEtCapteur::batonSortieGauche(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,0);
    lastCommandTime = millis();
}
void BrasEtCapteur::batonSortieDroit(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,170);
    lastCommandTime = millis();

}
void BrasEtCapteur::batonRange(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,90);
    lastCommandTime = millis();
}

void BrasEtCapteur::batonUpdate() {
    if (millis() - lastCommandTime > commandDelay) {
        SERVO_Disable(this->inputPin);
        lastCommandTime = 0;
    }
}