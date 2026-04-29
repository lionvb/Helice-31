#include "leds.h"
#include <Arduino.h>
static int modeActuel = MODE_NUMERIQUE;
 
void initLeds() {

    pinMode(PIN_LED_SATURATION, OUTPUT);
    pinMode(PIN_LED_KP, OUTPUT);
    pinMode(PIN_LED_KI, OUTPUT);
    pinMode(PIN_LED_KD, OUTPUT);
    digitalWrite(PIN_LED_SATURATION,HIGH);
    digitalWrite(PIN_LED_KP, HIGH);
    digitalWrite(PIN_LED_KI, HIGH);
    digitalWrite(PIN_LED_KD, HIGH);

}

void majLeds(bool saturation) {
    // LED rouge : commande moteur saturée
    digitalWrite(PIN_LED_SATURATION, saturation ? HIGH : LOW);
}
 
void majLedsGain(GainSelectionne gain) {
    // Une seule LED allumée à la fois
    digitalWrite(PIN_LED_KP, (gain == GAIN_KP) ? HIGH : LOW);
    digitalWrite(PIN_LED_KI, (gain == GAIN_KI) ? HIGH : LOW);
    digitalWrite(PIN_LED_KD, (gain == GAIN_KD) ? HIGH : LOW);
}
 