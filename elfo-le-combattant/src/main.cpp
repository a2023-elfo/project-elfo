/*
Projet: Jordan.cpp
Equipe: ELFO
Auteurs: Les membres auteurs du script
Description: Started coding 24h before deadline :)
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <Arduino.h>
#include <math.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// Global speed, love to see it
const float defaultVitesseR = 0.25;
const float defaultVitesseL = 0.265;
const float turningSpeed = 0.3;
float vitesseR, vitesseL;

byte valEtat = 0; // 0 = Ready to start, 1 = running, 2 = done

//mur
const int ROUGE = 49; // Left
const int VERTE = 48; // Right

//sifflet
int PIN_AMBIANT = A6;
int PIN_5KHZ = A7;
const int soundAverageSize = 10;
int soundAverage[soundAverageSize] = {0};
int soundCounter = 0;
long int firstSpikeTime = 0;

// Debug variables
#define ELFO_DEBUG true
long int lastDebugPrintTime = 0;

/* ****************************************************************************
Utility functions functions
**************************************************************************** */
bool faceAuMur() {
  int sensorGauche = digitalRead(ROUGE);
  int sensorDroit = digitalRead(VERTE);

  return (sensorGauche == LOW || sensorDroit == LOW);
}

void resetMotorVariables() {
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    vitesseL = defaultVitesseL;
    vitesseR = defaultVitesseR;
}

bool sifflet(){
    int value5khz = analogRead(PIN_5KHZ);

    delay(50);
    //comparaison du son present a la moyenne du son
    if (soundCounter > soundAverageSize) {
        //fait une moyenne du son
        int sum = 0;
        for (int i = 0; i < (int)soundAverageSize; i++) {
            sum += soundAverage[i];
        }
        float average = sum / soundAverageSize;
        //verifie la longueur du spike
        if (value5khz > (average + 60)) {
            if (firstSpikeTime == 0) {
              firstSpikeTime = millis();
            }
            else {
                long int timeSinceSpike = millis() - firstSpikeTime;
                if (timeSinceSpike > 1000) {
                    Serial.println("Sifflet!");
                    return true;
                }
            }
        }
        else {
            firstSpikeTime = 0;
            soundAverage[soundCounter%soundAverageSize] = value5khz;
            soundCounter++;
        }
    }
    else {
      soundAverage[soundCounter%soundAverageSize] = value5khz;
      soundCounter++;
    }

    return false;
}

/* ****************************************************************************
Movement functions
**************************************************************************** */

void arret() {
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(150);
}

/* ****************************************************************************
Main functions
**************************************************************************** */

void setup() {
    BoardInit();
    Serial.begin(9600);
    pinMode(ROUGE, INPUT);
    pinMode(VERTE, INPUT);
    pinMode(PIN_5KHZ, INPUT);
    // Set default speeds
    vitesseL = defaultVitesseL;
    vitesseR = defaultVitesseR;
}

void printDebugInfo() {
    int msDelay = 250;
    if (millis() > lastDebugPrintTime) {
        Serial.println("This is a debug call");

        lastDebugPrintTime += msDelay;
    }
    
}

void loop() {
    if (ELFO_DEBUG) {
        printDebugInfo();
    }
    
    switch (valEtat)
    {   
        default: //Ready to start
            if (sifflet() || ROBUS_IsBumper(3)) { // On entend le sifflet, ca part!
                valEtat = 1;
            }
            break;
    }
}