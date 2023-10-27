#include <Arduino.h>
#include <math.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <SparkFunISL29125.h> //librairie du capteur de couleur
#include <Wire.h>

//couleurs à détecter
#define rouge = 1
#define vert = 2
#define bleu = 3
#define inconnu = 4

byte couleurappercue = 0;
byte capteCouleur(int);

int main(void)
{
}
// Declare sensor object
int RGB_sensor;

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Capteur initialisé!\n\r");
  }
}

void loop()
{
  // Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();

  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Red: "); Serial.println(red,HEX);
  Serial.print("Green: "); Serial.println(green,HEX);
  Serial.print("Blue: "); Serial.println(blue,HEX);
  Serial.println();
  delay(2000);
}