#ifndef LEDS_H
#define LEDS_H

#include "config.h"
#include "commande.h"  // pour GainSelectionne

// Configure toutes les pins LED en OUTPUT et éteint tout
void initLeds();

// Met à jour la LED de saturation moteur
void majLeds(bool saturation);

// Met à jour les 3 LEDs Kp/Ki/Kd selon le gain actuellement sélectionné
void majLedsGain(GainSelectionne gain);


#endif