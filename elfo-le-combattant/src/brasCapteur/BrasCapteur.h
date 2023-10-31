#include <LibRobus.h>


class BrasEtCapteur{
    private:
        int inputPin = 0;
        int lastCommandTime = 0;
        static const int commandDelay = 100;
      
    public:
        void setupBrasEtCapteur(uint8_t pin);
        void batonSortieGauche();
        void batonSortieDroit();
        void batonRange();
        // Fonction qui doit être appelé pour assurer le bon fonctionnement
        void batonUpdate();
};