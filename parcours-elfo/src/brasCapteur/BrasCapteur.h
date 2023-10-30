#include <LibRobus.h>


class BrasEtCapteur{
    private:
        int inputPin = 0;
        int moteur = 0;
        int angle = 90;
        int distanceVoulu =0;
        bool brasActione = false;
        int delayCounter = 0;
    public:
        void setupBrasEtCapteur(uint8_t,uint8_t,uint8_t,int);
        void loopBrasEtCapteur();
};