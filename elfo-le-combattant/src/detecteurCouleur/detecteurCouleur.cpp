#include <detecteurCouleur/detecteurCouleur.h>

// charte des couleurs:
// rouge = r
// jaune = j 
// vert = v 
// bleu = e 
// blanc = j 
// noir et erreur = e
void detecteurCouleur::setup(){
    Serial.begin(9600);
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  }
}
char detecteurCouleur::getCouleur(){
    uint16_t r, g, b, c, colorTemp, lux;
  
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);

    if(r == 1 && g  == 0 && b  == 0){
        return 'r';
    }else if (r == 1 && g == 1 && b == 0){
        return 'j';
    }else if (r  == 0 && g == 1 && b  == 0){
        return 'v';
    }else if (r  == 0 && g  == 0 && b == 1){
        return 'b';
    }else if (r == 1 && g == 1 && b == 1){
        return 'w';
    }else{
        return 'e';
    }

      Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
};