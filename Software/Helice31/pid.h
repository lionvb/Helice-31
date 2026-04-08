#ifndef PID_H
#define PID_H

#include "config.h"


//  STRUCTURE ÉTAT PID
typedef struct {
    float erreur_prec;      // erreur à n-1
    float integrale_prec;   // intégrale à n-1
    float derivee_prec;     // dérivée à n-1
    float commande;         // sortie u[n]
} EtatPID;

// Remise à zéro de l'état (appeler dans setup())
void resetPID(EtatPID* pid);


float calculPID(float consigne, float mesure,float kp, float ki, float kd,EtatPID* pid);

#endif