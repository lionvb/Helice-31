#include "pid.h"
#include <Arduino.h>
 
float erreur,Un,Up,Ui,Ud ;

void resetPID(EtatPID* pid) {
    pid->erreur_prec = 0.0f;
    pid->integrale_prec = 0.0f;
    pid->derivee_prec = 0.0f;
    pid->commande = 0.0f;
}


float calculPID(float consigne, float mesure,float kp, float ki, float kd,EtatPID* pid){
    erreur=consigne-mesure; 
    Up=kp*erreur;   //Calcul Up
    Ui=pid->integrale_prec+((ki*TE)/2)*(erreur-pid->erreur_prec);     //Calcul Ui
    Ud=((TE*N_FILTRE)/(TE*N_FILTRE+2*(kd/kp)))*(pid->derivee_prec*((2*(kd/kp)/TE)+1)+(2*kd/TE)*(erreur-pid->erreur_prec)); //Calcul Ud
    Un=Up+Ui+Ud;    //Calcul total
    pid->erreur_prec=erreur;
    pid->integrale_prec=Ui;
    pid->derivee_prec=Ud;
    return Un;
}

