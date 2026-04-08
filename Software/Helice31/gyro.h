#ifndef GYRO_H
#define GYRO_H
 
#include "config.h"
 
// Initialise le MPU-6050 via I2C, configure plage et filtre interne
void initGyro();
 
// Calcule et stocke les offsets à vide (appeler au démarrage, hélico immobile)
void calibrerGyro();
 
// Applique le filtre complémentaire accel + gyro → angle fusionné stable
// À appeler à chaque période d'échantillonnage avant de lire les angles
void mettreAJourFiltreComp();
 
// Retourne l'angle de tanguage en degrés (axe principal)
float lireAngleTangage();
 
// Retourne l'angle de lacet en degrés (axe rotor de queue)
float lireAngleLacet();

// Retourne true si le MPU-6050 a été détecté au démarrage
bool gyroEstDisponible();
 
#endif