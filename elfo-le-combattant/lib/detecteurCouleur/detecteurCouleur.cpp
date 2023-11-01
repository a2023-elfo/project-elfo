#include <detecteurCouleur/detecteurCouleur.h>

// charte des couleurs:
// rouge = r
// jaune = j 
// vert = v 
// bleu = b 
// blanc = w 
// erreur = e
void detecteurCouleur::setup(){
    Serial.begin(9600);
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}
void detecteurCouleur::getCouleur(char& color ){
    uint16_t r, g, b, c, colorTemp, lux;
  
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);

    if(r == 1 && g  == 0 && b  == 0){
        color = 'r';
    }else if (r == 1 && g == 1 && b == 0){
        color = 'j';
    }else if (r  == 0 && g == 1 && b  == 0){
        color = 'v';
    }else if (r  == 0 && g  == 0 && b == 1){
        color = 'b';
    }else if (r == 1 && g == 1 && b == 1){
        color = 'w';
    }else{
        color = 'e';
    }

      Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
};