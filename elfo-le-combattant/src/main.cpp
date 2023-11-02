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
const float defaultVitesseL = 0.265;
const float turningSpeed = 0.3;
float vitesseR, vitesseL;

byte valEtat = 0; // 0 = Ready to start, 1 = running, 2 = done

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

// Debug variables
#define ELFO_DEBUG true
long int lastDebugPrintTime = 0;

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
  delay(150);
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

   pinMode(A2, INPUT);

    // Set default speeds

    vitesseL = defaultVitesseL;
    vitesseR = defaultVitesseR;
}

void avance(){
    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, 0.2);


}

void dash(){
     MOTOR_SetSpeed(LEFT, 0.4);
    MOTOR_SetSpeed(RIGHT, 0.4);
    

}

void dashdroit(){
    MOTOR_SetSpeed(LEFT, 0.5);
    MOTOR_SetSpeed(RIGHT, 0.05);
    

}

void printDebugInfo() {
    int msDelay = 250;
    if (millis() > lastDebugPrintTime) {
        Serial.println("This is a debug call");

        lastDebugPrintTime += msDelay;
    }
    
}


void correction_shortcut_d(){

    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, 0.05);
    
    
    
}

void correction_shortcut_d_lente(){

    MOTOR_SetSpeed(LEFT, 0.3);
    MOTOR_SetSpeed(RIGHT, 0.2);
    
    
    
}


void correction_shortcut_g(){

    MOTOR_SetSpeed(LEFT, 0.05);
    MOTOR_SetSpeed(RIGHT, 0.3);
    
    


    
}
//ne pas utiliser ce suivre_ligne
void suivre_ligne(){

    //faudra rajouter un etat dans la fonction main pour cette fonction avec le capteur de couleur
    //faudra aussi rajouter fonction de contournement
    
    int voltage = analogRead(A2);
    
    if (voltage >= 980 && voltage <= 1024){

        dash();
    
    
    
    }
    if (voltage >= 0 && voltage <= 20){
    //Aucun
    //detecte du noir partout, ne va pas arriver
    arret();
    }
    if (voltage >= 560 && voltage <= 600){
    //1
    correction_shortcut_d();
    
    
    //ligne sur le 1 et 2, ne va pas arriver
    }
    if (voltage >= 270 && voltage <= 310){
    //2
    arret();
    //ligne sur le 1 et 3, ne va pas arriver
    }
    if (voltage >= 125 && voltage <= 165){
    //3
    correction_shortcut_g();
    
    
    //ligne sur le 2 et 3, ne vas pas arrivé
    }
    if (voltage >= 860 && voltage <= 900){
    //1,2
    //ligne sur le 1, tourne a droite
    
    correction_shortcut_g();
    
    
    
    }
    if (voltage >= 420 && voltage <= 460){
    //2,3
    //ligne sur le 3, tourne a gauche
    
    correction_shortcut_d();
    
    
    
    
    }
    if (voltage >= 710 && voltage <= 755){
    //1,3
    // ligne sur le 2, ce qu on veut
    // on veut que le robot aille droit
    delay(50);
    avance();

        
   

    }
  

}



void shortcut(){

    


    if(ROBUS_ReadIR(3) < 560 &&  ROBUS_ReadIR(3) > 200){
        correction_shortcut_d();
               
    }   
    
    
    if(ROBUS_ReadIR(3) > 640){
        correction_shortcut_g();
        
        
    }
    if(ROBUS_ReadIR(3) >= 560 && ROBUS_ReadIR(3) <= 640){
        dash();
        
        
    }

    if (ROBUS_ReadIR(3) <= 200){

        correction_shortcut_d();
    }


    
    
}

void shortcut_complet_jump(){
    
avance();// remplacer par avancer pid
if(ROBUS_ReadIR(3) < 100){
    MOTOR_SetSpeed(LEFT, 0.6);
    MOTOR_SetSpeed(RIGHT, -0.2);
    delay(350);
    avance();
    if (ROBUS_ReadIR(3) > 400){
        shortcut();
        if (ROBUS_ReadIR(3) < 25 && ROBUS_ReadIR(0) < 25)
            {/*hard coder le reste du virage ici apres avoir sorti du shortcut*/
            }
    }

}
}
void loop() {

shortcut_complet_jump();

}