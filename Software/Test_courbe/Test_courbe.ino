#include "config.h"
#include "gyro.h"

// Variables pour notre minuterie non-bloquante
unsigned long tempsPrecedent = 0;
const int PERIODE_AFFICHAGE_MS = 250; // 20ms = 50 Hz (rafraîchissement fluide)

void setup() {
    Serial.begin(115200);

    // Initialisation du MPU6050 en utilisant VOS fonctions
    initGyro();
    
    // Rappel : l'hélicoptère doit être parfaitement immobile ici !
    calibrerGyro(); 
}

void loop() {
    // 1. Le calcul des angles tourne en permanence à vitesse maximale
    mettreAJourFiltreComp();

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
        Serial.print(angleTangage);
        Serial.print(","); // La virgule sépare les différentes courbes
        
        Serial.print("Lacet:");
        Serial.println(angleLacet); // println à la fin pour valider la ligne
    }
}