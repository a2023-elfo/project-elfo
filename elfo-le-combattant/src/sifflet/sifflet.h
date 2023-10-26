#include <LibRobus.h>

class Sifflet{
    private:
        const int PIN_AMBIANT = A6;
        const int PIN_5KHZ = A7;
        static const int soundAverageSize = 10;
        int soundAverage[soundAverageSize] = {0};
        int soundCounter = 0;
        long int firstSpikeTime = 0;
    public:
        void setupSifflet();
        bool lireSifflet();
};