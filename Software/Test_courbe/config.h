#define PIN_MOTEUR_PRINCIPAL 9
#define PIN_ROTOR_ANTICOUPLE      
#define PIN_POT_CONSIGNE      
#define PIN_POT_KP            
#define PIN_POT_KI            
#define PIN_POT_KD            
#define PIN_LED_SATURATION    
#define PIN_LED_MODE     

//  SIGNAL ESC (PWM en µs)

#define PWM_MIN               1000   // moteur à l'arrêt
#define PWM_MAX               2000   // pleine puissance.


#define COMMANDE_MIN     0.0f    // commande minimale (correspond à PWM_MIN)
#define COMMANDE_MAX     100.0f  // commande maximale (correspond à PWM_MAX)