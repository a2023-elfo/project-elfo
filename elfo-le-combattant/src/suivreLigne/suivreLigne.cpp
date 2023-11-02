#include <suivreLigne/suivreLigne.h>
 
void avance(){
    MOTOR_SetSpeed(LEFT, 0.3);
    MOTOR_SetSpeed(RIGHT, 0.3);
}
void arret(){
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}
void correctiong(){
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0.3);
}
void correctiond(){    
    MOTOR_SetSpeed(LEFT, 0.3);
    MOTOR_SetSpeed(RIGHT, 0);
    }
void SuivreLigne ::suivreLigne(){
 
    //faudra rajouter un etat dans la fonction main pour cette fonction avec le capteur de couleur
    //faudra aussi rajouter fonction de contournement
    
    int voltage = analogRead(A2);
    
    if (voltage >= 980 && voltage <= 1024){
        avance();
    //1, 2, 3
    //detecte du blanc partout
    }
    if (voltage >= 0 && voltage <= 20){
    //Aucun
    //detecte du noir partout, ne va pas arriver
        arret();
    }
    if (voltage >= 560 && voltage <= 600){
    //1
        correctiond();
    //ligne sur le 1 et 2, ne va pas arriver
    }
    if (voltage >= 270 && voltage <= 310){
    //2
        MOTOR_SetSpeed(LEFT, 0.2);
        delay(1000);
    //ligne sur le 1 et 3, ne va pas arriver
    }
    if (voltage >= 125 && voltage <= 165){
    //3
        correctiong();
    //ligne sur le 2 et 3, ne vas pas arrivé
    }
    if (voltage >= 860 && voltage <= 900){
    //ligne sur le 3, tourne a gauche
        correctiong();
    }
    if (voltage >= 420 && voltage <= 460){
    //2,3
    //ligne sur le 3, tourne a gauche
        correctiond();
    }
    if (voltage >= 710 && voltage <= 755){
    //1,3
    // ligne sur le 2, ce qu on veut
    // on veut que le robot aille droit
        avance();
    }
}