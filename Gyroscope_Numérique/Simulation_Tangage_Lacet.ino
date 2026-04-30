// ================================================================
// HÉLICE-31 — Simulation mathématique 2 DOF (Tangage / Lacet)
// ================================================================
 
// --- MÉMOIRE DE L'HÉLICO ---
float angleX = 0.0;
float angleY = 0.0;
float vitesseX = 0.0;
float vitesseY = 0.0;
 
// --- BROCHES ---
const int pinPotX   = A0;
const int pinPotY   = A1;
const int pinBouton = 2;
 
unsigned long dernierTemps;
 
// --- PARAMÈTRES PHYSIQUES DU MODÈLE ---
const float INERTIE          = 1.5;
const float FROTTEMENT       = 0.8;
const float FORCE_GRAVITE    = 30.0;
const float FACTEUR_COUPLAGE = 0.20;
const float COEFF_GYRO       = 0.05;
const float ZONE_MORTE       = 20;
const float DT_MAX           = 0.05;
 
// ================================================================
void resetHelice() {
  angleX   = 0.0;
  angleY   = 0.0;
  vitesseX = 0.0;
  vitesseY = 0.0;
}
 
// --- Barre de progression ASCII ---
String barreAngle(float valeur, float mini, float maxi, int largeur) {
  int pos = (int)((valeur - mini) / (maxi - mini) * (largeur - 1));
  pos = constrain(pos, 0, largeur - 1);
  int centre = (largeur - 1) / 2;
 
  String barre = "[";
  for (int i = 0; i < largeur; i++) {
    if (i == pos)         barre += "O";
    else if (i == centre) barre += "|";
    else                  barre += "-";
  }
  barre += "]";
  return barre;
}
 
// --- Formate un float avec signe forcé ---
String avecSigne(float val, int decimales) {
  String s = "";
  if (val >= 0) s += "+";
  s += String(val, decimales);
  return s;
}
 
// --- Affiche le panneau complet ---
void afficherPanneau(float aX, float aY, float vX, float vY,
                     float fX, float fY, bool reset) {
 
  Serial.print("\033[2J\033[H");
 
  Serial.println(F("╔══════════════════════════════════════════╗"));
  Serial.println(F("║         H É L I C E - 3 1               ║"));
  Serial.println(F("║    Simulateur 2 DOF — Tangage / Lacet    ║"));
  Serial.println(F("╠══════════════════════════════════════════╣"));
 
  // ── Axe X (Lacet) — 0° à 360° ──
  Serial.println(F("║  AXE X — LACET                           ║"));
  Serial.print  (F("║  Angle  : "));
  String aXstr = String(aX, 1) + " deg   ";
  Serial.print(aXstr.substring(0, 10));
  Serial.println(F("                 ║"));
 
  Serial.print  (F("║  "));
  Serial.print  (barreAngle(aX, 0.0, 360.0, 38));  // 0° à 360°
  Serial.println(F(" ║"));
 
  Serial.print  (F("║  Vitesse: "));
  String vXstr = avecSigne(vX, 2) + " deg/s   ";
  Serial.print(vXstr.substring(0, 14));
  Serial.println(F("             ║"));
 
  Serial.print  (F("║  Force  : "));
  String fXstr = avecSigne(fX, 1) + " N.m   ";
  Serial.print(fXstr.substring(0, 14));
  Serial.println(F("             ║"));
 
  Serial.println(F("╠══════════════════════════════════════════╣"));
 
  // ── Axe Y (Tangage) — -50° à +50° ──
  Serial.println(F("║  AXE Y — TANGAGE                         ║"));
  Serial.print  (F("║  Angle  : "));
  String aYstr = avecSigne(aY, 1) + " deg   ";
  Serial.print(aYstr.substring(0, 10));
 
  if (aY >= 49.9)       Serial.println(F("  [!] BUTEE HAUTE (+50)  ║"));
  else if (aY <= -49.9) Serial.println(F("  [!] BUTEE BASSE (-50)  ║"));
  else                  Serial.println(F("                         ║"));
 
  Serial.print  (F("║  "));
  Serial.print  (barreAngle(aY, -50.0, 50.0, 38));
  Serial.println(F(" ║"));
 
  Serial.print  (F("║  Vitesse: "));
  String vYstr = avecSigne(vY, 2) + " deg/s   ";
  Serial.print(vYstr.substring(0, 14));
  Serial.println(F("             ║"));
 
  Serial.print  (F("║  Force  : "));
  String fYstr = avecSigne(fY, 1) + " N.m   ";
  Serial.print(fYstr.substring(0, 14));
  Serial.println(F("             ║"));
 
  Serial.println(F("╠══════════════════════════════════════════╣"));
 
  // ── Statut ──
  if (reset) {
    Serial.println(F("║  [ RESET ]  Angles remis à 0,0           ║"));
  } else {
    Serial.println(F("║  Statut : EN COURS                       ║"));
  }
  Serial.println(F("╚══════════════════════════════════════════╝"));
}
 
