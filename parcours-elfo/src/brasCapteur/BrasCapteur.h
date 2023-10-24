#include <LibRobus.h>


class BrasEtCapteur{
    private:
        int inputPin;
        int motorOutputPin;
        int moteurMouvement;
        int moteur = 0;
    public:
        void setupCapteurBras();
        void loopCapteurBras();
};