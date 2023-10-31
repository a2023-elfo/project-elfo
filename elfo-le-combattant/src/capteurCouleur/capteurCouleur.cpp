#include <capteurCouleur/capteurCouleur.h>
#include <SparkFunISL29125.h>

void CapteurCouleur::setupCapteurCouleur() {
    RGB_sensor.init();
}

uint8_t CapteurCouleur::lireCouleur() {
    int rougeappercu = RGB_sensor.readRed();
    int vertappercu = RGB_sensor.readGreen();
    int bleuappercu = RGB_sensor.readBlue();

    if (rougeappercu >= 190 && vertappercu <= 20 && bleuappercu <= 50)
    {
        return ROUGE;
    }
    else if (rougeappercu <= 20 && vertappercu >= 90 && bleuappercu <= 60)
    {
        return VERT;
    }
    else if (rougeappercu <= 20 && vertappercu <= 70 && bleuappercu >= 120)
    {
        return BLEU;
    }
    else if (rougeappercu >= 200 && vertappercu >= 200 && bleuappercu <= 20)
    {
        return JAUNE;
    }
    else if (rougeappercu <= 30 && vertappercu <= 30 && bleuappercu <= 30)
    {
        return NOIR;
    }
    else if (rougeappercu >= 240 && vertappercu >= 240 && bleuappercu >= 240)
    {
        return BLANC;
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