
#include "gyro.h"
#include <Wire.h>

#define MPU_ADDR 0x68
#define ALPHA 0.98f

static float anglePitch = 0.0f;
static float angleYaw   = 0.0f;

static float offsetGX = 0.0f;
static float offsetGZ = 0.0f;
static float offsetAnglePitch = 0.0f;

static unsigned long lastTime = 0;


// ===== LECTURE =====
static void lireRegistres(int16_t* ax, int16_t* ay, int16_t* az,
                          int16_t* gx, int16_t* gy, int16_t* gz) {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  *ax = (Wire.read() << 8) | Wire.read();
  *ay = (Wire.read() << 8) | Wire.read();
  *az = (Wire.read() << 8) | Wire.read();

  Wire.read(); Wire.read();

  *gx = (Wire.read() << 8) | Wire.read();
  *gy = (Wire.read() << 8) | Wire.read();
  *gz = (Wire.read() << 8) | Wire.read();
}


// ===== INIT =====
void initGyro() {

  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  lastTime = micros();

  // ===== ZERO PROPRE (moyenne) =====
  const int N = 100;
  float somme = 0;

  for (int i = 0; i < N; i++) {
    int16_t ax, ay, az, gx, gy, gz;
    lireRegistres(&ax, &ay, &az, &gx, &gy, &gz);

    float angle = atan2((float)ay, (float)az) * 180.0f / PI;
    somme += angle;

    delay(5);
  }

  offsetAnglePitch = somme / N;
  anglePitch = 0.0f;
}


// ===== CALIBRATION =====
void calibrerGyro() {

  const int N = 200;
  long sumGX = 0, sumGZ = 0;

  for (int i = 0; i < N; i++) {
    int16_t ax, ay, az, gx, gy, gz;
    lireRegistres(&ax, &ay, &az, &gx, &gy, &gz);

    sumGX += gx;
    sumGZ += gz;

    delay(5);
  }

  offsetGX = sumGX / (float)N;
  offsetGZ = sumGZ / (float)N;
}


// ===== FILTRE =====
void mettreAJourFiltreComp() {

  int16_t ax, ay, az, gx, gy, gz;
  lireRegistres(&ax, &ay, &az, &gx, &gy, &gz);

  unsigned long now = micros();
  float dt = (now - lastTime) / 1000000.0f;
  lastTime = now;

  float gyroX = (gx - offsetGX) / 131.0f;
  float gyroZ = (gz - offsetGZ) / 131.0f;

  float accelPitch = atan2((float)ay, (float)az) * 180.0f / PI;
  accelPitch -= offsetAnglePitch;

  anglePitch = ALPHA * (anglePitch + gyroX * dt)
             + (1.0f - ALPHA) * accelPitch;

  angleYaw += gyroZ * dt;
}


// ===== GETTERS =====
float lireAngleTangage() {
  return anglePitch;
}

float lireAngleLacet() {
  return angleYaw;
}

bool gyroEstDisponible() {
  return true;
}

