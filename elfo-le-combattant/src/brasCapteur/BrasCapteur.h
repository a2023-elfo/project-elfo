#include <LibRobus.h>


class BrasEtCapteur{
    private:
        int inputPin = 0;
      
    public:
        void setupBrasEtCapteur(uint8_t pin);
        void batonSortieGauche();
        void batonSortieDroit();
        void batonRange();
};