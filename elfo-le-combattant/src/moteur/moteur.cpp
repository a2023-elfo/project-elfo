#include <moteur/moteur.h>

void Moteur::setupMoteur() {
    // BoardInit(); // Already done in main.cpp
    resetVariablesMoteur();
}

// Reset the variables for the PID, or only one motor
void Moteur::resetVariablesMoteur(int moteur) {
    // Reset the variables
    if (moteur != RIGHT) {
        ENCODER_Reset(LEFT);
        integralL = 0;
        previousErrorL = 0;
        previousTimeL = 0;
        targetSpeedL = 0;
        currentSpeedL = 0;
    }

    if (moteur != LEFT) {
        ENCODER_Reset(RIGHT);
        integralR = 0;
        previousErrorR = 0;
        previousTimeR = 0;
        targetSpeedR = 0;
        currentSpeedR = 0;
    }
}

void Moteur::uniMoteurPID(int moteur) {
    float* ptrTargetSpeed;
    float* ptrCurrentSpeed;
    float* ptrCurrentCommand;
    int* ptrPreviousTime;
    float* ptrIntegral;
    float* ptrPreviousError;

    if (moteur == RIGHT) {
        ptrCurrentSpeed = &currentSpeedR;
        ptrTargetSpeed = &targetSpeedR;
        ptrIntegral = &integralR;
        ptrPreviousError = &previousErrorR;
        ptrPreviousTime = &previousTimeR;
        ptrCurrentCommand = &currentCommandR;
    } else {
        ptrCurrentSpeed = &currentSpeedL;
        ptrTargetSpeed = &targetSpeedL;
        ptrIntegral = &integralL;
        ptrPreviousError = &previousErrorL;
        ptrPreviousTime = &previousTimeL;
        ptrCurrentCommand = &currentCommandL;
    }

    // Calcul de la vitesse actuelle
    // On ne veut pas appliquer le PID trop rapidement, donc on le fait seulement à chaque 10ms
    int currentTime = millis();
    int timeDiffMs = currentTime - *ptrPreviousTime; 
    if (timeDiffMs >= 125) {
        // Calcul de la vitesse actuelle
        *ptrCurrentSpeed = getDistanceParcourue(moteur) / timeDiffMs * 1000; // cm/s
        
        // Calcul de l'erreur
        float error = *ptrTargetSpeed - *ptrCurrentSpeed;

        // Calcul de l'intégrale
        *ptrIntegral += error;

        // Calcul de la dérivée
        float derivative = (error - *ptrPreviousError)/timeDiffMs * 1000;

        // Calcul de la commande P + I + D
        float command = kp * error + ki * *ptrIntegral + kd * derivative;

        // On s'assure que la ne dépasse pas un certain cap
        if (command > acceleration) {
            command = acceleration;
        } else if (command < -deceleration) {
            command = -deceleration;
        }
        
        // On applique la commande
        *ptrCurrentCommand += command;
        *ptrCurrentCommand = constrain(*ptrCurrentCommand, -1, 1);


        MOTOR_SetSpeed(moteur, *ptrCurrentCommand);
        // On met à jour l'erreur précédente
        *ptrPreviousError = error;
        *ptrPreviousTime = currentTime;
        
        if (moteur == RIGHT) {
            Serial.println("======================");
            Serial.print("EncoderRead : ");
            Serial.println(ENCODER_Read(RIGHT));
            Serial.print("Current speed :");
            Serial.println(*ptrCurrentSpeed);
            Serial.print("Target speed :");
            Serial.println(*ptrTargetSpeed);
            Serial.print("Motor error :");
            Serial.println(error);
            Serial.print("Prev Error :");
            Serial.println(*ptrPreviousError);
            Serial.print("Integral :");
            Serial.println(*ptrIntegral);
            Serial.print("Derivative :");
            Serial.println(derivative);
            Serial.print("Command :");
            Serial.println(*ptrCurrentCommand);
        }
    }
}

void Moteur::moteurPID() {
    uniMoteurPID(LEFT);
    uniMoteurPID(RIGHT);
}

void Moteur::moteurArret() {
    resetVariablesMoteur();
    targetSpeedL = 0;
    targetSpeedR = 0;
}

void Moteur::moteurArretUrgence() {
    // Stop the motors
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    // Reset the variables
    resetVariablesMoteur();
}

void Moteur::avancerLigneDroite(float vitesse) {
    // On veut avancer en ligne droite, donc on veut que les deux moteurs aient la même vitesse
    resetVariablesMoteur();
    targetSpeedL = getDistanceVitesseEnCMparS(vitesse);
    targetSpeedR = getDistanceVitesseEnCMparS(vitesse);
}

void Moteur::moteurSetSpeedDroite(float vitesse) {
    resetVariablesMoteur(RIGHT);
    targetSpeedR = getDistanceVitesseEnCMparS(vitesse);
}

void Moteur::moteurSetSpeedGauche(float vitesse) {
    resetVariablesMoteur(LEFT);
    targetSpeedL = getDistanceVitesseEnCMparS(vitesse);
}

float Moteur::getDistanceParcourue(int moteur) {
    return ENCODER_ReadReset(moteur) * wheelCircumference / ticksPerRotation ;
}

float Moteur::getDistanceVitesseEnCMparS(float vitesse) {
    return vitesse * maxSpeedCMparS;
}
