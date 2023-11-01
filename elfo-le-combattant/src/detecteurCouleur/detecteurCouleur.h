#include <LibRobus.h>
#include <../lib/Grove_I2C_Color_Sensor_TCS3472-master/Adafruit_TCS34725.h>


class detecteurCouleur{

    Adafruit_TCS34725 tcs = Adafruit_TCS34725();
    
    public:

    static const char  ROUJE = 'r';
    static const char  JAUNE = 'j';
    static const char  VERT = 'v';
    static const char  BLEU = 'e';
    static const char  BLANC = 'j';
    static const char NOIR = 'e';

    void setup();
    char getCouleur();
    
};