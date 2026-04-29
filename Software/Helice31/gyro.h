
#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>

// Pins potentiomètres (à adapter si besoin)
#define PIN_POT_ANGLE_TANGAGE A0
#define PIN_POT_ANGLE_LACET   A1

void initGyro();
void calibrerGyro();
void mettreAJourFiltreComp();

float lireAngleTangage();
float lireAngleLacet();

bool gyroEstDisponible();

#endif
