
#include "gyro.h"

#define PIN_MOTEUR 9   // PWM moteur

// Réglage simple (à ajuster)
float Kp = 3.0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_MOTEUR, OUTPUT);

  initGyro();
  calibrerGyro();

  delay(1000);
}

void loop() {

  mettreAJourFiltreComp();

  float angle = lireAngleTangage();

  // Consigne = équilibre
  float consigne = 0.0;

  // Erreur
  float erreur = consigne - angle;

  // Commande moteur (P simple)
  float commande = Kp * erreur;

  // Conversion PWM
  int pwm = constrain(commande + 128, 0, 255);
  analogWrite(PIN_MOTEUR, pwm);

  // ===== PLOT =====
  Serial.print(angle);     // courbe 1
  Serial.print(" ");
  Serial.print(consigne);  // courbe 2
  Serial.print(" ");
  Serial.println(pwm);     // courbe 3

  delay(10);
}
