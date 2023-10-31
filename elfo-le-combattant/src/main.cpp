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
#include <brasServo/brasServo.h>

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
BrasServo baton;

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
    //capteur de proximite
    pinMode(ROUGE, INPUT);
    pinMode(VERTE, INPUT);
    sifflet.setupSifflet();
    moteur.setupMoteur();
    baton.setupBrasServo(0);
    //bras et capteur baton


}

void loop() {
    moteur.moteurUpdate();
    baton.batonUpdate();

    if (ROBUS_IsBumper(REAR)) {
        baton.batonRange();
    } else if (ROBUS_IsBumper(LEFT)) {
        baton.batonSortieGauche();
    } else if (ROBUS_IsBumper(RIGHT)) {
        baton.batonSortieDroit();
    }
}