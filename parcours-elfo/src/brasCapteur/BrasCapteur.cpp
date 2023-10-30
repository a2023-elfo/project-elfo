#include <brasCapteur/BrasCapteur.h>

void BrasEtCapteur::setupBrasEtCapteur(uint8_t moteur, uint8_t pin, uint8_t angle, int distance ){
    this->moteur = moteur;
    this->angle = angle;
    this->inputPin = pin;
    this->distanceVoulu = distance;
    SERVO_Enable(moteur);
    SOFT_TIMER_Enable(1);
}

void BrasEtCapteur::loopBrasEtCapteur(){
    int distance = ROBUS_ReadIR(inputPin);
    if(distance < distanceVoulu){
        SERVO_SetAngle(moteur, angle);
    }

    if(brasActione){
        if(this->delayCounter >= 32000){
            this->delayCounter++;
        }
        else{
            SERVO_SetAngle(this->moteur, 90);
            brasActione = false;
        }    
    } 
}