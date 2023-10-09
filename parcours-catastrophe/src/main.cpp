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


/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void maFonction(){
  // code
  
}
void tourner_90_gauche ()
{
  ENCODER_Reset(MOTEUR_GAUCHE);
  ENCODER_Reset(MOTEUR_DROIT);

  while ((ENCODER_Read(MOTEUR_GAUCHE) <1800) && (ENCODER_Read(MOTEUR_DROIT)<1800))
  {
      MOTOR_SetSpeed(MOTEUR_GAUCHE, +SPEED_GAUCHE);
      MOTOR_SetSpeed(MOTEUR_DROIT, -SPEED_DROITE);
      
  }
    
  MOTOR_SetSpeed(MOTEUR_GAUCHE, 0);
  MOTOR_SetSpeed(MOTEUR_DROIT, 0);
}

void tourner_90_droite ()
{
  ENCODER_Reset(MOTEUR_GAUCHE);
  ENCODER_Reset(MOTEUR_DROIT);

  while ((ENCODER_Read(MOTEUR_GAUCHE)<1800) && (ENCODER_Read(MOTEUR_DROIT)<1800))
  {
      MOTOR_SetSpeed(MOTEUR_GAUCHE, -SPEED_GAUCHE);
      MOTOR_SetSpeed(MOTEUR_DROIT, +SPEED_DROITE);
      
  }
  MOTOR_SetSpeed(MOTEUR_GAUCHE, 0);
  MOTOR_SetSpeed(MOTEUR_DROIT, 0);

}

void Avancer(int NbCycles){

  int NbPulseMoteurGauche = ENCODER_Read(MOTEUR_GAUCHE);
  int NbPulseMoteurDroit =  ENCODER_Read(MOTEUR_DROIT);
 
  

 for(int i = 0; i < NbCycles; i++){
  ENCODER_Reset(MOTEUR_DROIT);
  ENCODER_Reset(MOTEUR_GAUCHE);
  NbPulseMoteurDroit = ENCODER_Read(MOTEUR_DROIT);
  NbPulseMoteurGauche = ENCODER_Read(MOTEUR_GAUCHE);

   MOTOR_SetSpeed(MOTEUR_GAUCHE, SPEED_GAUCHE);
   MOTOR_SetSpeed(MOTEUR_DROIT, SPEED_DROIT);

   while (NbPulseMoteurGauche < 6684 && NbPulseMoteurDroit < 6684)
   {
    if (NbPulseMoteurGauche < NbPulseMoteurDroit){
      
      SPEED_GAUCHE + 0.01;

    }
    else if(NbPulseMoteurGauche > NbPulseMoteurDroit){

      SPEED_DROIT + 0.010305;

    }
 
    NbPulseMoteurGauche = ENCODER_Read(MOTEUR_GAUCHE);
    NbPulseMoteurDroit =  ENCODER_Read(MOTEUR_DROIT);
    
   }
 }
MOTOR_SetSpeed(MOTEUR_GAUCHE, 0);
MOTOR_SetSpeed(MOTEUR_DROIT, 0);
}
/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
  
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU

  // Si le bumper avant est appuyé, switch les moteurs au reculon
  if (ROBUS_IsBumper(2)) {
    MOTOR_SetSpeed(0, -SPEED_GAUCHE);
    MOTOR_SetSpeed(1, -SPEED_DROITE);
  }

  // Si le bumper arrière est appuyé, switch les moteurs au reculon
  if (ROBUS_IsBumper(3)) {
    MOTOR_SetSpeed(0, SPEED_GAUCHE);
    MOTOR_SetSpeed(1, SPEED_DROITE);
  }

  // Si le bumpeur droit est appuyé, tourne a gauche
  if (ROBUS_IsBumper(1)) {
    tourner_90_gauche();
  }

  // Si le bumpeur gauche est appuyé, tourne a gauche
  if (ROBUS_IsBumper(0)) {
     tourner_90_droite();
  }

}


