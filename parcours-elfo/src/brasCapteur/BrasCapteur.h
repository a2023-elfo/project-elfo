#include <LibRobus.h>


class BrasEtCapteur{
    private:
        int inputPin = 0;
        int moteur = 0;
        int angle = 90;
    public:
        void setupCapteurBras(uint8_t,uint8_t,uint8_t);
        void loopCapteurBras();
};