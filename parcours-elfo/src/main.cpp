/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS



/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces
//float SPEED_GAUCHE = 0.485;
//float SPEED_DROITE = 0.5;

float SPEED_GAUCHE = 0.485;
float SPEED_DROITE = 0.5;
uint8_t MOTEUR_GAUCHE = 0;
uint8_t MOTEUR_DROIT = 1;
int PIN_AMBIANT = A6;
int PIN_5KHZ = A7;

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void maFonction(){
  // code
}


/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
  MOTOR_SetSpeed(0, SPEED_GAUCHE);
  MOTOR_SetSpeed(1, SPEED_DROITE);
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {
    
    int valueAmb = analogRead(PIN_AMBIANT);
    int value5khz = analogRead(PIN_5KHZ);
    Serial.print("Ambiant :");
    Serial.print(valueAmb);
    Serial.print("\t");
    Serial.print("5KHz : ");
    Serial.println(value5khz);
    delay(250);
     if(value5khz >= valueAmb){
      Serial.print("Avance\n");
    };

 }

