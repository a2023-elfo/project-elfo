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
RGB_sensor;

/* ****************************************************************************
Utility functions functions
**************************************************************************** */
byte capteCouleur(int rougeappercu, int vertappercu, int bleuappercu)
{
    if (rougeappercu >= 190 && vertappercu <= 20 && bleuappercu <= 50)
    {
        return rouge;
    }
    else if (rougeappercu <= 20 && vertappercu >= 90 && bleuappercu <= 60)
    {
        return vert;
    }
    else if (rougeappercu <= 20 && vertappercu <= 70 && bleuappercu >= 120)
    {
        return bleu;
    }
    else if (rougeappercu >= 200 && vertappercu >= 200 && bleuappercu <= 20)
    {
        return jaune;
    }
    else if (rougeappercu <= 30 && vertappercu <= 30 && bleuappercu <= 30)
    {
        return noir;
    }
    else if (rougeappercu >= 240 && vertappercu >= 240 && bleuappercu >= 240)
    {
        return blanc;
    }
    else
    {
        return inconnu;
    }
}

/* ****************************************************************************
Main functions
**************************************************************************** */

void setup() {
    // Initialize the serial communication
    Serial.begin(115200);

    // Initialize the ISL29125 with simple configuration so it starts sampling
    if (RGB_sensor.init())
    {
        Serial.println("Capteur initialisé!\n\r");
    }
}

void loop() {
    int rougeappercu = RGB_sensor.readRed();
    int vertappercu = RGB_sensor.readGreen();
    int bleuappercu = RGB_sensor.readBlue();

    byte couleur = capteCouleur(rougeappercu, vertappercu, bleuappercu);
    Serial.println("======================");
    Serial.print("Couleur : ");
    switch (couleur) {
        case rouge:
            Serial.println("Rouge");
            break;
        case vert:
            Serial.println("Vert");
            break;
        case bleu:
            Serial.println("Bleu");
            break;
        case jaune:
            Serial.println("Jaune");
            break;
        case noir:
            Serial.println("Noir");
            break;
        case blanc:
            Serial.println("Blanc");
            break;
        default:
            Serial.println("Inconnu");
            break;
    }
    delay(1000); // Add a delay to avoid excessive serial output
}