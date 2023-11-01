#include <shortcut/shortcut.h>

void Shortcut::shortcut(){
    unsigned int distanceMur = 500;
    unsigned int buffer = 25;
    if(ROBUS_ReadIR(3)<distanceMur+buffer){
        MOTOR_SetSpeed(RIGHT,0.5);
        MOTOR_SetSpeed(LEFT,0.25);
    }
    if(ROBUS_ReadIR(3)>distanceMur+buffer){
        MOTOR_SetSpeed(RIGHT,0.25);
        MOTOR_SetSpeed(LEFT,0.5);
    }
    else{
        MOTOR_SetSpeed(RIGHT,0.25);
        MOTOR_SetSpeed(LEFT,0.25);
    }
}
