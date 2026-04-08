#include "config.h"

#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>

// INITIALISATION
void initGyro();
void calibrerGyro();

// MISE À JOUR
void mettreAJourFiltreComp();

// LECTURE DES ANGLES
float lireAngleTangage();
float lireAngleLacet();

#endif

