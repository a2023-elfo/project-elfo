/*
Projet: Capteur de couleur
Equipe: ELFO
Auteurs: Anahi
Description: either the code is done or i die
Date: 28 octobre 2023
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <Arduino.h>
#include <math.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <SparkFunISL29125.h>

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
#define rouge 1
#define vert 2
#define bleu 3
#define jaune 4
#define noir 5
#define blanc 6
#define inconnu 7

byte capteCouleur(int);
int RGB_sensor;

/* ****************************************************************************
Utility functions functions
**************************************************************************** */
byte capteCouleur(int couleur, int rougeappercu, int vertappercu, int bleuappercu)
{
    couleur = 0;
    rougeappercu = RGB_sensor.readRed();
    vertappercu = RGB_sensor.readGreen();
    bleuappercu = RGB_sensor.readBlue();

    if (rougeappercu >= 190 && vertappercu <= 20 && bleuappercu <= 50)
    {
        couleur = rouge;
        if (rougeappercu <= 20 && vertappercu >= 90 && bleuappercu <= 60)
    {
        couleur = vert;
    }

    if (rougeappercu <= 20 && vertappercu <= 70 && bleuappercu >= 120)
    {
        couleur = bleu;
    }

    if (rougeappercu >= 200 && vertappercu >= 200 && bleuappercu <= 20)
    {
        couleur = jaune;
    }
    
    if (rougeappercu <= 30 && vertappercu <= 30 && bleuappercu <= 30)
    {
        couleur = noir;
    }
    
    if (rougeappercu >= 240 && vertappercu >= 240 && bleuappercu >= 240)
    {
        couleur = blanc;
    }
    
    else
    {
        couleur = inconnu;
    }
    
    }
    return couleur;
}

/* ****************************************************************************
Main functions
**************************************************************************** */

void setup() {

    // Initialiser le capteur
  Serial.begin(115200);

  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Capteur initialisé!\n\r");
  }
}

void loop() 
{

}