# Journal des changements

Tous les changements notables de ce projet sont documentés dans ce fichier.
Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/).

---

## [1.5.0] - 2026-01-15

### Ajouté
- **Clignotants**: Nouvelle fonctionnalité activée par les boutons physiques.
  - Bouton 1 : Clignotant Gauche (Flèche Gauche).
  - Bouton 2 : Clignotant Droit (Flèche Droite).
  - Bouton 1+2 : Mode Warning (Les deux flèches).
  - Appui Court : Actif pour une durée configurable.
  - Appui Long : Actif indéfiniment jusqu'à annulation.
- **Sauvegarde Mémoire**: Les paramètres sont maintenant sauvegardés en mémoire NVS et restaurés au démarrage.
  - Sauvegarde : Luminosité, Couleurs, Mode Animation, Thème, Durée clignotant.
- **Configuration Web UI**: Ajout d'un slider pour configurer la durée du clignotant (défaut 3000ms).

## [1.4.2] - 2026-01-15

### Corrigé
- **Mise en page Écran Principal**: Déplacement de la ligne de séparation et du statut "MODE" vers le bas pour éviter le chevauchement avec la ligne d'adresse mDNS.

## [1.4.1] - 2026-01-15

### Modifié
- **Animation IDLE**: Ajout d'un clignement aléatoire des yeux en mode IDLE (toutes les 3 à 8 secondes) pour un rendu plus vivant.
- **Animation BLINK**: Logique mise à jour pour toujours revenir à l'état IDLE après exécution, évitant les boucles de clignement infinies.

## [1.4.0] - 2026-01-15

### Ajouté
- **Interface Thème Cars**: Redesign complet de l'écran de démarrage et de l'écran principal avec l'esthétique du film "Cars".
- **Écran de Démarrage**: Ajout d'un éclair animé style Flash McQueen et fond rouge.
- **Écran Principal**: Ajout d'un motif damier, meilleure mise en page et statut WiFi visuel.
- **Barre de Progression**: Amélioration du style visuel de la barre de progression de connexion.

## [1.3.2] - 2026-01-15

### Corrigé
- **Bug d'Animation**: Correction du "gel" lors du changement manuel d'effet en priorisant l'entrée utilisateur sur le minuteur AutoPlay.
- **Animation LOOK_UP**: Correction du motif `LOOK_UP` pour être le miroir exact de `LOOK_DOWN` (iris montant) par souci de cohérence.
- **Latence**: Suppression du délai de trame lors du changement d'effet pour une réponse instantanée.

## [1.3.1] - 2026-01-15

### Corrigé
- **Ecran LCD**: Correction de l'écran noir sur ESP32-S3 en basculant le rétroéclairage du PWM vers une commande numérique directe.
- **Animations Neopixel**: Implémentation des motifs manquants SAD (Triste), SLEEP (Dormir) et SURPRISED (Surpris).
- **Performance**: Correction des gels d'animation dans la boucle AutoPlay.

### Modifié
- **Animation IDLE**: Rendue dynamique avec des mouvements subtils des paupières.
- **Logique Regard**: Amélioration de LOOK_UP/LOOK_DOWN pour un rendu plus réaliste.

## [1.3.0] - 2026-01-15

### Ajouté
- **Moteur de Rendu**: Réécriture complète de `neopixel.cpp` pour supporter l'anti-aliasing (luminosité partielle).
- **Thèmes**: Ajout des thèmes "Cars" (Flash McQueen), "Réaliste" et "Élégant".

## [1.1.0] - 2026-01-14

### Changement 5 - Amélioration : Pattern BLINK plus expressif
**Fichier:** `src/neopixel.cpp` (lignes 47-56)  
**Problème:** Le pattern BLINK original utilisait une simple ligne horizontale plate (rows 3-4 uniquement), ce qui n'était pas assez expressif pour simuler un clignotement naturel d'œil.  
**Solution:** Redesign complet du pattern pour créer une forme arrondie d'œil fermé s'étendant sur 6 rangées actives (rows 1-6). Le pattern montre maintenant les paupières supérieure et inférieure convergeant vers un point central de fermeture.  
**Impact:** Animation de clignotement beaucoup plus naturelle et expressive, améliorant la personnalité des yeux animés.

