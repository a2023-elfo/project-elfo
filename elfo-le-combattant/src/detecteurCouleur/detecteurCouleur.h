#include <LibRobus.h>
#include <../lib/Grove_I2C_Color_Sensor_TCS3472-master/Adafruit_TCS34725.h>


class detecteurCouleur{

    Adafruit_TCS34725 tcs = Adafruit_TCS34725();
    
    public:
    void setup();
    char getCouleur();
    
};