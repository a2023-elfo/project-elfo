#include <brasCapteur/BrasCapteur.h>


void BrasEtCapteur::setupCapteurBras(){
    pinMode(inputPin, INPUT);

    SERVO_Enable(moteur);
}

void BrasEtCapteur::loopCapteurBras(){
    int distance = analogRead(inputPin);

    if(distance < 600){
        SERVO_SetAngle(moteur, angle);
    }

}