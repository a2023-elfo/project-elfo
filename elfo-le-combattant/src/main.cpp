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
#include <sifflet/sifflet.h>

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
Sifflet sifflet;

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


/* ****************************************************************************
Main functions
**************************************************************************** */

void setup() {
    BoardInit();
    Serial.begin(9600);
    pinMode(ROUGE, INPUT);
    pinMode(VERTE, INPUT);
    sifflet.setupSifflet();
    // Set default speeds
    vitesseL = defaultVitesseL;
    vitesseR = defaultVitesseR;
}

void loop() {
    switch (valEtat)
    {   
        default: //Ready to start
            if (sifflet.lireSifflet() || ROBUS_IsBumper(3)) { // On entend le sifflet, ca part!
                valEtat = 1;
            }
            break;
    }
}