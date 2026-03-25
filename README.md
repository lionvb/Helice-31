# 🚁 Hélice 31 — Démonstrateur Hélicoptère Asservi

> Maquette transportable de démonstration des compétences en automatique et systèmes embarqués, destinée aux salons et Journées Portes Ouvertes (JPO).

---

## 📋 Présentation du projet

Ce projet consiste à concevoir et réaliser un **démonstrateur interactif en forme d'hélicoptère** capable d'illustrer les compétences E2 en automatique (asservissement PID) et en systèmes embarqués (acquisition, traitement temps réel, commande moteur). La maquette est dimensionnée pour être facilement transportable (≈ 30 × 30 cm) et peut être manipulée en direct par les visiteurs via un boîtier de commande.

---

## 🎯 Objectifs

- Démontrer un **asservissement sur deux axes** (tanguage + anti-couple, comme un quadricoptère)
- Permettre le **réglage en temps réel** des gains PID (Kp, Ki, Kd) par potentiomètres
- Offrir une interface de commande intuitive (potentiomètre ou manette radiofréquence)
- Assurer une **commande numérique temps réel** avec une période d'échantillonnage définie par interruption timer
- Basculer entre correcteur **analogique** et **numérique** 


---

## 🏗️ Architecture du système

```
┌─────────────────────────────────────────────────────────────┐
│                    BOÎTIER DE COMMANDE                       │
│   [Potentiomètre consigne]  [Pot. Kp]  [Pot. Ki]  [Pot. Kd] │
│   [Manette RF optionnelle]                                   │
└─────────────────────────┬───────────────────────────────────┘
                          │  (consigne + gains)
                          ▼
              ┌───────────────────────┐
              │       ARDUINO         │
              │  ┌─────────────────┐  │
              │  │  Timer / IRQ    │  │  ← Période d'échantillonnage fixe
              │  │  Correcteur PID │  │
              │  │  numérique      │  │
              │  └────────┬────────┘  │
              └───────────┼───────────┘
                          │
          ┌───────────────┼───────────────┐
          ▼               ▼               ▼
   [Moteur principal]  [Rotor anticouple]  [LEDs état]
   (tanguage / lift)   (anti-couple)    (saturation, etc.)
          │
          ▼
   [MPU-6050 — mesure angles]
          │
          └──────────────► feedback vers Arduino 
```

### Logique de commande anti-couple 

| Action sur le rotor anticouple | Effet |
|---|---|
| Diminution de la vitesse | Rotation plus lente dans le sens du couple réactif |
| Augmentation de la vitesse | Rotation dans le sens inverse (compensation accrue) |

---

## 🔧 Matériel

| Composant | Rôle | Remarques |
|---|---|---|
| **Arduino** | Unité de traitement centrale | Gestion timers, PID, PWM |
| **MPU-6050** | Gyroscope / accéléromètre | Mesure des angles à intervalle régulier |
| **Moteurs Brushless** | Propulsion (principal + rotor de queue) | ⚠️ Initialiser à 0 pour déverrouiller la sécurité ESC |
| **ESC** | Contrôleurs moteurs brushless | Signal PWM depuis Arduino |
| **Potentiomètres (×4 min.)** | Consigne + réglage Kp, Ki, Kd | Réglage en direct |
| **Manette RF** | Commande sans fil optionnelle | Module RF à intégrer |
| **Batterie LiPo** | Alimentation moteurs | En premier lieu : générateur de labo |
| **Hélices** | 2 modèles disponibles | Format dirigeable et format drone |
| **LEDs** | Indicateurs d'état | Saturation commande, modes, alarmes |
| **Structure** | Châssis de la maquette | Impression 3D — ≈ 30 × 30 cm |

---

## 💻 Logiciel & Algorithmes

### Boucle de contrôle

La boucle de régulation tourne à **période fixe**, déclenchée par une interruption timer sur l'Arduino :

```
IRQ Timer (période T)
  ├── Lecture MPU-6050 (angle mesuré θ)
  ├── Lecture potentiomètre consigne (θ_ref)
  ├── Lecture Kp, Ki, Kd (potentiomètres dédiés)
  ├── Calcul erreur : e = θ_ref - θ
  ├── Calcul correcteur PID numérique
  ├── Saturation + indication LED
  └── Envoi commande PWM vers ESC
```

---

## 🔩 Points critiques de conception

- **Frottements aux jonctions** : à minimiser impérativement pour ne pas perturber la dynamique
- **Légèreté du montage** : impact direct sur les performances - privilégier l'impression 3D 
- **Sécurité ESC brushless** : les moteurs doivent être initialisés à **0** (signal neutre) pour déverrouiller la sécurité électronique

---

## 📐 Modélisation

Le projet inclut une **double modélisation** :
- Modèle **mécanique / physique** de la maquette (dynamique des rotors, couples, inerties)
- Modèle **Matlab** pour la simulation et la synthèse des correcteurs

---

## 💡 Indicateurs LED

| LED | Signification |
|---|---|
| 🔴 Rouge | Saturation de la commande moteur |


---

## 📅 Planning

| Date | Séance | Contenu |
|---|---|---|
| 19/03 | S1 | Spécification & planning |
| 26/03 | S2 | Étude, CAO, Arduino, schéma global |
| 02/04 | S3 | Intégration / Consigne |
| 23/04 | S4 | Modélisation |
| 30/04 | S5 | Soutenance + Making-off|

---

## ⚠️ Règles de sécurité

1. **Ne jamais tester avec des hélices montées sans la présence d'un enseignant.**
2. Toujours initialiser les ESC à la position neutre (commande = 0) avant mise sous tension.
3. En premier prototype, utiliser un **générateur de laboratoire** (pas de batterie LiPo).
4. Vérifier les réglages PID en simulation Matlab avant de les appliquer sur la maquette réelle.
5. S'assurer que la structure est solidement fixée avant tout essai moteur.

---

## 📚 Références & Ressources

- [Maquette existante — Lien 1](#) *(à compléter)*
- [Maquette existante — Lien 2](#) *(à compléter)*
- Documentation MPU-6050 (InvenSense)
- Documentation Arduino (timers, interruptions)
- Datasheet ESC brushless utilisé

---

*Projet **Hélice 31** — réalisé dans le cadre de la formation — Compétences E2 — Automatique & Systèmes Embarqués en collaboration avec M. Trenchant*