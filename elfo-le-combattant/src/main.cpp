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
#include <brasServo/brasServo.h>
#include <detecteurCouleur/detecteurCouleur.h>

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// Global variables :)
int valEtat = -1; // 0 = Ready to start, 1 = running, 2 = done
char startColor =colorSensor.VERT;
int distanceMurDepart = 0;

//mur
const int ROUGE_DROITE = 45; // Front droite
const int VERTE_DROITE = 44; // Back droite
const int ROUGE_GAUCHE = 48; // Front gauche
const int VERTE_GAUCHE = 49; // Back gauche

// Déclaration de classes pour les différentes composantes
Moteur moteur;
Sifflet sifflet;
detecteurCouleur colorSensor = detecteurCouleur();
BrasServo baton;
Pince pince;

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
    baton.setupBrasServo(0);
    pince.setupPince(1);

}

void loop() {
    moteur.moteurUpdate();
    baton.batonUpdate();

    switch (valEtat)
    {   
        case -1: // Attente de départ
            if (ROBUS_IsBumper(REAR)) { 
                valEtat++;
                startColor = colorSensor.getCouleur();
                distanceMurDepart = ROBUS_ReadIR(3);
                moteur.avancerLigneDroite();
            }
            break;
        case 0: // Vers section 1
            if (ROBUS_ReadIR(3) + 200 < distanceMurDepart) {
                // Moteur commence à virer à droite
                // TODO faire une vraie fonction pour ceci lol xd mdr
                moteur.moteurSetSpeedDroite(0.7);
                valEtat++;
            }
            break;
        case 1: // Fin du virage, arrive sur le tapis
            if (colorSensor.NOIR == colorSensor.getCouleur()) {
                moteur.avancerLigneDroite();
                valEtat++;
            }
            break;
        case 2: // Commence le deuxieme virage
            if (ROBUS_ReadIR(3) < distanceMurDepart + 100) {
                // Moteur commence à virer à droite
                // TODO faire une vraie fonction pour ceci lol xd mdr
                moteur.avancerLigneDroite(0.3);
                valEtat++;
            }
            break;
        case 3: // On est arrivé pour faire tomber le verre

            // Dépendant de notre couleur, on check un des deux capteurs de distance d'APP1
            if (startColor == colorSensor.VERT && digitalRead(VERTE_GAUCHE) == LOW) {
                baton.batonSortieGauche();
                moteur.avancerLigneDroite();
                // On attends que le capteur est aligné avec l'arrière
                while(digitalRead(ROUGE_GAUCHE) == HIGH) {
                    delay(0);
                }
                delay(200);
                baton.batonRange();
                valEtat++;
            } else if (startColor == colorSensor.JAUNE && digitalRead(ROUGE_DROITE) == LOW) {
                baton.batonSortieDroit();
                moteur.avancerLigneDroite();
                // On attends que le capteur est aligné avec l'arrière
                while(digitalRead(VERTE_GAUCHE) == HIGH) {
                    delay(0);
                }
                delay(200);
                baton.batonRange();
                valEtat++;
            }
            break;
        case 4: // On se rend au suiveur de ligne
            if (colorSensor.JAUNE == colorSensor.getCouleur()) {
                valEtat++;
            }

            break;
        default: //Ready to start
            Serial.print("default");
            break;
    }
}