// ================================================================
void setup() {
  Serial.begin(9600);
  pinMode(pinBouton, INPUT_PULLUP);
  dernierTemps = millis();
 
  Serial.print("\033[2J\033[H");
  Serial.println(F("╔══════════════════════════════════════════╗"));
  Serial.println(F("║         H É L I C E - 3 1               ║"));
  Serial.println(F("║         Initialisation...                ║"));
  Serial.println(F("╚══════════════════════════════════════════╝"));
  delay(1500);
}
 
// ================================================================
void loop() {
 
  bool resetEffectue = false;
 
  // ── 0. Bouton RESET ──────────────────────────────────────────────
  if (digitalRead(pinBouton) == LOW) {
    resetHelice();
    resetEffectue = true;
    delay(200);
  }
 
  // ── 1. Temps ─────────────────────────────────────────────────────
  unsigned long tempsActuel = millis();
  float dt = (tempsActuel - dernierTemps) / 1000.0;
  dernierTemps = tempsActuel;
  if (dt > DT_MAX) dt = DT_MAX;
 
  // ── 2. Lecture des potentiomètres ────────────────────────────────
  int potX = analogRead(pinPotX);
  int potY = analogRead(pinPotY);
 
  // ── 3. Mise à l'échelle en force ─────────────────────────────────
  float forceHéliceX = ((float)(potX - 512) / 512.0) * 100.0;
  float forceHéliceY = ((float)(potY - 512) / 512.0) * 100.0;
 
  if (abs(potX - 512) < ZONE_MORTE) forceHéliceX = 0.0;
  if (abs(potY - 512) < ZONE_MORTE) forceHéliceY = 0.0;
 
  // ── 4. Moteur physique ───────────────────────────────────────────
  float couplageTotal = (forceHéliceY * FACTEUR_COUPLAGE)
                      + (vitesseY     * COEFF_GYRO);
 
  float forceX_Totale = forceHéliceX + couplageTotal;
  float forceY_Totale = forceHéliceY - FORCE_GRAVITE;
 
  float accX = (forceX_Totale - vitesseX * FROTTEMENT) / INERTIE;
  float accY = (forceY_Totale - vitesseY * FROTTEMENT) / INERTIE;
 
  vitesseX += accX * dt;
  angleX   += vitesseX * dt;
 
  // ── Normalisation 0° → 360° ──────────────────────────────────────
  angleX = fmod(angleX, 360.0);
  if (angleX < 0) angleX += 360.0;
 
  vitesseY += accY * dt;
  angleY   += vitesseY * dt;
 
  // ── 5. Butées inélastiques sur Y ─────────────────────────────────
  if (angleY >= 50.0)  { angleY =  50.0; if (vitesseY > 0.0) vitesseY = 0.0; }
  if (angleY <= -50.0) { angleY = -50.0; if (vitesseY < 0.0) vitesseY = 0.0; }
 
  // ── 6. Affichage ─────────────────────────────────────────────────
  afficherPanneau(angleX, angleY, vitesseX, vitesseY,
                  forceHéliceX, forceHéliceY, resetEffectue);
 
  delay(20);
}
