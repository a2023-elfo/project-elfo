#include <moteur/moteur.h>

void Moteur::setupMoteur() {
    // BoardInit(); // Already done in main.cpp
    resetVariablesMoteur();
}

// Reset the variables for the PID, or only one motor
void Moteur::resetVariablesMoteur(int moteur = 3) {
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
    // Pour avoir la même logique pour les deux moteurs
    float* ptrTargetSpeed;
    float* ptrCurrentSpeed;
    int* ptrPreviousTime;
    float* ptrIntegral;
    float* ptrPreviousError;

    if (moteur == RIGHT) {
        ptrCurrentSpeed = &currentSpeedR;
        ptrTargetSpeed = &targetSpeedR;
        ptrIntegral = &integralR;
        ptrPreviousError = &previousErrorR;
        ptrPreviousTime = &previousTimeR;
    } else {
        ptrCurrentSpeed = &currentSpeedL;
        ptrTargetSpeed = &targetSpeedL;
        ptrIntegral = &integralL;
        ptrPreviousError = &previousErrorL;
        ptrPreviousTime = &previousTimeL;
    }

    // Calcul de la vitesse actuelle
    // On ne veut pas appliquer le PID trop rapidement, donc on le fait seulement à chaque 10ms
    int currentTime = millis();
    int timeDiff = currentTime - *ptrPreviousTime;
    if (timeDiff >= 10) {
        // Calcul de la vitesse actuelle
        *ptrCurrentSpeed = getDistanceParcourue(moteur) * 1000 / (timeDiff); // cm/s
        // Calcul de l'erreur
        float error = *ptrTargetSpeed - *ptrCurrentSpeed;
        // Calcul de l'intégrale
        *ptrIntegral += error;
        // Calcul de la dérivée
        float derivative = error - *ptrPreviousError;
        // Calcul de la commande P + I + D
        float command = kp * error + ki * *ptrIntegral + kd * derivative;
        // On applique la commande
        MOTOR_SetSpeed(moteur, *ptrCurrentSpeed + command);
        // On met à jour l'erreur précédente
        *ptrPreviousError = error;
        *ptrPreviousTime = currentTime;
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

void Moteur::avancerLigneDroite(float vitesse = 0.7) {
    // On veut avancer en ligne droite, donc on veut que les deux moteurs aient la même vitesse
    resetVariablesMoteur();
    targetSpeedL = vitesse;
    targetSpeedR = vitesse;
}

void Moteur::moteurSetSpeedDroite(float vitesse) {
    resetVariablesMoteur(RIGHT);
    targetSpeedR = vitesse;
}

void Moteur::moteurSetSpeedGauche(float vitesse) {
    resetVariablesMoteur(LEFT);
    targetSpeedL = vitesse;
}

float Moteur::getDistanceParcourue(int moteur) {
    return (ENCODER_Read(moteur) / ticksPerRotation) * wheelCircumference;
}
