#ifndef CONFIG_H
#define CONFIG_H

//  PARAMÈTRES TEMPS RÉEL

#define TE       0.1f    // Période d'échantillonnage en secondes (0.1s = 100ms)
#define TE_MS    100     // TE en millisecondes (pour le calcul du timer)
#define N_FILTRE 10      // Facteur filtre dérivée (renommé pour éviter conflit)

//  LIMITES COMMANDE PID

#define COMMANDE_MIN     0.0f    // commande minimale (correspond à PWM_MIN)
#define COMMANDE_MAX     100.0f  // commande maximale (correspond à PWM_MAX)

//  PINS MOTEURS

#define PIN_MOTEUR_PRINCIPAL  9
#define PIN_ROTOR_QUEUE       10

//  PINS POTENTIOMÈTRES

#define PIN_POT_ANGLE_TANGAGE A0   // retour mécanique axe tanguage
#define PIN_POT_ANGLE_LACET   A1   // retour mécanique axe lacet
#define PIN_POT_REDONDANCE    A2   // consigne de secours (sans manette RF)
#define PIN_POT_GAIN          A3   // pot unique réglage Kp / Ki / Kd

//  BOUTON SÉLECTION GAIN
#define PIN_BTN_GAIN          2    // pull-up interne

//  PINS LEDs

#define PIN_LED_SATURATION    3    // rouge  — commande saturée
#define PIN_LED_MODE          4    // jaune  — mode analogique actif
#define PIN_LED_RF            5    // bleu   — RF active    
#define PIN_LED_KP            6    // vert   — on règle Kp
#define PIN_LED_KI            7    // vert   — on règle Ki
#define PIN_LED_KD            8    // vert   — on règle Kd

//  SIGNAL ESC (PWM en µs)

#define PWM_MIN               1000   // moteur à l'arrêt
#define PWM_MAX               2000   // pleine puissance

//  LIMITES GAINS PID

#define KP_MAX                5.0f
#define KI_MAX                2.0f
#define KD_MAX                1.0f

//  CONSIGNE

#define ANGLE_MAX_DEG         30.0f 

//  ROTOR DE QUEUE

#define VITESSE_BASE_QUEUE    40.0f   // régime nominal rotor queue (%)

//  MODES

#define MODE_NUMERIQUE        0
#define MODE_ANALOGIQUE       1

#endif