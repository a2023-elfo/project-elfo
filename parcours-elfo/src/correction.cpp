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
#include <librobus.h>
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */

float vitesseR = 0.20;
float vitesseL = 0.18;

int etat = 0; //0 = arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
#define KP 0.001
/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

//Envoyer la commande par l'utilisateur
void avance(){
  MOTOR_SetSpeed(RIGHT, vitesseR);
  MOTOR_SetSpeed(LEFT, vitesseL);
}

void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
}

void algorithme_avance(){
  MOTOR_SetSpeed(RIGHT, vitesseR);
  MOTOR_SetSpeed(LEFT, vitesseL);

  Serial.println("vitesseR");
  Serial.println(vitesseR);
  Serial.println("vitesseL");
  Serial.println(vitesseL);

  delay(50);
  int leftPulse = ENCODER_ReadReset(LEFT);
  int rightPulse = ENCODER_ReadReset(RIGHT);

  int error = rightPulse - leftPulse;
  float correction = KP * error;

  Serial.println("correction");
  Serial.println(correction);

  vitesseL = vitesseL + correction;
  vitesseR = vitesseR - correction;
}

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

/* ****************************************************************************

Fonctions d'initialisation (setup)

**************************************************************************** */

void setup(){
  BoardInit();
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
void loop()

{
  algorithme_avance();
}