#ifndef COMMANDE_H
#define COMMANDE_H

#include "config.h"

// Sélection du gain en cours de réglage
typedef enum {
    GAIN_KP = 0,
    GAIN_KI = 1,
    GAIN_KD = 2
} GainSelectionne;

// Initialise les pins : potentiomètres, bouton (pull-up), module RF
void initCommande();

// Doit être appelé dans loop() : détecte l'appui bouton et cycle Kp→Ki→Kd→Kp
// Gère le debounce en interne
void majSelectionGain();

// Retourne le gain actuellement sélectionné (GAIN_KP / GAIN_KI / GAIN_KD)
GainSelectionne gainActuel();

// Lit le potentiomètre unique et met à jour le gain sélectionné
// Retourne les 3 valeurs courantes de kp, ki, kd
void lireGains(float* kp, float* ki, float* kd);

// Consigne principale depuis la manette RF (si disponible)
float lireConsigneRF();

// Consigne de secours depuis potentiomètre de redondance (si RF indispo)
float lireConsigneRedondance();

// Retourne true si une trame RF valide a été reçue récemment
bool rfDisponible();

#endif