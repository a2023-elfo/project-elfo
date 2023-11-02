#include <brasServo/brasServo.h>

void BrasServo::setupBrasServo(uint8_t pin){
    this->inputPin = pin;
    this->batonRange();
}
void BrasServo::batonSortieGauche(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,0);
    lastCommandTime = millis();
}
void BrasServo::batonSortieDroit(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,170);
    lastCommandTime = millis();

}
void BrasServo::batonRange(){
    SERVO_Enable(this->inputPin);
    SERVO_SetAngle(this->inputPin,90);
    lastCommandTime = millis();
}

// Vérifie si le bras doit être désactivé
void BrasServo::batonUpdate() {
    if (lastCommandTime != 0 && millis() - lastCommandTime > commandDelay) {
        SERVO_Disable(this->inputPin);
        lastCommandTime = 0;
    }
}