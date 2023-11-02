#include <LibRobus.h>
class Moteur {
    private:
        const float wheelCircumference = 23.938936; // cm
        const int ticksPerRotation = 3200;
        const float maxSpeedCMparS = 75;

        // PID Variables 
        const float kp = 0.005;
        const float ki = 0.00025;
        const float kd = 0.0015;
        float integralL;
        float previousErrorL;
        int previousTimeL;
        float integralR;
        float previousErrorR;
        int previousTimeR;

        // Max speed, acceleration et deceleration
        const float acceleration = 0.093;
        const float deceleration = 0.22;

        // Current targets
        float targetSpeedL = 0;
        float targetSpeedR = 0;
        float currentSpeedL = 0;
        float currentSpeedR = 0;
        float currentCommandL = 0;
        float currentCommandR = 0;

        // PID functions
        void resetVariablesMoteur(int moteur = 3);
        void uniMoteurPID(int moteur);
        float calculateSpeed(float vitesse);
        

    public:
        void setupMoteur();
        void avancerLigneDroite(float vitesse = 0.5);
        void moteurSetSpeedGauche(float vitesse);
        void moteurSetSpeedDroite(float vitesse);
        // Fonction de maintenance du PID demandé
        void moteurUpdate();
        // Fonction pour une décélération jusqu'à l'arrêt
        void moteurArret();
        // Fonction pour arrêt d'urgence
        void moteurArretUrgence();
        float getDistanceParcourue(int moteur);
        float getDistanceVitesseEnCMparS(float vitesse);
};