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
#include <moteur/moteur.h>

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// Global speed, love to see it
byte valEtat = 0; // 0 = Ready to start, 1 = running, 2 = done

//mur
const int ROUGE = 49; // Left
const int VERTE = 48; // Right

// Déclaration de classes pour les différentes composantes
Moteur moteur;
Sifflet sifflet;

/* ****************************************************************************
Utility functions functions
**************************************************************************** */
bool faceAuMur() {
  int sensorGauche = digitalRead(ROUGE);
  int sensorDroit = digitalRead(VERTE);

  return (sensorGauche == LOW || sensorDroit == LOW);
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
    moteur.setupMoteur();
}

void loop() {
    switch (valEtat)
    {   
        default: //Ready to start
            if (ROBUS_IsBumper(3)) { // On entend le sifflet, ca part!
                valEtat = 1;
                moteur.avancerLigneDroite(0.4);             
            }
            break;
        case 1: // Running
            moteur.moteurPID();
            if (ROBUS_IsBumper(2)) {
                moteur.moteurArret();
                valEtat = 2;
            }
            break;
    }
}