### Changement 4 - Correction : Fonction xyToIndex() pour numérotation linéaire des LEDs
**Fichier:** `src/neopixel.cpp` (lignes 155-164)  
**Problème:** La fonction xyToIndex() implémentait une compensation zigzag (mirroring des rangées impaires), mais l'utilisateur a révélé que les matrices LED sont câblées linéairement (0-63 dans l'ordre de lecture).  
**Solution:** Simplification complète de xyToIndex() pour retourner `y * NEOPIXEL_MATRIX_WIDTH + x`, supprimant toute logique de zigzag.  
**Impact:** Tous les patterns s'affichent désormais correctement sans décalage de rangées paires/impaires.

### Changement 3 - Correction : Patterns LOOK_LEFT et LOOK_RIGHT
**Fichier:** `src/neopixel.cpp` (lignes 60-81)  
**Problème:** Les patterns LOOK_LEFT et LOOK_RIGHT ne suivaient pas la numérotation linéaire des LEDs et ne déplaçaient pas l'œil entier comme LOOK_UP/LOOK_DOWN le faisaient.  
**Solution:** Redesign complet des deux patterns pour déplacer un œil rond complet (comme LOOK_UP/DOWN). LOOK_LEFT positionne l'œil aux colonnes 1-4, LOOK_RIGHT aux colonnes 3-6. Patterns conçus pour numérotation linéaire.  
**Impact:** Les yeux se déplacent correctement à gauche et à droite avec un mouvement fluide et symétrique.

