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

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// Global variables :)
int valEtat = -1; // 0 = Ready to start, 1 = running, 2 = done
byte startColor = 2;
int distanceMurDepart = 0;
unsigned long allPurpousTimer = 0;
int tempsEnSecPourLeVirage = 3;


//mur
const int ROUGE_GAUCHE = 49; // Front gauche
const int VERTE_GAUCHE = 48; // Back gauche
const int ROUGE_DROITE = 49; // Front droite
const int VERTE_DROITE = 48; // Back droite

// Déclaration de classes pour les différentes composantes
Moteur moteur;
Sifflet sifflet;
CapteurCouleur capteurCouleur;
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
    capteurCouleur.setupCapteurCouleur();
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
                distanceMurDepart = ROBUS_ReadIR(3);
                moteur.avancerLigneDroite(0.2);
            }
            break;
        case 0: // Vers section 1
            if (ROBUS_IsBumper(LEFT) || ROBUS_IsBumper(RIGHT)) {

                // Faire le calcul du quart de circonférence en fonction de la couleur de départ

                int distanceRoueDroitePouces = 0;
                int distanceRoueGauchePouces = 0;

                if (startColor == 2) { // C'est le vert
                    distanceRoueDroitePouces = 1;
                    distanceRoueGauchePouces = 2;
                }

                float circonferenceRoueDroiteCM = (distanceRoueDroitePouces * 30.5 + 6) * 2 * PI;
                float circonferenceRoueGaucheCM = (distanceRoueGauchePouces * 30.5 - 6) * 2 * PI;

                Serial.print("Circonference droite : ");
                Serial.println(circonferenceRoueDroiteCM);
                Serial.print("Circonference gauche : ");
                Serial.println(circonferenceRoueGaucheCM);

                moteur.setTargetSpeedDroite(circonferenceRoueDroiteCM / 4 / tempsEnSecPourLeVirage);
                moteur.setTargetSpeedGauche(circonferenceRoueGaucheCM / 4 / tempsEnSecPourLeVirage);
                allPurpousTimer = millis();
                valEtat++;
            }
            break;
        case 1: // Fin du virage, arrive sur le tapis
            if (allPurpousTimer + tempsEnSecPourLeVirage * 1000 < millis() ) {
                Serial.println("Fin du virage");
                allPurpousTimer = 0;
                moteur.avancerLigneDroite(0.2);
                valEtat++;
            }
            break;
        case 2: // Commence le deuxieme virage
            if (ROBUS_ReadIR(3) < 100) {
                
                // Faire le calcul du quart de circonférence en fonction de la couleur de départ

                int distanceRoueDroitePouces = 0;
                int distanceRoueGauchePouces = 0;

                if (startColor == 2) { // C'est le vert
                    distanceRoueDroitePouces = 1;
                    distanceRoueGauchePouces = 2;
                }

                float circonferenceRoueDroiteCM = (distanceRoueDroitePouces * 30.5 + 6) * 2 * PI;
                float circonferenceRoueGaucheCM = (distanceRoueGauchePouces * 30.5 - 6) * 2 * PI;

                moteur.setTargetSpeedDroite(circonferenceRoueDroiteCM / 4 / tempsEnSecPourLeVirage);
                moteur.setTargetSpeedGauche(circonferenceRoueDroiteCM / 4 / tempsEnSecPourLeVirage);
                allPurpousTimer = millis();
                valEtat++;
            }
            break;
        case 3: // Même virage que le premier
            if (allPurpousTimer + tempsEnSecPourLeVirage * 1000 < millis() ) {
                moteur.avancerLigneDroite(0.2);
                valEtat++;
            }
            break;
        case 4: // On est arrivé pour faire tomber le verre

            // Dépendant de notre couleur, on check un des deux capteurs de distance d'APP1
            if (startColor == capteurCouleur.VERT && digitalRead(VERTE_GAUCHE) == LOW) {
                baton.batonSortieGauche();
                moteur.avancerLigneDroite();
                // On attends que le capteur est aligné avec l'arrière
                while(digitalRead(ROUGE_GAUCHE) == HIGH) {
                    delay(0);
                }
                delay(200);
                baton.batonRange();
                valEtat++;
            } else if (startColor == capteurCouleur.JAUNE && digitalRead(ROUGE_DROITE) == LOW) {
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
        case 5: // On se rend au suiveur de ligne
            if (capteurCouleur.BLANC == capteurCouleur.lireCouleur()) {
                valEtat++;
            }

            break;
        default: //Ready to start
            Serial.print("default");
            break;
    }
}