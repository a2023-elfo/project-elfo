#include <brasCapteur/BrasCapteur.h>


void BrasEtCapteur::setupCapteurBras(uint8_t moteur, uint8_t pin, uint8_t angle ){
    this->moteur = moteur;
    this->angle = angle;
    this->inputPin = pin;
    SERVO_Enable(moteur);
}

void BrasEtCapteur::loopCapteurBras(){
    int distance = ROBUS_ReadIR(inputPin);
    if(distance < 600){
        SERVO_SetAngle(moteur, angle);
    }

}