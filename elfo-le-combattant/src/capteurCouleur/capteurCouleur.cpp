#include <capteurCouleur/capteurCouleur.h>

void CapteurCouleur::setupCapteurCouleur() {
    tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
    
    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("Cry, no color for you");
    }
}

uint8_t CapteurCouleur::lireCouleur() {
    uint16_t r, g, b, c, colorTemp, lux;
    
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);

    // RED was around 44800
    // YELLOW was around 2000
    // GREEN was around 4500
    // BLUE was around 2600+
    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.println(" K");

    // Return the most prominent color as a uint8_t
    // use color temp in decimal base
    if (colorTemp > 0 && colorTemp < 2150) {
        return JAUNE;
    } else if (colorTemp > 2150 && colorTemp < 3200) {
        return BLANC;
    } else if (colorTemp > 3200 && colorTemp < 6000) {
        return VERT;
    } else if (colorTemp > 6000 && colorTemp < 20000) {
        return VERT;
    } else if (colorTemp > 20000) {
        return ROUGE;
    } else {
        return INCONNU;
    }
}

String CapteurCouleur::couleurToString(uint8_t couleur) {
    switch (couleur) {
        case ROUGE:
            return "Rouge";
            break;
        case VERT:
            return "Vert";
            break;
        case BLEU:
            return "Bleu";
            break;
        case JAUNE:
            return "Jaune";
            break;
        case NOIR:
            return "Noir";
            break;
        case BLANC:
            return "Blanc";
            break;
        default:
            return "Inconnu";
            break;
    }
}