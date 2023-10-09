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
const int soundAverageSize = 10;
int soundAverage[soundAverageSize] = {0};
int soundCounter = 0;
long int firstSpikeTime = 0;

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
int siflet(){
    int valueAmb = analogRead(PIN_AMBIANT);
    int value5khz = analogRead(PIN_5KHZ);
    
    delay(50);

    if (soundCounter > soundAverageSize) {
      int sum = 0;
      for (int i = 0; i < (int)soundAverageSize; i++) {
        sum += soundAverage[i];
      }
      float average = sum / soundAverageSize;

      if (value5khz > (average + 100)) {
        if (firstSpikeTime == 0) {
          firstSpikeTime = millis();
        } else {
          long int timeSinceSpike = millis() - firstSpikeTime;
          if (timeSinceSpike > 3000) {
            Serial.println("Ca fait longtemps qu'on a du 5khz");
            delay(1000);
          }
        }

        Serial.print("Average value ");
        Serial.println(average);
        Serial.print("current reading value ");
        Serial.println(value5khz);
      } else {
        firstSpikeTime = 0;
        soundAverage[soundCounter%soundAverageSize] = value5khz;
        soundCounter++;
      }

    } else {
      soundAverage[soundCounter%soundAverageSize] = value5khz;
      soundCounter++;
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
  MOTOR_SetSpeed(0, SPEED_GAUCHE);
  MOTOR_SetSpeed(1, SPEED_DROITE);
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
    MOTOR_SetSpeed(MOTEUR_GAUCHE, 0);
    MOTOR_SetSpeed(1, 0.25);
  }

  // Si le bumpeur gauche est appuyé, tourne a gauche
  if (ROBUS_IsBumper(0)) {
    MOTOR_SetSpeed(MOTEUR_GAUCHE, 0.25);
    MOTOR_SetSpeed(1, 0);
  }

}