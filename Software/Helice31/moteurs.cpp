/*
initMoteurs()
Attache les ESC, envoie le signal neutre 3 s pour déverrouiller la sécurité
Init
setMoteurPrincipal(commande)
Convertit la commande normalisée en µs PWM et l'envoie à l'ESC principal
Sortie
setRotorQueue(vitesse_base, delta)
Règle le rotor anti-couple : vitesse_base + delta de correction lacet
Sortie
arretUrgence()
Coupe immédiatement les deux moteurs (PWM_MIN sur les deux ESC)
Sortie
estEnSaturation()
Retourne true si le dernier appel a atteint les limites PWM → pour les LEDs
Utilitaire
*/

#include "moteurs.h"
#include <Servo.h>
#include <Arduino.h>
 
static Servo escPrincipal;
static Servo escQueue;
static bool  saturation = false;
 
// Convertit une commande (COMMANDE_MIN, COMMANDE_MAX)
// en largeur d'impulsion PWM en µs [PWM_MIN, PWM_MAX]

static int commandeVersPWM(float commande) {
    float ratio = (commande - COMMANDE_MIN) / (COMMANDE_MAX - COMMANDE_MIN);
    int pwm = (int)(PWM_MIN + ratio * (PWM_MAX - PWM_MIN));
    return constrain(pwm, PWM_MIN, PWM_MAX);
}
 
void initMoteurs() {
    escPrincipal.attach(PIN_MOTEUR_PRINCIPAL, PWM_MIN, PWM_MAX);
    escQueue.attach(PIN_ROTOR_QUEUE,          PWM_MIN, PWM_MAX);
 
    // Séquence obligatoire : signal neutre minimum 3 s
    // Les ESC brushless ont une sécurité au démarrage :
    // ils refusent de démarrer si le signal n'est pas à PWM_MIN
    escPrincipal.writeMicroseconds(PWM_MIN);
    escQueue.writeMicroseconds(PWM_MIN);
    delay(3000);
}
 
void setMoteurPrincipal(float commande) {
    // Détecter la saturation avant de contraindre
    saturation = (commande >= COMMANDE_MAX || commande <= COMMANDE_MIN);
    commande   = constrain(commande, COMMANDE_MIN, COMMANDE_MAX);
    escPrincipal.writeMicroseconds(commandeVersPWM(commande));
}
 
void setRotorQueue(float vitesse_base, float delta) {
    // Le rotor de queue tourne en continu à vitesse_base pour compenser
    // le couple réactif du rotor principal. Le delta est la correction PID lacet.
    //
    // Comportement attendu :
    //   delta > 0 → augmente la vitesse → tourne dans l'autre sens (surcompense)
    //   delta < 0 → diminue la vitesse → tourne moins vite (sous-compense)
    float cmd = constrain(vitesse_base + delta, COMMANDE_MIN, COMMANDE_MAX);
    escQueue.writeMicroseconds(commandeVersPWM(cmd));
}
 
void arretUrgence() {
    escPrincipal.writeMicroseconds(PWM_MIN);
    escQueue.writeMicroseconds(PWM_MIN);
    saturation = false;
}
 
bool estEnSaturation() {
    return saturation;
}