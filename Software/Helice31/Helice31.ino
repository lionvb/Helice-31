#include "config.h"
#include "gyro.h"
#include "pid.h"
#include "moteurs.h"
#include "commande.h"
#include "leds.h"





void setup() {
  Serial.begin(115200);

  initGyro();
  calibrerGyro();

  delay(1000); // stabilisation
}

void loop() {

  mettreAJourFiltreComp();

  // Format pour Serial Plotter
  Serial.print(lireAngleTangage());
  Serial.print(",");
  Serial.println(lireAngleLacet());

  delay(10);
}
