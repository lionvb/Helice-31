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
#include "config.h"
#include <Arduino.h>

static int modeActuel = MODE_ANALOGIQUE;

// ============================================================
void initLeds() {
  pinMode(PIN_LED_SATURATION, OUTPUT);
  pinMode(PIN_LED_MODE,       OUTPUT);

  digitalWrite(PIN_LED_SATURATION, LOW);
  digitalWrite(PIN_LED_MODE,       LOW);
}

// ============================================================
void majLeds(bool saturation) {
  // LED saturation : allumée si un ESC a atteint ses limites
  digitalWrite(PIN_LED_SATURATION, saturation ? HIGH : LOW);

  // LED mode : reflète le mode courant
  setModeLed(modeActuel);
}

// ============================================================
void setModeLed(int mode) {
  modeActuel = mode;
  // MODE_ANALOGIQUE → LED éteinte (pas de signal RF → état par défaut)
  // MODE_RF         → LED allumée (signal RF actif et valide)
  digitalWrite(PIN_LED_MODE, (mode == MODE_RF) ? HIGH : LOW);
}