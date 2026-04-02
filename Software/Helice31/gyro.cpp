#include "gyro.h"
#include <Wire.h>
#include <Arduino.h>
 
#define MPU_ADDR  0x68
 
// 0.98 → 98% gyroscope, 2% accéléromètre (filtre complémentaire classique)
#define ALPHA_GYRO  0.98f
 
// 0.95 → 95% angle gyro filtré, 5% potentiomètre mécanique
// Corrige la dérive lente du gyroscope pendant les longues démos
#define ALPHA_POT   0.95f
 
// Débattement angulaire couvert par les potentiomètres mécaniques
// À ajuster selon la mécanique réelle de la maquette
#define POT_ANGLE_MIN  -45.0f
#define POT_ANGLE_MAX   45.0f
 
static float angleTanguage  = 0.0f;
static float angleLacet     = 0.0f;
static float offsetGX       = 0.0f;
static float offsetGZ       = 0.0f;
static bool  gyroDisponible = false;
static unsigned long dernierTemps = 0;
 
static void lireRegistres(int16_t* aX, int16_t* aY, int16_t* aZ, int16_t* gX, int16_t* gY, int16_t* gZ) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);
  *aX = (Wire.read() << 8) | Wire.read();
  *aY = (Wire.read() << 8) | Wire.read();
  *aZ = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read();  // température ignorée
  *gX = (Wire.read() << 8) | Wire.read();
  *gY = (Wire.read() << 8) | Wire.read();
  *gZ = (Wire.read() << 8) | Wire.read();
}
 
// ADC 0-1023 → angle en degrés selon la plage mécanique
static float potVersAngle(int pin) {
  int raw = analogRead(pin);
  long mapped = map(raw, 0, 1023, (long)(POT_ANGLE_MIN * 100), (long)(POT_ANGLE_MAX * 100));
  return mapped / 100.0f;
}
 
void initGyro() {
  pinMode(PIN_POT_ANGLE_TANGAGE, INPUT);
  pinMode(PIN_POT_ANGLE_LACET,   INPUT);
  Wire.begin();

  // Tenter de réveiller le MPU-6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  byte err = Wire.endTransmission(true);

  if (err == 0) {
    // MPU détecté : gyro ±250°/s, accel ±2g
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1B); Wire.write(0x00);
    Wire.endTransmission(true);
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1C); Wire.write(0x00);
    Wire.endTransmission(true);
    gyroDisponible = true;
  } else {
    // Mode dégradé : potentiomètres mécaniques seuls
    gyroDisponible = false;
  }

  // Initialiser les angles depuis les pots (point de départ cohérent)
  angleTanguage = potVersAngle(PIN_POT_ANGLE_TANGAGE);
  angleLacet    = potVersAngle(PIN_POT_ANGLE_LACET);
  dernierTemps  = micros();
}
 
void calibrerGyro() {
  if (!gyroDisponible) return;

  const int N = 200;
  long sumGX = 0, sumGZ = 0;
  for (int i = 0; i < N; i++) {
    int16_t aX, aY, aZ, gX, gY, gZ;
    lireRegistres(&aX, &aY, &aZ, &gX, &gY, &gZ);
    sumGX += gX;
    sumGZ += gZ;
    delay(5);
  }
  offsetGX = sumGX / (float)N;
  offsetGZ = sumGZ / (float)N;
}
 
void mettreAJourFiltreComp() {
  // Lecture retour mécanique (toujours disponible, quelle que soit la config)
  float potTanguage = potVersAngle(PIN_POT_ANGLE_TANGAGE);
  float potLacet    = potVersAngle(PIN_POT_ANGLE_LACET);

  if (gyroDisponible) {
    // ── Mode nominal : gyro + accél + correction dérive par pot ──
    int16_t aX, aY, aZ, gX, gY, gZ;
    lireRegistres(&aX, &aY, &aZ, &gX, &gY, &gZ);

    unsigned long maintenant = micros();
    float dt = (maintenant - dernierTemps) / 1000000.0f;
    dernierTemps = maintenant;

    // Vitesses angulaires en °/s (sensibilité 131 LSB/°/s pour ±250°/s)
    float gyroTanguage = (gX - offsetGX) / 131.0f;
    float gyroLacet    = (gZ - offsetGZ) / 131.0f;

    // Angle tanguage depuis accéléromètre (référence absolue)
    float accelTanguage = atan2((float)aY, (float)aZ) * 180.0f / PI;

    // Filtre complémentaire gyro + accel
    float angleBrutTanguage = ALPHA_GYRO * (angleTanguage + gyroTanguage * dt) + (1.0f - ALPHA_GYRO) * accelTanguage;

    // Fusion finale avec potentiomètre mécanique pour corriger la dérive
    angleTanguage = ALPHA_POT * angleBrutTanguage + (1.0f - ALPHA_POT) * potTanguage;

    // Lacet : intégration gyro uniquement (pas de référence accel sur Z)
    // + correction légère par potentiomètre mécanique
    float angleBrutLacet = angleLacet + gyroLacet * dt;
    angleLacet = ALPHA_POT * angleBrutLacet + (1.0f - ALPHA_POT) * potLacet;

} else {
    // ── Mode dégradé : potentiomètres mécaniques seuls ──
    // Filtre passe-bas simple pour lisser le bruit ADC (10 bits)
    angleTanguage = 0.8f * angleTanguage + 0.2f * potTanguage;
    angleLacet    = 0.8f * angleLacet    + 0.2f * potLacet;
  }
}

float lireAngleTangage() {
return angleTanguage;
}

float lireAngleLacet() {
return angleLacet;
}

bool  gyroEstDisponible() { return gyroDisponible; }
