#include "config.h"
#include "gyro.h"
#include "moteurs.h"

// Variables pour notre minuterie non-bloquante
unsigned long tempsPrecedent = 0;
const int PERIODE_AFFICHAGE_MS = 50; // 20ms = 50 Hz (rafraîchissement fluide)

void setup() {
    Serial.begin(115200);

    // Initialisation du MPU6050 en utilisant VOS fonctions
    initGyro();
    
    // Rappel : l'hélicoptère doit être parfaitement immobile ici !
    calibrerGyro(); 
    initMoteurs();

}

void loop() {
    // 1. Le calcul des angles tourne en permanence à vitesse maximale
    mettreAJourFiltreComp();
    setMoteurPrincipal(5);

    // 2. Minuterie non-bloquante pour l'affichage
    unsigned long tempsActuel = millis();
    if (tempsActuel - tempsPrecedent >= PERIODE_AFFICHAGE_MS) {
        tempsPrecedent = tempsActuel;

        // Récupération de vos angles filtrés
        float angleTangage = lireAngleTangage();
        float angleLacet   = lireAngleLacet();

        // 3. Formatage "magique" pour le Traceur Série Arduino
        // La syntaxe doit être exactement : "Nom1:Valeur1, Nom2:Valeur2"
        Serial.print("Tangage:");
        Serial.println(angleTangage);
         // println à la fin pour valider la ligne
    }
    /*if (tempsActuel%10000==0){
        setMoteurPrincipal(5);
        delay(2000);
    }
    else{
        setMoteurPrincipal(0);
    }*/
}