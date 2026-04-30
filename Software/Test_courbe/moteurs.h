#ifndef MOTEURS_H
#define MOTEURS_H

#include "config.h"

// Initialise les ESC et envoie le signal neutre (séquence sécurité 3 s)
void initMoteurs();

// Envoie la commande au moteur principal [COMMANDE_MIN, COMMANDE_MAX]
void setMoteurPrincipal(float commande);

// Commande le rotor de queue : vitesse_base + delta de correction lacet
void setRotorQueue(float vitesse_base, float delta);

// Arrêt d'urgence : coupe immédiatement les deux ESC
void arretUrgence();

// Retourne true si le dernier appel a atteint les limites PWM
bool estEnSaturation();

#endif