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
#include <brasCapteur/BrasCapteur.h>


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

float vitesse;
int etat = 0; //0 = arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
#define KP = 0.0001
#define KI = 0.00002

BrasEtCapteur bras = BrasEtCapteur();

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
//Ajuste la vitesse de rotation des deux moteurs 
void avance()
{
  MOTOR_SetSpeed(RIGHT, vitesse);
  MOTOR_SetSpeed(LEFT, vitesse);
}

void recule()
{
  MOTOR_SetSpeed(RIGHT, -vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
}

void tourneDroit()
{
  MOTOR_SetSpeed(RIGHT, 0.5*vitesse);
  MOTOR_SetSpeed(LEFT, -0.5*vitesse);
}

void tourneGauche()
{
  MOTOR_SetSpeed(RIGHT, -0.5*vitesse);
  MOTOR_SetSpeed(LEFT, 0.5*vitesse);
}


/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
  bras.setupBrasEtCapteur(0,0,0,250);
  Serial.begin(9600);
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {
  bras.loopBrasEtCapteur();
  int test = ROBUS_ReadIR(0);
  Serial.println(test);
}