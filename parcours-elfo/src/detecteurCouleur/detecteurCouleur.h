#include <LibRobus.h>



class detecteurCouleur{
    private:
    int16_t r;
    int16_t g;
    int16_t b;
    uint8_t input;

    public:
    void setup();
    void getCouleur();

};