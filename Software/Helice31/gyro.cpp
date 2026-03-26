


#include "Gyro.h"
#include <Wire.h>

#define MPU 0x68

// VARIABLES INTERNES

// Offsets du gyroscope (erreurs)
static float offset_gx = 0, offset_gy = 0, offset_gz = 0;

// Angles calculés
static float anglePitch = 0;
static float angleYaw = 0;

// Gestion du temps
static unsigned long lastTime;

// Données brutes
static int16_t ax, ay, az;
static int16_t gx, gy, gz;


// FONCTION INTERNE (non accessible)
static void lireBrut() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  Wire.read(); Wire.read(); // température ignorée

  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();
}


// INITIALISATION
void initGyro() {
  Wire.begin();

  // Réveil du MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Gyroscope ±250°/s
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Accéléromètre ±2g
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Filtre interne (réduction du bruit)
  Wire.beginTransmission(MPU);
  Wire.write(0x1A);
  Wire.write(0x03);
  Wire.endTransmission(true);

  lastTime = millis();
}


// CALIBRATION
void calibrerGyro() {
  int n = 500;

  for (int i = 0; i < n; i++) {
    lireBrut();

    offset_gx += gx;
    offset_gy += gy;
    offset_gz += gz;

    delay(5);
  }

  // Moyenne des offsets
  offset_gx /= n;
  offset_gy /= n;
  offset_gz /= n;
}


// FILTRE COMPLÉMENTAIRE
void mettreAJourFiltreComp() {
  lireBrut();

  // Conversion accéléromètre
  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;

  // Conversion gyroscope + correction offset
  float gyroX = (gx - offset_gx) / 131.0;
  float gyroY = (gy - offset_gy) / 131.0;
  float gyroZ = (gz - offset_gz) / 131.0;

  // Calcul du temps écoulé
  float dt = (millis() - lastTime) / 1000.0;
  lastTime = millis();

  // Angle avec accéléromètre (stable)
  float angleAccPitch = atan2(accY, accZ) * 180 / PI;

  // Fusion gyro + accel (filtre complémentaire)
  anglePitch = 0.98 * (anglePitch + gyroX * dt) + 0.02 * angleAccPitch;

  // Lacet (gyro seul → dérive possible)
  angleYaw += gyroZ * dt;
}


// ACCES AUX DONNEES
float lireAngleTangage() {
  return anglePitch;
}

float lireAngleLacet() {
  return angleYaw;
}
