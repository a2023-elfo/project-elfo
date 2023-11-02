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
#include <capteurCouleur/capteurCouleur.h>
#include <suivreLigne/suivreLigne.h>

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// Global variables :)
int valEtat = -1; // 0 = Ready to start, 1 = running, 2 = done
byte startColor = 2;
int distanceMurDepart = 0;
unsigned long allPurpousTimer = 0;
int tempsEnSecPourLeVirage = 3;
int compteur_tour = 1;
float floatTargetSpeed = 0.23;
int straighawayCounter=0;
float distanceAParcourirCM = 30.25 * 8;
float parcourirEnSec = 6;


//mur
const int ROUGE_GAUCHE = 48; // Back gauche
const int VERTE_GAUCHE = 49; // Front gauche
const int ROUGE_DROITE = 45; // Front droite
const int VERTE_DROITE = 44; // Back droite

// Déclaration de classes pour les différentes composantes
Moteur moteur;
Sifflet sifflet;
CapteurCouleur capteurCouleur;
BrasServo baton;
Pince pince;
SuivreLigne ligne;

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
    capteurCouleur.setupCapteurCouleur();
    baton.setupBrasServo(0);
    pince.setupPince(1);

}

void loop() {
    moteur.moteurUpdate();
    baton.batonUpdate();
    pince.pinceOuvert();
    switch (valEtat)
    {   
        case -2: 
            capteurCouleur.lireCouleur();
            delay(5000);
            break;
        case -1: // Attente de départ
            if (ROBUS_IsBumper(REAR) || sifflet.lireSifflet()) { //Remplacer le bumper par le sifflet 
                valEtat++;
                distanceMurDepart = ROBUS_ReadIR(3);
                startColor = capteurCouleur.VERT;
                moteur.avancerLigneDroite(0.2);
            }
            break;
        case 0: // Vers section 1
            if (ROBUS_ReadIR(3) < 70) {
                // Faire le calcul du quart de circonférence en fonction de la couleur de départ

                int distanceRoueDroitePouces = 0;
                int distanceRoueGauchePouces = 0;

                    if (startColor == capteurCouleur.VERT) {
                    distanceRoueDroitePouces = 1;
                    distanceRoueGauchePouces = 2;
                } else if (startColor == capteurCouleur.JAUNE) {
                    distanceRoueDroitePouces = 2;
                    distanceRoueGauchePouces = 3;
                } else { // On considère bleu
                    distanceRoueGauchePouces = 1;
                }

                float circonferenceRoueDroiteCM = (distanceRoueDroitePouces * 30.5 + 6) * 2 * PI;
                float circonferenceRoueGaucheCM = (distanceRoueGauchePouces * 30.5 - 6) * 2 * PI;

                float targetSpeedDroite = circonferenceRoueDroiteCM / 4 / tempsEnSecPourLeVirage;
                float targetSpeedGauche = circonferenceRoueGaucheCM / 4 / tempsEnSecPourLeVirage;

                moteur.setTargetSpeedGauche(targetSpeedGauche);
                moteur.setTargetSpeedDroite(targetSpeedDroite);
                allPurpousTimer = millis();
                valEtat++;
            }
            break;
        case 1: // Fin du virage, arrive sur le tapis
            if (allPurpousTimer + tempsEnSecPourLeVirage * 1000 < millis() ) {
                allPurpousTimer = 0;
                moteur.avancerLigneDroite(0.2);
                unsigned long curTime = millis();
                // Start going forward to before checking
                // if we are at suiveur de ligne
                while (curTime + 2000 > millis())
                {
                    moteur.moteurUpdate();
                }
                valEtat++;
            }
            break;
        case 2: // Commence le deuxieme virage
            if (ROBUS_ReadIR(3) < 70) {
                // Faire le calcul du quart de circonférence en fonction de la couleur de départ
                moteur.moteurArretUrgence();

                int distanceRoueDroitePouces = 0;
                int distanceRoueGauchePouces = 0;

                if (startColor == capteurCouleur.VERT) {
                    distanceRoueDroitePouces = 1;
                    distanceRoueGauchePouces = 2;
                } else if (startColor == capteurCouleur.JAUNE) {
                    distanceRoueDroitePouces = 2;
                    distanceRoueGauchePouces = 3;
    
                } else { // On considère bleu
                    distanceRoueGauchePouces = 1;
                }

                float circonferenceRoueDroiteCM = (distanceRoueDroitePouces * 30.5 + 6) * 2 * PI;
                float circonferenceRoueGaucheCM = (distanceRoueGauchePouces * 30.5 - 6) * 2 * PI;

                float targetSpeedDroite = circonferenceRoueDroiteCM / 4 / tempsEnSecPourLeVirage;
                float targetSpeedGauche = circonferenceRoueGaucheCM / 4 / tempsEnSecPourLeVirage;

                if (startColor == capteurCouleur.VERT) {
                    // Cramp le vert pour être certain de pas pogner le bois
                    targetSpeedGauche += 4;
                }
                else {
                    targetSpeedGauche += 1;
                }

                // Speed up a bit because we always end up on the wood thing
                moteur.setTargetSpeedGauche(targetSpeedGauche);
                moteur.setTargetSpeedDroite(targetSpeedDroite);
                allPurpousTimer = millis();
                valEtat++;
            }
            break;
        case 3: // Même virage que le premier
            if (allPurpousTimer + (tempsEnSecPourLeVirage - 0.75) * 1000 < millis() ) {
                // Fuck le verre, on veut juste faire un tour.
                allPurpousTimer = 0;
                moteur.moteurArret();
                moteur.setTargetSpeedDroite(distanceAParcourirCM/parcourirEnSec);
                moteur.setTargetSpeedGauche(distanceAParcourirCM/parcourirEnSec);
                valEtat++;
            }
            break;
        case 4:
            // Je veux avancer en ligne droite pour 8 pieds
            if (digitalRead(VERTE_GAUCHE) == LOW) {
                baton.batonSortieGauche();
                allPurpousTimer = millis();
            }

            if (allPurpousTimer + 1500 < millis()) {
                baton.batonRange();
                allPurpousTimer = 0;
            }

            Serial.println(moteur.getDistanceParcourueDroiteEnCM());

            if (moteur.getDistanceParcourueDroiteEnCM() >= 230) {
                // On vire 180 degreee pour le big virage apères le shortcut
                int distanceRoueDroitePouces = 2;
                int distanceRoueGauchePouces = 3;
                float circonferenceRoueDroiteCM = (distanceRoueDroitePouces * 30.5 + 6) * 2 * PI;
                float circonferenceRoueGaucheCM = (distanceRoueGauchePouces * 30.5 - 6) * 2 * PI;

                float targetSpeedDroite = circonferenceRoueDroiteCM / 2 / 8;
                float targetSpeedGauche = circonferenceRoueGaucheCM / 2 / 8;

                moteur.setTargetSpeedGauche(targetSpeedGauche);
                moteur.setTargetSpeedDroite(targetSpeedDroite);
                allPurpousTimer = millis();
                valEtat++;
            }
            break;
        case 5:
            if (allPurpousTimer + 8000 < millis()) {
                //Le virage est terminé, full speed ahead pour le jump.
                moteur.avancerLigneDroite(0.6);
                allPurpousTimer = millis();
                valEtat++;
            }
            break;
            
        case 6: 
            if (allPurpousTimer + 2000 < millis()) {
                //Le virage est terminé, full speed ahead pour le jump.
                moteur.avancerLigneDroite();
                valEtat = 0;
                // On a fini, on refait le tour
            }
            
            

            break;
        default: //Ready to start
            Serial.print("default");
            break;
    }
}