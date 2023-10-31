#include <brasCapteur/BrasCapteur.h>

void BrasEtCapteur::setupBrasEtCapteur(uint8_t moteur, uint8_t pin, uint8_t angle, int distance ){
    this->moteur = moteur;
    this->angle = angle;
    this->inputPin = pin;
    this->distanceVoulu = distance;
    SERVO_Enable(moteur);
}

void BrasEtCapteur::loopBrasEtCapteur(){
    int distance = ROBUS_ReadIR(inputPin);
    if(!brasActione){
        if(distance < distanceVoulu){
            SERVO_SetAngle(moteur, angle);
            startTime = millis();
            brasActione = true;
        } 
    }
    if(brasActione){
        int timeDiff = millis() - startTime;
        if(timeDiff > 3000){
            SERVO_SetAngle(this->moteur, 90);
        }
    }
}