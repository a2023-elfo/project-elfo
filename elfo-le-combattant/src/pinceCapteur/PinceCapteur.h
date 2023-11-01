#include <LibRobus.h>

class Pince{
    private:
        int inputPin = 1;
      
    public:
        void setupPince(uint8_t pin);
        void pinceOuvert();
        void pinceFerme();
};