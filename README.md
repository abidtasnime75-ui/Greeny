# 🌱 Bloomy — Serre Connectée Intelligente

> **Gérez votre serre intelligente en toute simplicité.**

Bloomy est un système IoT complet de gestion de serre agricole connectée. Il combine un microcontrôleur **ESP32**, une base de données cloud **Firebase**, et une application mobile **Flutter** pour surveiller et contrôler en temps réel les conditions environnementales d'une serre.

---

##  Aperçu

| Écran de connexion | Contrôle Manuel |
|---|---|
| ![Login](assets/login.png) | ![Control](assets/control.png) |

---
## 🎨 Design UI/UX

> Interface conçue sur Figma avec prototype interactif complet.

[![Figma Design](https://img.shields.io/badge/Figma-Voir%20le%20prototype-F24E1E?style=for-the-badge&logo=figma&logoColor=white)](https://www.figma.com/make/DGyKfKnE46p9pNNWF9Xyeo/Smart-Greenhouse-App?fullscreen=1)

##  Architecture du Système

```
![Architecture Système](assets/architecture.png)
```

Le système est divisé en **trois composantes principales** :

| Couche | Technologies |
|---|---|
| 🔧 Hardware | ESP32 WROOM 32D, DHT11, Capteur sol, RFID PN532, Servo SG90, Relais 4 canaux, LCD I2C 16×2 |
| ☁️ Cloud | Firebase Firestore, Firebase Auth, Firebase FCM, Firebase Storage |
| 📱 Software | Flutter (Android/iOS), Architecture MVVM |

---

##  Fonctionnalités

-  **Surveillance en temps réel** — Température, humidité air et humidité du sol
- **Mode automatique** — Arrosage automatique si sol sec, ventilation si température > 30°C
-  **Contrôle manuel** — Activation à distance de la pompe, du ventilateur et des LEDs depuis l'app
-  **Accès RFID** — Ouverture de la porte via badge, avec journal des entrées
- **Alertes push** — Notifications FCM pour les événements critiques
-  **Affichage local** — Écran LCD I2C affichant les données en direct
-  **Authentification** — Connexion sécurisée via Firebase Auth

---

##  Structure du Projet

```
bloomy/
├── firmware/
│   └── pfa-final.ino          # Code Arduino ESP32
├── app/                        # Application Flutter
│   ├── lib/
│   │   ├── models/            # SensorData, ActuatorData
│   │   ├── viewmodels/        # Logique MVVM
│   │   └── views/             # Écrans UI
│   └── pubspec.yaml
├── assets/
│   ├── Block_diagram.png
│   ├── cas_dutilisation.png
│   └── Serre-connectée.docx   # Rapport complet
└── README.md
```

---

## 🔌 Tableau des Pins ESP32

| Composant | Pin Module | Pin ESP32 | Remarque |
|---|---|---|---|
| DHT11 | DATA | GPIO 4 | Temp + Humidité air |
| LCD I2C | SDA / SCL | GPIO 21 / 22 | Bus I2C partagé |
| Capteur sol | AO | GPIO 34 | Entrée analogique |
| Relais Pompe | IN1 | GPIO 23 | Actif bas |
| Relais Ventilateur | IN2 | GPIO 19 | Actif bas |
| LED croissance | — | GPIO 5 | — |
| Servo (porte) | SIGNAL | GPIO 18 | Alimentation 5V externe |
| RFID PN532 | SDA / SCL | GPIO 21 / 22 | Partagé avec LCD |

---

##  Seuils Automatiques

| Paramètre | Seuil | Action déclenchée |
|---|---|---|
| Température | > 30°C | Activation ventilateur |
| Humidité sol | > 3000 (brut ADC) | Activation pompe |

---


##  Installation & Configuration

### 1. Firmware ESP32

**Bibliothèques Arduino requises :**
```
WiFi.h
HTTPClient.h
ArduinoJson
DHT sensor library
LiquidCrystal_I2C
ESP32Servo
```

**Configuration dans `pfa-final.ino` :**
```cpp
#define WIFI_SSID       "votre_ssid"
#define WIFI_PASSWORD   "votre_mot_de_passe"
#define FIREBASE_PROJECT_ID "votre-projet-firebase"
#define API_KEY         "votre-api-key-firebase"
#define DEVICE_ID       "esp32_serre_01"
```

Flasher via Arduino IDE (sélectionner la carte **ESP32 Dev Module**).

---

### 2. Firebase

1. Créer un projet Firebase sur [console.firebase.google.com](https://console.firebase.google.com)
2. Activer **Firestore Database**, **Authentication** (email/password) et **FCM**
3. Créer les collections :
   - `sensors/{DEVICE_ID}` — données capteurs
   - `actuators/{DEVICE_ID}` — commandes actionneurs
   - `rfid_logs/` — journal des accès RFID
4. Télécharger `google-services.json` et le placer dans `app/android/app/`

---

### 3. Application Flutter

```bash
# Cloner le repo
git clone https://github.com/votre-org/bloomy.git
cd bloomy/app

# Installer les dépendances
flutter pub get

# Lancer l'application
flutter run
```

**Dépendances principales :**
```yaml
dependencies:
  firebase_core: ^latest
  cloud_firestore: ^latest
  firebase_auth: ^latest
  firebase_messaging: ^latest
  provider: ^latest
```

---

## 📱 Écrans de l'Application

| Écran | Description |
|---|---|
| Connexion | Authentification email/mot de passe |
| Dashboard | Données temps réel (temp, humidité, sol) |
| Contrôle Manuel | Activation/désactivation pompe, ventilo, LEDs, porte |
| Alertes | Historique des notifications push |
| Journal RFID | Qui / quand / combien d'entrées |

---

## 🏗️ Architecture Logicielle (MVVM)

```
View (Flutter UI)
    │
    ▼
ViewModel (Provider)
    │
    ▼
Model (SensorData / ActuatorData)
    │
    ▼
Firebase Service (Firestore REST API)
    │
    ▼
ESP32 ◄──────────────────────────────
```

---

## 👩‍💻 Équipe

| Nom | Rôle |
|---|---|
| Wala OUAILI | |
| Ela ALOUI |  |
| Asma BELLALAH | |
| Zeineb HENCHIRI |  |
| Tasnim ABID |  |

**Encadrant :** Prof. Faouzi MOUSSA  
**Spécialité :** Licence en Ingénierie des Systèmes Informatiques — 2025/2026

---

## 📄 Licence

Ce projet est réalisé dans le cadre d'un **Projet de Fin d'Année (PFA)** académique.

---

*Bloomy — Smart Greenhouse Management System 🌿*
