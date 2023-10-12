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
const float defaultVitesseL = 0.25;
const float turningSpeed = 0.2;
float vitesseR, vitesseL;

byte valEtat = 0; // 0 = Ready to start, 1 = running, 2 = done

const int MAGIC_NUMBER_50CM = 6583;
const int MAGIC_NUMBER_TURNING_LEFT = 1769;
const int MAGIC_NUMBER_TURNING_RIGHT = 1995;
const int SPEED_BUFFER = 75;

//position
byte compteur_colonne = 1;
byte compteur_ligne = 0;
int orientation_degree = 90; // Commence face au nord
byte coordsColonneVisite[30] = {99};
byte coordsLigneVisite[30] = {99};

byte casesParcouru = 0;

#define NORD 90
#define SUD 270
#define EST 0
#define WEST 180


// Offset correction
#define KP 0.00002

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

// Donne l'orientation du robot en angle toujours positive
// entre 0 et 359
int getOrientationDegrees() {
    return abs(orientation_degree % 360);
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
  delay(500);
}

void avance50cm() {
    resetMotorVariables();

    int leftPulse, rightPulse;
    // Reset speeds
    MOTOR_SetSpeed(RIGHT, vitesseR);
    MOTOR_SetSpeed(LEFT, vitesseL);

    while(ENCODER_Read(LEFT) < MAGIC_NUMBER_50CM && ENCODER_Read(RIGHT) < MAGIC_NUMBER_50CM) {
        leftPulse = ENCODER_Read(LEFT);
        rightPulse = ENCODER_Read(RIGHT);

        // Le moteur en retard accéllère
        if (leftPulse + SPEED_BUFFER < rightPulse) {
            vitesseL = vitesseL + KP;
            MOTOR_SetSpeed(LEFT, vitesseL);

        } else if (leftPulse > rightPulse + SPEED_BUFFER){
            vitesseR = vitesseR + KP;
            MOTOR_SetSpeed(RIGHT, vitesseR);
        }
    }

    // Compute la nouvelle position
    coordsColonneVisite[casesParcouru] = compteur_colonne;
    coordsLigneVisite[casesParcouru] = compteur_ligne;
    casesParcouru++;

    switch (getOrientationDegrees())
    {
        case NORD:
            compteur_ligne++;
            break;
        case EST:
            compteur_colonne++;
            break;
        case SUD:
            compteur_ligne--;
            break;
        case WEST:
            compteur_colonne--;
            break;
        
        default:
            // wtf
            break;
    }

    arret();
}

void tournerGauche90() {
    resetMotorVariables();

    MOTOR_SetSpeed(RIGHT, turningSpeed);
    MOTOR_SetSpeed(LEFT, -turningSpeed);      
    while((ENCODER_Read(RIGHT) < MAGIC_NUMBER_TURNING_LEFT) && (ENCODER_Read(LEFT) < MAGIC_NUMBER_TURNING_LEFT )){      
    }

    orientation_degree += 90;

    arret();
}

void tournerDroit90() {
    resetMotorVariables();

    MOTOR_SetSpeed(RIGHT, -turningSpeed);
    MOTOR_SetSpeed(LEFT, turningSpeed);  

    while((ENCODER_Read(RIGHT) < MAGIC_NUMBER_TURNING_RIGHT) && (ENCODER_Read(LEFT) < MAGIC_NUMBER_TURNING_RIGHT )){
    }

    orientation_degree -= 90;
    
    arret();
}

void navigation_depart() {
    tournerDroit90();
    avance50cm();
    tournerGauche90();
    avance50cm();
    avance50cm();
}

void navigation() {
    // Navigation qui tourne vers la gauche lorsqu'on est en questionnement
    if (!faceAuMur()) {

        // Empêcher l'aller ou on était la dernière fois
        byte computeLigne = compteur_ligne;
        byte computeColonne = compteur_colonne;

        switch (getOrientationDegrees())
        {
        case NORD:
            computeLigne++;
            break;
        case EST:
            computeColonne++;
            break;
        case SUD:
            computeLigne--;
            break;
        case WEST:
            computeColonne--;
            break;
        }

        bool faceDejaVisite = false;

        for (int i = 0; i < casesParcouru; i++) {
            if (computeColonne == coordsColonneVisite[i] && computeLigne == coordsLigneVisite[i]) {
                faceDejaVisite = true;
                break;
            }
        }

        if (faceDejaVisite) {
            tournerGauche90();
        }   // On check si on est pas sur le bord d'aller out of bounds
        else if (getOrientationDegrees() == EST && compteur_colonne == 2) {
            tournerGauche90();
        } else if (getOrientationDegrees() == WEST && compteur_colonne == 0) {
            tournerDroit90();
        } else {
            avance50cm();
        }
    } else {
        // On est devant un mur, check orientation et colonne pour poursuivre
        switch (getOrientationDegrees())
        {
        case NORD:
            if (compteur_colonne == 0) { // Face au nord, colonne de gauche
                tournerDroit90();
            } else { // Colonne droite ou centre, ya un mur
                tournerGauche90();
            }
            break;
        
        case EST:
            // When in doubt go left
            tournerGauche90();
            break;

        case SUD:
            // we spin si c'était a droite
            tournerGauche90();
            break;

        case WEST: // Same as nord, dumb logic for now
            if (compteur_colonne == 0) { // colonne de gauche
                tournerDroit90();
            } else { // Colonne droite ou centre, ya un mur
                tournerGauche90();
            }
            break;
        
        default:
            // Scream, you shouldn't be here
            AX_BuzzerON();
            break;
        }
    }
    
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

    // Ajout des valeurs illégales
    coordsColonneVisite[casesParcouru] = 1;
    coordsLigneVisite[casesParcouru] = 1;
    casesParcouru++;

    coordsColonneVisite[casesParcouru] = 1;
    coordsLigneVisite[casesParcouru] = 3;
    casesParcouru++;

    coordsColonneVisite[casesParcouru] = 1;
    coordsLigneVisite[casesParcouru] = 7;
    casesParcouru++;

    coordsColonneVisite[casesParcouru] = 1;
    coordsLigneVisite[casesParcouru] = 9;
    casesParcouru++;
}

void printDebugInfo() {
    Serial.print("I face ");
    Serial.println(getOrientationDegrees());
    Serial.print("Ligne : ");
    Serial.println(compteur_ligne);
    Serial.print("Colonne : ");
    Serial.println(compteur_colonne);
}

void speen() {
   tournerGauche90();
   tournerGauche90();
   tournerGauche90();
   tournerGauche90();
   delay(1000);
}

void loop() {
    switch (valEtat)
    {
        case 1: // Navigation
            if (compteur_ligne < 3) { // Fonction de démarrage pour le cul de sac potetiel
                navigation_depart();
            } else if (compteur_ligne < 9) {
                navigation();
                printDebugInfo();
            } else {
                valEtat = 2; // We finished!
            }
            break;

        case 2: // We win!
            speen();
            break;
        
        default: //Ready to start
            if (sifflet() || ROBUS_IsBumper(3)) { // On entend le sifflet, ca part!
                valEtat = 1;
            }
            break;
    }
}