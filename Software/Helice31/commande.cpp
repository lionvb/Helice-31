#include "commande.h"
#include <Arduino.h>

#define RF_TIMEOUT_MS    500   // délai sans trame avant bascule redondance
#define DEBOUNCE_MS       50   // anti-rebond bouton

static GainSelectionne gainCourant = GAIN_KP;

// Valeurs mémorisées de chaque gain 
static float valKp = 0.0f;
static float valKi = 0.0f;
static float valKd = 0.0f;

// RF
static unsigned long derniereTrameRF = 0;
static float consigneRF = 0.0f;

// Debounce bouton
static bool  etatBoutonPrec   = HIGH;   // pull-up → HIGH = relâché
static unsigned long dernierAppui = 0;

void initCommande() {
    pinMode(PIN_POT_REDONDANCE, INPUT);
    pinMode(PIN_POT_GAIN,       INPUT);
    pinMode(PIN_BTN_GAIN,       INPUT_PULLUP);  // pull-up interne

}

void majSelectionGain() {
    bool etatActuel = digitalRead(PIN_BTN_GAIN);

    // Détection front descendant (HIGH → LOW) avec debounce
    if (etatBoutonPrec == HIGH && etatActuel == LOW) {
        unsigned long maintenant = millis();
        if (maintenant - dernierAppui > DEBOUNCE_MS) {
            dernierAppui = maintenant;

            // Cycle : Kp → Ki → Kd → Kp
            switch (gainCourant) {
                case GAIN_KP: gainCourant = GAIN_KI; break;
                case GAIN_KI: gainCourant = GAIN_KD; break;
                case GAIN_KD: gainCourant = GAIN_KP; break;
            }
        }
    }
    etatBoutonPrec = etatActuel;
}

GainSelectionne gainActuel() {
    return gainCourant;
}

void lireGains(float* kp, float* ki, float* kd) {
    // Le potentiomètre unique met à jour UNIQUEMENT le gain sélectionné
    // Les deux autres gardent leur dernière valeur mémorisée
    float valPot = analogRead(PIN_POT_GAIN) / 1023.0f;

    switch (gainCourant) {
        case GAIN_KP: valKp = valPot * KP_MAX; break;
        case GAIN_KI: valKi = valPot * KI_MAX; break;
        case GAIN_KD: valKd = valPot * KD_MAX; break;
    }

    *kp = valKp;
    *ki = valKi;
    *kd = valKd;
}

float lireConsigneRF() {
    // Implémentation plus tard
    return consigneRF;
}

float lireConsigneRedondance() {
    // Potentiomètre de secours → angle consigne [-ANGLE_MAX_DEG, +ANGLE_MAX_DEG]
    int raw = analogRead(PIN_POT_REDONDANCE);
    long mapped = map(raw, 0, 1023,
                      (long)(-ANGLE_MAX_DEG * 100),
                      (long)( ANGLE_MAX_DEG * 100));
    return mapped / 100.0f;
}

bool rfDisponible() {
    return (millis() - derniereTrameRF) < RF_TIMEOUT_MS;
}