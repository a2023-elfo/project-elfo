#include "sifflet/sifflet.h"

void Sifflet::setupSifflet() {
    pinMode(PIN_5KHZ, INPUT);

};

bool Sifflet::lireSifflet(){
    int value5khz = analogRead(PIN_5KHZ);

    delay(25);
    //comparaison du son present a la moyenne du son
    if (soundCounter > soundAverageSize) {
        //fait une moyenne du son
        int sum = 0;
        for (int i = 0; i < (int)soundAverageSize; i++) {
            sum += soundAverage[i];
        }
        float average = sum / soundAverageSize;
        //verifie la longueur du spike
        if (value5khz > (average + 60)) {
            if (firstSpikeTime == 0) {
                firstSpikeTime = millis();
            }
            else {
                long int timeSinceSpike = millis() - firstSpikeTime;
                if (timeSinceSpike > 1000) {
                    Serial.println("Sifflet!");
                    return true;
                }
            }
        }
        else {
            firstSpikeTime = 0;
            soundAverage[soundCounter%soundAverageSize] = value5khz;
            soundCounter++;
        }
    } else {
        soundAverage[soundCounter%soundAverageSize] = value5khz;
        soundCounter++;
    }

    return false;
}