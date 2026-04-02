/*
initLeds()
Configure les pins LED en OUTPUT, éteint tout
Init
majLeds(saturation)
Met à jour toutes les LEDs : saturation moteur, mode numérique/analogique, RF actif
Sortie
setModeSLed(mode)
Force l'affichage du mode courant (NUMERIQUE / ANALOGIQUE / RF)
Sortie
*/

#include "leds.h"
#include <Arduino.h>
static int modeActuel = MODE_NUMERIQUE;
 
void initLeds() {

    pinMode(PIN_LED_SATURATION, OUTPUT);
    pinMode(PIN_LED_MODE,       OUTPUT);
    pinMode(PIN_LED_RF,         OUTPUT);
    pinMode(PIN_LED_KP,         OUTPUT);
    pinMode(PIN_LED_KI, OUTPUT);
    pinMode(PIN_LED_KD, OUTPUT);
    digitalWrite(PIN_LED_SATURATION, LOW)
    digitalWrite(PIN_LED_MODE, LOW);
    digitalWrite(PIN_LED_RF, LOW);
    digitalWrite(PIN_LED_KP, LOW);
    digitalWrite(PIN_LED_KI, LOW);
    digitalWrite(PIN_LED_KD, LOW);
    digitalWrite(PIN_LED_KP, HIGH);
}

void majLeds(bool saturation) {
    // LED rouge : commande moteur saturée
    digitalWrite(PIN_LED_SATURATION, saturation ? HIGH : LOW);
    // LED mode : allumée en mode analogique
    digitalWrite(PIN_LED_MODE, (modeActuel == MODE_ANALOGIQUE) ? HIGH : LOW);
}
 
void majLedsGain(GainSelectionne gain) {
    // Une seule LED allumée à la fois
    digitalWrite(PIN_LED_KP, (gain == GAIN_KP) ? HIGH : LOW);
    digitalWrite(PIN_LED_KI, (gain == GAIN_KI) ? HIGH : LOW);
    digitalWrite(PIN_LED_KD, (gain == GAIN_KD) ? HIGH : LOW);
}
 
void setModeLed(int mode) {
    modeActuel = mode;
    digitalWrite(PIN_LED_MODE, (mode == MODE_ANALOGIQUE) ? HIGH : LOW);
}
 