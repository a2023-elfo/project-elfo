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

void Debut(bool go)
{
  if (go == true)
  {
    MOTOR_SetSpeed(LEFT, 0.5);
    MOTOR_SetSpeed(RIGHT, 0.53);
  }
}

void ArreterMoteur()
{
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

int DetectionMur()
{
  int pin1 = 49;
  int pin2 = 53;
  if (digitalRead(pin1) == LOW || digitalRead(pin2) == LOW)
  {
    delay(100);
    ArreterMoteur();
    return 1;
  }
  else
  {
    return 0;
  }
}

bool Depart(uint8_t a, uint8_t b)
{
  int valeurambiant = analogRead(a);
  int valeur5kHz = analogRead(b);

  if (valeur5kHz > 500)
  {
    return true;
  }
  else
  {
    return false;
  }
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
   MOTOR_SetSpeed(MOTEUR_DROIT, SPEED_DROITE);

   while (NbPulseMoteurGauche < 6684 && NbPulseMoteurDroit < 6684)
   {
    if (NbPulseMoteurGauche < NbPulseMoteurDroit){
      
      SPEED_GAUCHE + 0.01;

    }
    else if(NbPulseMoteurGauche > NbPulseMoteurDroit){

      SPEED_DROITE + 0.010305;

    }
 
    NbPulseMoteurGauche = ENCODER_Read(MOTEUR_GAUCHE);
    NbPulseMoteurDroit =  ENCODER_Read(MOTEUR_DROIT);
    
   }
 }
MOTOR_SetSpeed(MOTEUR_GAUCHE, 0);
MOTOR_SetSpeed(MOTEUR_DROIT, 0);
}

void Case_0_nav()
{
  int ligne=0;

  while(ligne<9)
  {
    if(DetectionMur()==1)
    {
      tourner_90_droite();
      Avancer(1);
      if(DetectionMur()==1)
      {
        tourner_90_gauche();
        Avancer(2);
      }
      else
      {
        Avancer(1);
        tourner_90_gauche();
        Avancer(2);
      }
    }
    else
    {
      Avancer(2);
    }
  return;
  }

}
/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
  pinMode(49, INPUT);
  pinMode(53, INPUT);
  pinMode(48, INPUT);
  pinMode(52, INPUT);
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

bool depart = false;

void loop() {
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU


  // Si le bumpeur gauche est appuyé, tourne a gauche
  if (ROBUS_IsBumper(0)) {
     Case_0_nav();
  }

 /* while (depart != true)
  {
    depart = Depart(A0, A1);
  }

  DetectionMur();*/
  
}


