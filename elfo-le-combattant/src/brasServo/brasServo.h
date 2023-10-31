#include <LibRobus.h>


class BrasServo {
    private:
        int inputPin = 0;
        int lastCommandTime = 0;
        static const int commandDelay = 100;
      
    public:
        void setupBrasServo(uint8_t pin);
        void batonSortieGauche();
        void batonSortieDroit();
        void batonRange();
        // Fonction qui doit être appelé pour assurer le bon fonctionnement
        void batonUpdate();
};