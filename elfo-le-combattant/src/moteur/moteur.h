#include <LibRobus.h>
class Moteur {
    private:
        const float wheelCircumference = 23.9389360204; // cm
        const int ticksPerRotation = 3200;

        // PID Variables 
        const float kp = 0.05;
        const float ki = 0.002;
        const float kd = 0.01;
        float integralL;
        float previousErrorL;
        int previousTimeL;
        float integralR;
        float previousErrorR;
        int previousTimeR;

        // Max speed, acceleration et deceleration
        const float acceleration = 0.1;
        const float deceleration = 0.4;

        // Current targets
        float targetSpeedL = 0;
        float targetSpeedR = 0;
        float currentSpeedL = 0;
        float currentSpeedR = 0;

        // PID functions
        void resetVariablesMoteur(int moteur = 3);
        void uniMoteurPID(int moteur);
        

    public:
        void setupMoteur();
        void avancerLigneDroite(float vitesse = 0.7);
        void moteurSetSpeedGauche(float vitesse);
        void moteurSetSpeedDroite(float vitesse);
        // Fonction de maintenance du PID demandé
        void moteurPID();
        // Fonction pour une décélération jusqu'à l'arrêt
        void moteurArret();
        // Fonction pour arrêt d'urgence
        void moteurArretUrgence();
        float getDistanceParcourue(int moteur);
};