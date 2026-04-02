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
const int PIN_ESC_HAUT = 5;
const int PIN_ESC_BAS  = 6;

const int PWM_MIN     = 1000;
const int PWM_MAX     = 2000; 
const int PWM_ARME    = 1000; 
const int GAZ_MIN_VOL = 1100;