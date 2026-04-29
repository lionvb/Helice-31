

#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>

void initGyro();
void calibrerGyro();
void mettreAJourFiltreComp();

float lireAngleTangage();
float lireAngleLacet();

bool gyroEstDisponible();

#endif


