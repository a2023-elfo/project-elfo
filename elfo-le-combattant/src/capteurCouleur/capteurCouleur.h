#include <SparkFunISL29125.h>

class CapteurCouleur {
    private:
        SFE_ISL29125 RGB_sensor;

    public:
        static const uint8_t ROUGE = 1;
        static const uint8_t VERT = 2;
        static const uint8_t BLEU = 3;
        static const uint8_t JAUNE = 4;
        static const uint8_t NOIR = 5;
        static const uint8_t BLANC = 6;
        static const uint8_t INCONNU = 7;
        void setupCapteurCouleur();
        uint8_t lireCouleur();
        String couleurToString(uint8_t couleur);
};