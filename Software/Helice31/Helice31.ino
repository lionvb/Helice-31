#include "config.h"
#include "gyro.h"
#include "pid.h"
#include "moteurs.h"
#include "commande.h"
#include "leds.h"

//Instances PID
EtatPID pidTanguage;
EtatPID pidLacet;

//Flag levé par l'ISR du Timer2 
volatile bool flag_echantillon = false;

// ISR Timer12
ISR(TIMER2_COMPA_vect) {
    flag_echantillon = true;
}

//Configuration Timer2 en mode CTC
void initTimer2() {
    noInterrupts();

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;

    OCR2A = 156; // exemple ~10ms

    TCCR2A |= (1 << WGM21); // CTC
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // prescaler 1024

    TIMSK2 |= (1 << OCIE2A);

    interrupts();
}

void setup() {
    Serial.begin(115200);

    // 1. Gyroscope MPU-6050 + potentiomètres de mesure mécaniques
    initGyro();
    calibrerGyro();      //  hélico immobile obligatoire

    // 2. Moteurs : séquence sécurité ESC (3 s signal neutre)
    initMoteurs();

    // 3. Commande : pot gain, bouton sélection, pot redondance, RF
    initCommande();

    // 4. LEDs (allume LED_KP par défaut)
    initLeds();

    // 5. PID : remise à zéro des deux instances
    resetPID(&pidTanguage);
    resetPID(&pidLacet);

    // 6. Timer en dernier (démarre la boucle temps réel)
    initTimer2();

    if (gyroEstDisponible()) {
        Serial.println("Helice31 pret — mode nominal (gyro + pots)");
    } else {
        Serial.println("Helice31 pret — mode degrade (pots seuls)");
    }
}

void loop() {
    
    //Bouton sélection gain : géré HORS du flag timer
    majSelectionGain();
    majLedsGain(gainActuel());

    //Attendre le tick timer
    if (!flag_echantillon) return;
    flag_echantillon = false;
    
    //1. Mise à jour filtre et lecture angles
    mettreAJourFiltreComp();
    float angleTangage = lireAngleTangage();
    float angleLacet   = lireAngleLacet();

    //2. Lecture consigne 
    // Priorité : RF → pot redondance si RF indispo
    float consigne;
    bool rfOk = rfDisponible();
    if (rfOk) {
        consigne = lireConsigneRF();
    } else {
        consigne = lireConsigneRedondance();
    }
    digitalWrite(PIN_LED_RF, rfOk ? HIGH : LOW);

    //3. Lecture gains (pot unique → gain sélectionné)
    float kp, ki, kd;
    lireGains(&kp, &ki, &kd);

    // 4. Correcteur PID axe tanguage
    float cmdPrincipale = calculPID(consigne, angleTangage,kp, ki, kd, &pidTanguage);

    //5. Correcteur PID axe lacet (consigne = 0° : maintenir le cap)
    float cmdLacet = calculPID(0.0f, angleLacet,kp * 0.5f, ki * 0.3f, kd * 0.5f, &pidLacet);

    //6. Envoi commandes moteurs 
    setMoteurPrincipal(cmdPrincipale);
    setRotorQueue(VITESSE_BASE_QUEUE, cmdLacet);

    //7. LEDs saturation + mode 
    delay(500);
    //8. Debug série 
    Serial.print(rfOk ? "RF " : "POT");
    Serial.print(" ref=");  Serial.print(consigne, 1);
    Serial.print(" tan=");  Serial.print(angleTangage, 1);
    Serial.print(" lac=");  Serial.print(angleLacet, 1);
    Serial.print(" u1=");   Serial.print(cmdPrincipale, 1);
    Serial.print(" u2=");   Serial.print(cmdLacet, 1);
    Serial.print(" Kp=");   Serial.print(kp, 2);
    Serial.print(" Ki=");   Serial.print(ki, 2);
    Serial.print(" Kd=");   Serial.println(kd, 2);
}