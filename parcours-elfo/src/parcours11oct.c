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
int valEtat = 0; //0 = arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche

//position
int compteur_ligne = 0;
int compteur_colonne = 0;

//avance50
#define KP 0.00002
int leftPulse = 0;
int rightPulse = 0;

//mur
const int ROUGE = 49;
const int VERTE = 48;

//siflet
int PIN_AMBIANT = A6;
int PIN_5KHZ = A7;
const int soundAverageSize = 10;
int soundAverage[soundAverageSize] = {0};
int soundCounter = 0;
long int firstSpikeTime = 0;
/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void avance(){
    MOTOR_SetSpeed(RIGHT,vitesseR);
    MOTOR_SetSpeed(LEFT,vitesseL);
}
void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
}

bool faceAuMur() {
  int sensorGauche = digitalRead(ROUGE);
  int sensorDroit = digitalRead(VERTE);

  return (sensorGauche == LOW || sensorDroit == LOW);
}

void avance50(){
  while(ENCODER_Read(LEFT) < 6884 && ENCODER_Read(RIGHT)<6884){
    MOTOR_SetSpeed(RIGHT, vitesseR);
    MOTOR_SetSpeed(LEFT, vitesseL);

    //int error = abs(rightPulse - leftPulse);
    //float correction = KP * error;
      if(leftPulse < rightPulse){
        vitesseL = vitesseL + KP;
      }
      else if(leftPulse > rightPulse){
        vitesseR = vitesseR + KP;
      }
    leftPulse = ENCODER_Read(LEFT);
    rightPulse = ENCODER_Read(RIGHT);
    }
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
}

void tournerDROIT90(){
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);  

    while((ENCODER_Read(RIGHT) < 1963) && (ENCODER_Read(LEFT) < 1963 )){      
        MOTOR_SetSpeed(RIGHT, -0.5*vitesseR);
        MOTOR_SetSpeed(LEFT, 0.5*vitesseL);  
    }
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
}

void tournerGAUCHE90(){
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    while((ENCODER_Read(RIGHT) < 1963) && (ENCODER_Read(LEFT) < 1963 )){      
      MOTOR_SetSpeed(RIGHT, 0.5*vitesseR);
      MOTOR_SetSpeed(LEFT, -0.5*vitesseL);      
    }
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
}

void colonne_0(){
 if (faceAuMur() ){
  tournerDROIT90();
  avance50();
  tournerGAUCHE90();
  compteur_colonne = 1;
 }
 else{
  avance50();
  avance50();
  compteur_ligne = compteur_ligne + 2;
 }
}

void colonne_1(){
    //vers le nord
    if (faceAuMur() ){
        tournerDROIT90(); 
        //est
        if(faceAuMur()){
            tournerGAUCHE90();
            tournerGAUCHE90();
            //ouest
            if(faceAuMur()){
                tournerGAUCHE90();
                avance50();
                avance50();
                //sud
                if(faceAuMur()){
                    tournerDROIT90();
                    //ouest
                    if(faceAuMur()){
                        tournerGAUCHE90();
                        tournerGAUCHE90();
                        avance50();
                        tournerGAUCHE90();
                        compteur_colonne = 2;
                        //nord
                    }
                }
                else{
                    avance50();
                    avance50();
                    //sud
                    if(faceAuMur()){
                        tournerDROIT90();
                        //ouest
                        if(faceAuMur()){
                            tournerGAUCHE90();
                            tournerGAUCHE90();
                            avance50();
                            tournerGAUCHE90();
                            compteur_colonne = 2;
                            //nord
                        }
                    }
                }
            else{
                avance50();
                tournerDROIT90();
                compteur_colonne = 0;
                //nord
            }
            
        }
        else{
            avance50();
            tournerGAUCHE90();
            compteur_colonne = 2;
            //nord
        }
    }

    else{
        avance50();
        avance50();
        compteur_colonne = 1;
        //nord
    }
    
}
}

void colonne_2 (){
 if (faceAuMur()){
  tournerGAUCHE90();
  avance50();
  tournerDROIT90();
  compteur_colonne = 1;
 }
 else {
  avance50();
  avance50();
  compteur_ligne = compteur_ligne + 2;
 }
}

int sifflet(){
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
                    Serial.println("go");
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
}

/* ****************************************************************************

Fonctions d'initialisation (setup)

**************************************************************************** */

void setup(){
  BoardInit();
pinMode(ROUGE, INPUT);
pinMode(VERTE, INPUT);
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
void loop(){
  if (compteur_colonne==0)
  {
    colonne_0();
  }
  else if(compteur_colonne== 1){
    //colonne_1();
  }
  else if(compteur_colonne== 2){
    colonne_2();
  }
  delay(1500);
}