### Changement 2 - Amélioration : Contraste du texte dans l'interface Web
**Fichier:** `src/web_ui.cpp` (styles CSS, lignes ~20-55)  
**Problème:** Les textes des informations système (Brightness: 32, Auto-Play Mode, ESP32-S3, 240 MHz, etc.) n'étaient pas lisibles à cause d'un contraste insuffisant (texte gris par défaut sur fond blanc).  
**Solution:** Amélioration du CSS avec couleurs plus foncées (#222 pour .card div, #333 pour labels) et ajout de font-weight (500-600) pour les éléments clés (.card strong, .color-group label, .slider-label, button).  
**Impact:** Tous les textes de l'interface Web sont maintenant clairement lisibles.

### Changement 1 - Correction : Fonction setColor() obsolète
**Fichier:** `src/buttons.cpp` (ligne 41)  
**Problème:** Le code appelait `NeoPixel::setColor()` qui n'existe plus dans l'API refactorisée du module NeoPixel.  
**Solution:** Remplacement par `NeoPixel::setStatusLed(NeoPixel::makeColor(0, 0, 255))` pour définir la LED de statut en bleu avant redémarrage.  
**Impact:** Correction de l'erreur de compilation, le système compile maintenant avec succès.

---

## [1.0.0] - 2026-01-13

### Création du projet NeoPixel Eyes
Première version stable du contrôleur d'yeux animés avec matrices NeoPixel 8x8.

### Matériel supporté
- **ESP32-S3 DevKitC-1 N16R8** (16MB Flash, 8MB PSRAM)
- **2x Matrices NeoPixel 8x8** (128 LEDs WS2812B au total)
- **Écran TFT ST7789** (240x240 pixels)
- **LED de statut** (GPIO48)
- **Boutons physiques** (reset/reboot)

### Fonctionnalités principales

#### 1. Système d'animation
- 13 types d'animations : IDLE, BLINK, WINK_LEFT, WINK_RIGHT, LOOK_LEFT, LOOK_RIGHT, LOOK_UP, LOOK_DOWN, HAPPY, SAD, ANGRY, SURPRISED, SLEEP
- Système basé sur patterns 8x8 (0=éteint, 1=couleur primaire, 2=couleur secondaire)
- Mode auto-play avec animations aléatoires toutes les 3-5 secondes
- Contrôle de luminosité (0-255)
- Configuration des couleurs primaire (contour) et secondaire (remplissage)

#### 2. Interface Web moderne
- Design gradient violet (#667eea à #764ba2)
- Contrôle en temps réel des animations
- Réglage de la luminosité avec slider
- Sélection des couleurs primaire/secondaire
- Affichage des informations système (ESP32-S3, CPU, RAM, Flash, IP, SSID)
- API REST avec 5 endpoints :
  - `/api/eyes/status` - État du système
  - `/api/eyes/animation` - Contrôle des animations
  - `/api/eyes/brightness` - Réglage luminosité
  - `/api/eyes/color` - Configuration couleurs
  - `/api/eyes/autoplay` - Mode lecture automatique
- Mises à jour temps réel via fetch API (intervalle 2 secondes)

#### 3. Écran TFT ST7789
- Affichage des informations réseau (IP, SSID, hostname mDNS)
- Statut WiFi et mDNS dynamiques
- Informations système (version, fréquence CPU, RAM)
- Interface visuelle pour monitoring

#### 4. Connectivité réseau
- WiFiMulti avec support multi-réseaux
- Service mDNS (découverte réseau local)
- OTA (mise à jour Over-The-Air)
- Gestion robuste des déconnexions

#### 5. Architecture modulaire
Code séparé en modules indépendants avec namespaces :
- `tft_display.h/cpp` - Affichage ST7789 et rendu
- `web_ui.h/cpp` - Construction pages HTML et JSON
- `web_styles.h/cpp` - Styles CSS et JavaScript
- `web_server.h/cpp` - Serveur HTTP et routes
- `network.h/cpp` - WiFi, mDNS, OTA
- `neopixel.h/cpp` - Gestion LEDs NeoPixel
- `buttons.h/cpp` - Entrée boutons et reboot
- `ui_state.h/cpp` - État global interface
- `psram.h/cpp` - Initialisation PSRAM

#### 6. Configuration GPIO (board_config.h)
- **GPIO4** - NeoPixel œil gauche (64 LEDs)
- **GPIO5** - NeoPixel œil droit (64 LEDs)
- **GPIO48** - LED de statut
- **GPIO6** - TFT CS (Chip Select)
- **GPIO7** - TFT DC (Data/Command)
- **GPIO17** - TFT RST (Reset)
- **GPIO18** - TFT SPI SCK (Clock)
- **GPIO16** - TFT SPI MOSI (Data)
- **GPIO0** - Bouton BOOT

#### 7. Sécurité et configuration
- Wrapper de sécurité pour credentials WiFi (`secrets_wrapper.h/cpp`)
- Fichier secrets.h isolé (un seul point d'inclusion)
- Configuration centralisée dans `config.h`
- Validation d'index pour prévenir buffer overflow

#### 8. Documentation complète
- Documentation bilingue (EN/FR) dans `/docs`
- Guide de démarrage (GETTING_STARTED)
- Architecture modulaire (MODULAR_ARCHITECTURE)
- Structure du projet (PROJECT_STRUCTURE)
- Mapping des pins (PIN_MAPPING)
- Système LCD (LCD_SYSTEM)
- Interface Web (WEB_UI)
- Guide utilisateur (USER_GUIDE)
- Ajout de modules (ADD_MODULES)
- Dépendances des modules (MODULE_DEPENDENCIES)

### Spécifications techniques
- **Plateforme:** PlatformIO
- **Framework:** Arduino
- **Standard C++:** C++17
- **Fréquence CPU:** 240 MHz
- **Partition:** 16MB Flash (3MB APP, 9MB FATFS, 3MB OTA)
- **PSRAM:** 8MB OPI (Octal SPI)
- **Bibliothèques:**
  - Adafruit NeoPixel v1.12.0
  - Adafruit GFX Library v1.11.11
  - Adafruit ST7789 Library v1.10.4
  - WiFi, ESPmDNS, WebServer, ArduinoOTA (ESP32 core)

### Build et compilation
- **Environnement:** `esp32s3_n16r8`
- **Répertoire de build:** `C:/pio_builds/build`
- **Temps de compilation:** ~10 secondes
- **Utilisation RAM:** 15.2% (49872/327680 bytes)
- **Utilisation Flash:** 25.1% (838273/3342336 bytes)

### Configuration par défaut
- **Luminosité:** 32/255
- **Couleur primaire:** 0xFF4500 (orange)
- **Couleur secondaire:** 0xFFFFFF (blanc)
- **Animation initiale:** IDLE
- **Auto-play:** Désactivé par défaut
- **Hostname mDNS:** eyes-controller.local
