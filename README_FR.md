# Cars Eyes (Yeux Animés)

> **Version :** 2.0.0  
> **Version minimum :** 2.0.0

**Système d'yeux animés utilisant deux matrices NeoPixel 8x8 pour ESP32-S3, inspiré des personnages du film Cars.**

![ESP32-S3](https://img.shields.io/badge/ESP32-S3-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ready-green)
![License](https://img.shields.io/badge/license-MIT-blue)

## 🎯 Fonctionnalités

- **Matrices NeoPixel 8x8 Doubles** - Contrôle des yeux gauche et droit avec animations indépendantes ou synchronisées
- **17 Animations Intégrées** - IDLE, BLINK, WINK, LOOK (gauche/droite/haut/bas), HAPPY, SAD, ANGRY, SURPRISED, SLEEP, TURN_LEFT, TURN_RIGHT, HAZARD, CUSTOM
- **Clignotants & Warnings** - Contrôle physique (boutons) pour clignotants gauche/droite et mode Warning.
- **Persistance des Réglages** - Sauvegarde automatique (Flash) de la luminosité, des couleurs et de la durée.
- **Interface Web Moderne** - Contrôle en temps réel sans rechargement de page
- **Mode Auto-Play** - Séquences d'animations aléatoires pour un comportement réaliste
- **Personnalisation des Couleurs** - Couleurs primaire/secondaire + palettes 8 couleurs (Cars Orange v2, Human Eye)
- **Contrôle de la Luminosité** - Luminosité ajustable (0-255) pour les deux yeux
  - À faible luminosité (<64), une atténuation perceptuelle réduit davantage les tons très clairs (évite les blancs agressifs)
- **LED de Statut** - LED RGB intégrée pour l'indication du statut WiFi
- **Écran LCD** - Informations système en temps réel sur écran ST7789
- **Mises à Jour OTA** - Mises à jour du firmware sans fil via l'interface web
- **API REST** - API complète pour le contrôle programmatique

## 📋 Matériel Requis

### Composants Nécessaires

- **ESP32-S3 DevKitC-1 N16R8** (avec 16MB Flash + 8MB PSRAM)
- **2x Matrices NeoPixel 8x8** (WS2812B, 64 LEDs chacune)
- **Écran LCD ST7789** (240x320 ou 240x240)
- **Alimentation 5V** (minimum 2A, recommandé 5A pour luminosité maximale)
- **2x Résistances 330Ω** (pour les lignes de données NeoPixel)
- **Condensateur Électrolytique 1000µF** (16V+, pour stabilisation de l'alimentation)
- **2x Condensateurs Céramiques 100nF** (un par matrice, filtrage HF)
- **3x Boutons Poussoirs** (optionnel, pour contrôle manuel)

### Brochage GPIO (ESP32-S3)

| Composant | GPIO | Description |
|-----------|------|-------------|
| NeoPixel Œil Gauche | GPIO19 | Ligne de données NeoPixel (DIN) |
| NeoPixel Œil Droit | GPIO20 | Ligne de données NeoPixel (DIN) |
| LED de Statut | GPIO48 | LED RGB intégrée |
| LCD SCK | GPIO12 | Horloge SPI |
| LCD MOSI | GPIO11 | Données SPI |
| LCD CS | GPIO10 | Sélection chip |
| LCD DC | GPIO9 | Données/Commande |
| LCD RST | GPIO8 | Reset |
| LCD BL | GPIO14 | Rétroéclairage (Numérique) |
| Bouton BOOT | GPIO0 | Bouton boot intégré |
| Bouton Utilisateur 1 | GPIO38 | Bouton personnalisé 1 |
| Bouton Utilisateur 2 | GPIO39 | Bouton personnalisé 2 |

**⚠️ IMPORTANT :** NE PAS alimenter les matrices NeoPixel depuis la broche 5V de l'ESP32. Utilisez une alimentation externe 5V avec capacité de courant suffisante (minimum 2A, jusqu'à 8A pour blanc complet à luminosité maximale).

## 🔌 Schéma de Câblage

```
Alimentation 5V
    │
    ├──[1000µF]──GND
    │
    ├────────────► VCC Matrice Gauche
    │             │
    │             └──[100nF]──GND
    │
    └────────────► VCC Matrice Droite
                  │
                  └──[100nF]──GND

ESP32-S3
    GPIO4 ──[330Ω]──► DIN Matrice Gauche
    GPIO5 ──[330Ω]──► DIN Matrice Droite
    GPIO48 ─────────► LED RGB intégrée
    GND ────────────► GND (tous composants, masse commune)
```

## 🚀 Démarrage Rapide

### 1. Configuration de l'Environnement

```bash
# Cloner le dépôt
git clone <url-de-votre-depot>
cd Cars_Eyes

# Installer PlatformIO (si pas déjà installé)
pip install platformio

# Installer les dépendances
pio pkg install
```

### 2. Configuration WiFi

Copiez `include/secrets_example.h` vers `include/secrets.h` et éditez :

```cpp
#define WIFI_SSID "NomDeVotreReseau"
#define WIFI_PASSWORD "VotreMotDePasse"
```

### 3. Compilation & Upload

```bash
# Pour ESP32-S3 (par défaut)
pio run -e esp32s3_n16r8 -t upload

# Monitorer la sortie série
pio device monitor
```

### 4. Accéder à l'Interface Web

Après un démarrage réussi, l'écran LCD affiche l'adresse IP. Accédez à l'interface web :

```
http://<ADRESSE-IP-ESP32>/
```

Ou utilisez mDNS (si supporté par votre réseau) :

```
http://neopixel-eyes.local/
```

## 🌐 Interface Web

L'interface web fournit un contrôle complet des animations des yeux :

### Contrôles d'Animation
- 17 boutons d'animation avec aperçu en temps réel (inclut TURN_L, TURN_R, HAZARD, CUSTOM)
- Sélecteurs de couleurs primaire et secondaire
- Curseur de luminosité (0-255)
- Activation/désactivation du mode auto-play

### Informations Système
- Statut WiFi et informations réseau
- Utilisation mémoire (Heap et PSRAM)
- Animation et luminosité actuelles
- Interface de mise à jour OTA

### Points d'Accès API

**Obtenir le Statut Système :**
```http
GET /api/status
```

**Obtenir le Statut des Yeux :**
```http
GET /api/eyes/status
```

**Définir l'Animation :**
```http
GET /api/eyes/animation?id=0
```
- `id` : 0=IDLE, 1=BLINK, 2=WINK_LEFT, 3=WINK_RIGHT, 4=LOOK_LEFT, 5=LOOK_RIGHT, 6=LOOK_UP, 7=LOOK_DOWN, 8=HAPPY, 9=SAD, 10=ANGRY, 11=SURPRISED, 12=SLEEP, 13=TURN_LEFT, 14=TURN_RIGHT, 15=HAZARD, 16=CUSTOM

**Définir la Luminosité :**
```http
GET /api/eyes/brightness?value=128
```
- `value` : 0-255

**Définir la Couleur :**
```http
GET /api/eyes/color?type=1&value=16744448
```
- `type` : 1=primaire, 2=secondaire
- `value` : Couleur RGB en entier (0x00RRGGBB)

**Définir l'Auto-Play :**
```http
GET /api/eyes/autoplay?enabled=1
```
- `enabled` : 0=désactivé, 1=activé

## 📖 Documentation

La documentation complète est disponible dans le dossier `docs/` :

- [Guide de Démarrage](docs/GETTING_STARTED_FR.md)
- [Structure du Projet](docs/PROJECT_STRUCTURE_FR.md)
- [Référence Brochage](docs/PIN_MAPPING_FR.md)
- [Ajouter de Nouveaux Modules](docs/ADD_MODULES_FR.md)
- [Documentation UI Web](docs/WEB_UI_FR.md)
- [Guide Utilisateur](docs/USER_GUIDE_FR.md)

Les versions anglaises sont également disponibles (sans suffixe `_FR.md`).

## ⚙️ Configuration

Éditez `include/config.h` pour personnaliser :

```cpp
constexpr SystemConfig kSystemConfig {
  .enableWebUi = true,        // Activer l'interface web
  .enableOta = true,          // Activer les mises à jour OTA
  .backlightLevel = 200,      // Luminosité LCD (0-255)
  .lcdRotation = 0,           // Rotation LCD (0-3)
  .enableBootBar = true,      // Afficher la barre de progression au démarrage
  .mdnsHost = "neopixel-eyes" // Nom d'hôte mDNS
};
```

## 🎨 Personnalisation des Animations

Les motifs d'animation sont définis dans `src/neopixel.cpp`. Chaque motif est un tableau 8x8 :

```cpp
// Valeurs des motifs :
// 0 = OFF (noir)
// 1 = Couleur primaire (contour)
// 2 = Couleur secondaire (remplissage)

// Exemple codé avec la palette (C1..C8 + variantes atténuées)
static const uint8_t PATTERN_CUSTOM[64] = {
  0,71,51,50,50,51,71,0,
  71,51,50,50,50,50,51,71,
  51,41,11,10,10,11,41,51,
  50,10,80,81,81,80,10,50,
  50,10,81,82,82,81,10,50,
  51,41,11,10,10,11,41,51,
  71,51,50,50,50,50,51,71,
  0,71,51,50,50,51,71,0
};
```

Codes : 0=off, 1..8 = emplacements de palette (C1..C8). Pour atténuer une couleur, utilisez l'index de palette comme dizaine et ajoutez éventuellement `1/2/3` pour 70/40/20% (ex : 71 = C7 à 70%, 82 = C8 à 40%). Ajoutez votre motif dans `getPatternForAnimation()` pour l'activer.

## 🔧 Dépannage

**Les yeux ne s'allument pas :**
- Vérifiez l'alimentation (5V, courant suffisant)
- Vérifiez les connexions GPIO (GPIO4 et GPIO5)
- Confirmez la masse commune entre ESP32 et alimentation
- Vérifiez les résistances sur les lignes de données (330Ω)

**Interface web inaccessible :**
- Vérifiez les identifiants WiFi dans `secrets.h`
- Consultez le moniteur série pour l'adresse IP
- Essayez mDNS : `http://neopixel-eyes.local`
- Assurez-vous que le routeur autorise la communication entre appareils locaux

**LCD n'affiche rien :**
- Vérifiez les connexions SPI (GPIO 8-12)
- Vérifiez l'alimentation LCD (3.3V recommandé)
- Ajustez `lcdRotation` dans config.h si l'affichage est à l'envers

**Animations scintillantes :**
- Ajoutez des condensateurs (1000µF sur l'alimentation, 100nF par matrice)
- Réduisez la luminosité dans les situations de fort courant
- Utilisez des fils plus courts pour les lignes de données
- Assurez une alimentation 5V stable

## 📜 Licence

Ce projet est sous licence MIT - voir le fichier [LICENCE](LICENCE) pour plus de détails.

## 🙏 Crédits

- **Bibliothèque Adafruit NeoPixel** - Contrôle des LEDs
- **Bibliothèques Adafruit GFX & ST7789** - Affichage LCD
- **ESP32 Arduino Core** - Framework ESP32

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à soumettre une Pull Request.

## 📞 Support

Pour les questions et le support, veuillez ouvrir une issue sur GitHub.

---

**Construit avec ❤️ pour la communauté maker**





