#include "gyro.h"
#include <Wire.h>
#include <Arduino.h>

#define MPU_ADDR  0x68

#define ALPHA_GYRO  0.98f
#define ALPHA_POT   0.95f

#define POT_ANGLE_MIN  -45.0f
#define POT_ANGLE_MAX   45.0f

// Variables

static float angleTanguage  = 0.0f;
static float angleLacet     = 0.0f;

static float offsetGX = 0.0f;
static float offsetGZ = 0.0f;

// Offset d'angle pour définir le zéro
static float offsetAnglePitch = 0.0f;

static bool  gyroDisponible = false;
static unsigned long dernierTemps = 0;


// Lecture MPU

static void lireRegistres(int16_t* aX, int16_t* aY, int16_t* aZ,
                          int16_t* gX, int16_t* gY, int16_t* gZ) {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  *aX = (Wire.read() << 8) | Wire.read();
  *aY = (Wire.read() << 8) | Wire.read();
  *aZ = (Wire.read() << 8) | Wire.read();

  Wire.read(); Wire.read(); // température ignorée

  *gX = (Wire.read() << 8) | Wire.read();
  *gY = (Wire.read() << 8) | Wire.read();
  *gZ = (Wire.read() << 8) | Wire.read();
}


// Potentiomètre

static float potVersAngle(int pin) {
  int raw = analogRead(pin);
  long mapped = map(raw, 0, 1023,
                    (long)(POT_ANGLE_MIN * 100),
                    (long)(POT_ANGLE_MAX * 100));
  return mapped / 100.0f;
}


// Init

void initGyro() {

  pinMode(PIN_POT_ANGLE_TANGAGE, INPUT);
  pinMode(PIN_POT_ANGLE_LACET,   INPUT);

  Wire.begin();

  // Réveil MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  byte err = Wire.endTransmission(true);

  if (err == 0) {
    gyroDisponible = true;

    // Config gyro
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1B); Wire.write(0x00);
    Wire.endTransmission(true);

    // Config accel
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1C); Wire.write(0x00);
    Wire.endTransmission(true);

  } else {
    gyroDisponible = false;
  }

  // Initialisation du zéro
  int16_t aX, aY, aZ, gX, gY, gZ;
  lireRegistres(&aX, &aY, &aZ, &gX, &gY, &gZ);

  float accelTanguage = atan2((float)aY, (float)aZ) * 180.0f / PI;

  // Stocker le décalage
  offsetAnglePitch = accelTanguage;

  // Démarrer à 0°
  angleTanguage = 0.0f;
  angleLacet    = 0.0f;

  dernierTemps = micros();
}


// Calibration

void calibrerGyro() {

  if (!gyroDisponible) return;

  const int N = 200;

  long sumGX = 0;
  long sumGZ = 0;

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


// Filtre

void mettreAJourFiltreComp() {

  float potTanguage = potVersAngle(PIN_POT_ANGLE_TANGAGE);
  float potLacet    = potVersAngle(PIN_POT_ANGLE_LACET);

  if (gyroDisponible) {

    int16_t aX, aY, aZ, gX, gY, gZ;
    lireRegistres(&aX, &aY, &aZ, &gX, &gY, &gZ);

    unsigned long maintenant = micros();
    float dt = (maintenant - dernierTemps) / 1000000.0f;
    dernierTemps = maintenant;

    float gyroTanguage = (gX - offsetGX) / 131.0f;
    float gyroLacet    = (gZ - offsetGZ) / 131.0f;

    //  angle accel 
    float accelTanguage = atan2((float)aY, (float)aZ) * 180.0f / PI;
    accelTanguage -= offsetAnglePitch;

    float angleBrutTanguage =
      ALPHA_GYRO * (angleTanguage + gyroTanguage * dt)
    + (1.0f - ALPHA_GYRO) * accelTanguage;

    angleTanguage =
      ALPHA_POT * angleBrutTanguage
    + (1.0f - ALPHA_POT) * potTanguage;

    float angleBrutLacet = angleLacet + gyroLacet * dt;

    angleLacet =
      ALPHA_POT * angleBrutLacet
    + (1.0f - ALPHA_POT) * potLacet;

  } else {
    // Mode sans gyro
    angleTanguage = 0.8f * angleTanguage + 0.2f * potTanguage;
    angleLacet    = 0.8f * angleLacet    + 0.2f * potLacet;
  }
}

float lireAngleTangage() {
  return -angleTanguage;
}

float lireAngleLacet() {
  return angleLacet;
}

bool gyroEstDisponible() {
  return gyroDisponible;
}