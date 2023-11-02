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
#include <pinceCapteur/PinceCapteur.h>
//#include <brasServo/brasServo.h>
#include <detecteurCouleur/detecteurCouleur.h>
#include <suivreLigne/suivreLigne.h>
#include <shortcut/shortcut.h>
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// Global variables :)
int valEtat = 0; // 0 = Ready to start, 1 = running, 2 = done
char startColor ='v';
int distanceMurDepart = 0;

//mur
const int ROUGE_GAUCHE = 49; // Front gauche
const int VERTE_GAUCHE = 48; // Back gauche
const int ROUGE_DROITE = 49; // Front droite
const int VERTE_DROITE = 48; // Back droite

// Déclaration de classes pour les différentes composantes
Moteur moteur;
Sifflet sifflet;
detecteurCouleur colorSensor = detecteurCouleur();
//BrasServo baton;
Pince pince;
SuivreLigne suivreLigne;
Shortcut shortcut;

/* ****************************************************************************
Main functions
**************************************************************************** */

void setup() {
    BoardInit();
    Serial.begin(9600);
    //capteur de proximite
    pinMode(ROUGE_GAUCHE, INPUT);
    pinMode(VERTE_GAUCHE, INPUT);
    pinMode(ROUGE_DROITE, INPUT);
    pinMode(VERTE_DROITE, INPUT);
    
    sifflet.setupSifflet();
    moteur.setupMoteur();
    colorSensor.setup();
    //baton.setupBrasServo(0);
    pince.setupPince(1);

}

void loop() {
    shortcut.shortcut();
    Serial.println(ROBUS_ReadIR(3));
}