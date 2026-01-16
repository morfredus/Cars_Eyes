# Documentation des Patterns NeoPixel - Yeux Animés

**Version:** 1.6.2  
**Version minimale:** 1.6.0  
**Langage:** Français  
**Date de révision:** 17 janvier 2026  
**Améliorations:** Palettes 8 couleurs, animations recolorisées, patterns de regard directionnel avec rotations de matrice

---

## Vue d'ensemble du système

### Architecture des couleurs (v1.6.0)
- Les emplacements de palette **C1..C8** correspondent aux codes **1..8** du schéma courant.
- Variantes atténuées : utilisez l'index de palette comme dizaine et ajoutez éventuellement `1/2/3` pour 70/40/20% de luminosité. Exemples : `10` = C1 à 100%, `51` = C5 à 70%, `82` = C8 à 40%.
- Amber override : le code `100` force l'emplacement C1 (utilisé pour les clignotants/warning en ambre imposé).

### Palettes par défaut
- **Cars Orange v2 (8 couleurs)** : C1 0x00FF5500, C2 0x00FFAA55, C3 0x00662200, C4 0x00FFF2E5, C5 0x00E0F4FF, C6 0x000A1A33, C7 0x00CC4400, C8 0x00000000.
- **Human Eye (8 couleurs)** : C1 0x00C48A4A, C2 0x008B5A2B, C3 0x00402010, C4 0x006A7F4F, C5 0x00F7F2E8, C6 0x00E6F7FF, C7 0x00445E80, C8 0x00000000.
- Les schémas 3 couleurs hérités sont auto-étendus : C1=Primaire, C2=Secondaire, C3=Tertiaire, C4=C2@70%, C5=C2@40%, C6=C3@70%, C7=C3@40%, C8=Noir.

> Les grilles ci-dessous illustrent les formes ; le firmware utilise désormais les codes de palette (1..8) et variantes atténuées (10..83) selon le schéma actif.

---

## Catalogue des Patterns (Numérotés)

### **#01 - PATTERN_IDLE: Yeux au repos**
**État:** Regard centré, neutre  
**Usage:** Affichage par défaut au démarrage

```
0,0,0,0,0,0,0,0
0,0,2,2,2,2,0,0    ← Paupière supérieure (highlight)
0,2,1,3,3,1,2,0    ← Iris supérieur avec centre sombre
0,2,1,3,3,1,2,0    ← Centre iris (pupille = 3)
0,2,1,1,1,1,2,0    ← Iris inférieur avec traces de cils
0,0,2,2,2,2,0,0    ← Paupière inférieure
0,0,0,1,1,0,0,0    ← Ligne de cils inférieure
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Profondeur: Pupille sombre (3) entourée d'iris (1)
- ✓ Éclairage: Highlights (2) sur les paupières
- ✓ Détail: Traces de cils en bas
- ✓ Réalisme: Effet 3D avec gradation

---

### **#02 - PATTERN_SLEEP: Yeux fermés (sommeil)**
**État:** Paupières complètement fermées  
**Usage:** Mode sommeil, démarrage inactif

```
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,2,2,2,2,2,2,0    ← Paupière supérieure (highlight)
0,1,1,1,1,1,1,0    ← Ligne principale (coloration)
0,1,1,1,1,1,1,0    ← Épaisseur 3D
0,2,2,2,2,2,2,0    ← Paupière inférieure (highlight)
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Effet 2D épais (impression 3D)
- ✓ Highlights haut/bas pour le relief
- ✓ Transitions douces

---

### **#03 - PATTERN_BLINK: Clignement (passage)**
**État:** Fermeture progressive  
**Durée:** Rapide (~100ms)

```
0,0,0,0,0,0,0,0
0,0,2,2,2,2,0,0    ← Paupière qui baisse
0,2,2,1,1,2,2,0    ← Compression mi-clignement
0,2,2,1,1,2,2,0    ← Iris comprimée
0,2,2,2,2,2,2,0    ← Compression paupière
0,0,2,2,2,2,0,0    ← Paupière inférieure
0,0,0,1,1,0,0,0    ← Léger espace
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Dynamique: Iris comprimée (effet réaliste)
- ✓ Ombrage: Transition douce
- ✓ Animation: Crée un mouvement organique

---

### **#04 - PATTERN_LOOK_LEFT: Regard vers la gauche**
**État:** Iris décalé à gauche avec pupilles clignotantes  
**Angle:** ~90° gauche  
**Technique:** Pattern de base pour les rotations verticales

**FRAME0:**
```
0, 71, 51, 50, 50, 51, 71, 0
71, 51, 50, 50, 50, 50, 51, 71
51, 41, 11, 50, 51, 71, 71, 0
50, 11, 10, 81, 50, 71, 71, 0    ← Iris à gauche, pupilles: 10, 81
50, 10, 11, 80, 50, 71, 71, 0    ← Pupilles clignotantes: 10, 11, 80
51, 41, 11, 50, 51, 71, 71, 0
71, 51, 50, 50, 50, 50, 51, 71
0, 71, 51, 50, 50, 51, 71, 0
```

**FRAME1 (variation clignotement):**
```
0, 71, 51, 50, 50, 51, 71, 0
71, 51, 50, 50, 50, 50, 51, 71
51, 41, 10, 50, 51, 71, 71, 0
50, 81, 80, 81, 50, 71, 71, 0    ← Clignotement: 81, 80, 81
50, 80, 81, 80, 50, 71, 71, 0    ← Pupilles alternées
51, 41, 10, 50, 51, 71, 71, 0
71, 51, 50, 50, 50, 50, 51, 71
0, 71, 51, 50, 50, 51, 71, 0
```

**Caractéristiques:**
- ✓ Décalage gauche : Iris positionné au bord gauche (colonnes 1-3)
- ✓ Pupilles clignotantes : Codes 10/11 (noyau) et 80/81 (clignotement) alternent entre frames
- ✓ Contour préservé : Code 71 maintient la forme de l'œil
- ✓ Sclérotique : Code 50/51 remplit la zone visible à droite

---

### **#05 - PATTERN_LOOK_RIGHT: Regard vers la droite**
**État:** Iris décalé à droite avec pupilles clignotantes  
**Angle:** ~90° droite  
**Technique:** Miroir de LOOK_LEFT

**FRAME0:**
```
0, 71, 51, 50, 50, 51, 71, 0
71, 51, 50, 50, 50, 50, 51, 71
0, 71, 71, 51, 41, 11, 50, 51
0, 71, 71, 50, 81, 11, 10, 50    ← Iris à droite, pupilles: 81, 11, 10
0, 71, 71, 50, 80, 10, 11, 50    ← Pupilles clignotantes: 80, 10, 11
0, 71, 71, 51, 41, 11, 50, 51
71, 51, 50, 50, 50, 50, 51, 71
0, 71, 51, 50, 50, 51, 71, 0
```

**FRAME1 (variation clignotement):**
```
0, 71, 51, 50, 50, 51, 71, 0
71, 51, 50, 50, 50, 50, 51, 71
0, 71, 71, 51, 41, 10, 50, 51
0, 71, 71, 50, 80, 81, 80, 50    ← Clignotement: 80, 81, 80
0, 71, 71, 50, 81, 80, 81, 50    ← Pupilles alternées
0, 71, 71, 51, 41, 10, 50, 51
71, 51, 50, 50, 50, 50, 51, 71
0, 71, 51, 50, 50, 51, 71, 0
```

**Caractéristiques:**
- ✓ Décalage droite : Iris positionné au bord droit (colonnes 4-6)
- ✓ Pupilles clignotantes : Codes 10/11 (noyau) et 80/81 (clignotement) alternent entre frames
- ✓ Structure miroir : Miroir horizontal de LOOK_LEFT
- ✓ Sclérotique : Code 50/51 remplit la zone visible à gauche

---

### **#06 - PATTERN_LOOK_UP: Regard vers le haut**
**État:** Iris décalé vers le haut avec pupilles clignotantes  
**Angle:** ~90° haut  
**Technique:** LOOK_LEFT tourné de 90° sens horaire

**FRAME0:**
```
0, 71, 51, 50, 50, 51, 71, 0
71, 51, 41, 10, 10, 41, 51, 71    ← Iris en haut, pupilles: 10
51, 50, 11, 11, 10, 11, 50, 51    ← Rangées d'iris avec codes 11, 10
50, 50, 50, 80, 81, 50, 50, 50    ← Pupilles clignotantes: 80, 81
50, 50, 51, 50, 50, 51, 50, 50    ← Sclérotique remplissant le bas
51, 50, 71, 71, 71, 71, 50, 51
71, 51, 71, 71, 71, 71, 51, 71
0, 71, 0, 0, 0, 0, 71, 0
```

**FRAME1 (variation clignotement):**
```
0, 71, 51, 50, 50, 51, 71, 0
71, 51, 41, 80, 80, 41, 51, 71    ← Clignotement: 80
51, 50, 10, 80, 81, 10, 50, 51    ← Pupilles: 10, 80, 81
50, 50, 50, 81, 80, 50, 50, 50    ← Alternance: 81, 80
50, 50, 51, 50, 50, 51, 50, 50
51, 50, 71, 71, 71, 71, 50, 51
71, 51, 71, 71, 71, 71, 51, 71
0, 71, 0, 0, 0, 0, 71, 0
```

**Caractéristiques:**
- ✓ Décalage haut : Iris positionné en haut (rangées 1-3)
- ✓ Rotation de matrice : Généré par rotation de LOOK_LEFT à 90° sens horaire
- ✓ Pupilles clignotantes : Codes 10/11 et 80/81 alternent verticalement
- ✓ Sclérotique visible : Moitié inférieure montre la sclérotique (codes 50/51)

---

### **#07 - PATTERN_LOOK_DOWN: Regard vers le bas**
**État:** Iris décalé vers le bas avec pupilles clignotantes  
**Angle:** ~90° bas  
**Technique:** LOOK_LEFT tourné de 90° sens antihoraire

**FRAME0:**
```
0, 71, 0, 0, 0, 0, 71, 0
71, 51, 71, 71, 71, 71, 51, 71
51, 50, 71, 71, 71, 71, 50, 51
50, 50, 51, 50, 50, 51, 50, 50    ← Sclérotique remplissant le haut
50, 50, 50, 81, 80, 50, 50, 50    ← Pupilles clignotantes: 81, 80
51, 50, 11, 10, 11, 11, 50, 51    ← Rangées d'iris avec codes 11, 10
71, 51, 41, 11, 10, 41, 51, 71    ← Iris en bas, pupilles: 11, 10
0, 71, 51, 50, 50, 51, 71, 0
```

**FRAME1 (variation clignotement):**
```
0, 71, 0, 0, 0, 0, 71, 0
71, 51, 71, 71, 71, 71, 51, 71
51, 50, 71, 71, 71, 71, 50, 51
50, 50, 51, 50, 50, 51, 50, 50
50, 50, 50, 80, 81, 50, 50, 50    ← Alternance: 80, 81
51, 50, 10, 81, 80, 10, 50, 51    ← Pupilles: 10, 81, 80
71, 51, 41, 80, 80, 41, 51, 71    ← Clignotement: 80
0, 71, 51, 50, 50, 51, 71, 0
```

**Caractéristiques:**
- ✓ Décalage bas : Iris positionné en bas (rangées 4-6)
- ✓ Rotation de matrice : Généré par rotation de LOOK_LEFT à 90° sens antihoraire
- ✓ Pupilles clignotantes : Codes 10/11 et 80/81 alternent verticalement
- ✓ Sclérotique visible : Moitié supérieure montre la sclérotique (codes 50/51)

---

### **#08 - PATTERN_HAPPY: Expression joyeuse**
**État:** Yeux grand ouverts, heureux  
**Pupilles:** Largement dilatées

```
0,0,2,2,2,2,0,0    ← Paupière supérieure courbée
0,2,1,1,1,1,2,0    ← Sclérotique large
2,1,3,3,2,3,1,2    ← Iris grand ouvert
2,1,3,3,2,3,1,2    ← Centre: reflet lumineux (2) = joie
2,1,3,3,2,3,1,2    ← Iris continue - yeux très ouverts
0,2,1,1,1,1,2,0    ← Sclérotique bas
0,0,2,2,2,2,0,0    ← Paupière inférieure courbée
0,0,0,2,2,0,0,0    ← Rides de joie (pattes d'oie)
```

**Caractéristiques:**
- ✓ Pupilles grandes: Dilatation maximale
- ✓ Reflet central: Lumière de joie (2)
- ✓ Rides: Détail de mimique
- ✓ Réalisme: Yeux pétillants

---

### **#09 - PATTERN_SAD: Expression triste**
**État:** Yeux tristes, paupières baissées  
**Pupilles:** Comprimées vers le bas

```
0,2,2,2,2,2,0,0    ← Paupière supérieure tombante gauche
2,1,1,1,1,1,2,0    ← Sclérotique tombante
1,3,3,3,3,1,1,0    ← Iris comprimée (mélancolie)
1,3,3,3,3,1,1,0    ← Pupille petite (fermée)
0,2,1,1,1,2,0,0    ← Paupière inférieure relevée (triste)
0,0,2,2,2,0,0,0    ← Coin de l'oeil baissé
0,0,0,1,0,0,0,0    ← Larme (détail)
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Compression: Iris rétrécie
- ✓ Angle: Paupières baissées
- ✓ Détail: Larme possible
- ✓ Tonalité: Sombre et introspective

---

### **#10 - PATTERN_ANGRY: Expression furieuse**
**État:** Yeux plissés, agressifs  
**Pupilles:** Comprimées, regard intense

```
1,2,2,1,1,0,0,0    ← Paupière supérieure angled
2,1,3,3,1,2,0,0    ← Iris comprimée (fureur)
2,1,3,3,1,2,0,0    ← Pupille sombre intense (3)
1,1,1,1,1,1,0,0    ← Compacité maximale
0,2,2,1,1,0,0,0    ← Paupière inférieure angled
0,0,2,1,0,0,0,0    ← Angle agressif
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Plissement: Angles asymétriques
- ✓ Compacité: Iris très comprimée
- ✓ Ombrage: Maximum (3) au centre
- ✓ Agressivité: Regard perçant

---

### **#11 - PATTERN_SURPRISED: Expression surprise**
**État:** Yeux extrêmement ouverts  
**Pupilles:** Maximalement dilatées

```
0,2,2,2,2,2,0,0    ← Paupière supérieure très levée
2,1,3,3,3,1,2,0    ← Iris géante dilatée
1,3,3,2,2,3,3,1    ← Pupille énorme (2 = reflet de choc)
1,3,3,2,2,3,3,1    ← Pupille largement ouverte
2,1,3,3,3,1,2,0    ← Iris continue - état d'alerte
0,2,2,2,2,2,0,0    ← Paupière inférieure très abaissée
0,0,2,2,2,0,0,0    ← Extrême expressivité
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Dilatation extrême: Maximum possible
- ✓ Reflets: (2) = lumière de choc
- ✓ Ouverture: Paupières aux extrêmes
- ✓ Émotionnel: Choc/stupéfaction

---
### **#12 - PATTERN_SLEEP: Sommeil profond**
**État:** Yeux fermés hermétiquement  
**Pupilles:** Cachées, lumière minimale

```
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0    ← Paupière supérieure fermée
1,1,1,1,1,1,1,1    ← Couverture oculaire lourde
2,2,2,2,2,2,2,2    ← Reflet de paupière centrale
1,1,1,1,1,1,1,1    ← Couverture inférieure
0,0,0,0,0,0,0,0    ← Bas scellé
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Complètement scellé: Couverture totale des yeux
- ✓ Respiration: Léger pulsement optionnel
- ✓ Réaliste: Imite l'apparence réelle du sommeil
- ✓ Mode repos: Utilisation CPU faible

---

### **#13 - PATTERN_TURN_LEFT: Signal tournant à gauche**
**État:** Flèche pointant vers la gauche  
**Usage:** Clignotant véhicule (gauche)

```
0,0,0,1,0,0,0,0
0,0,1,1,0,0,0,0    ← Pointe de flèche (gauche)
0,1,1,0,0,0,0,0    ← Flèche pointant gauche
1,1,0,0,0,0,0,0    ← Intensité du signal
1,1,0,0,0,0,0,0    ← Activation brillante
0,1,1,0,0,0,0,0
0,0,1,1,0,0,0,0    ← Clarté de direction
0,0,0,1,0,0,0,0
```

**Caractéristiques:**
- ✓ Directionnel: Indication gauche claire
- ✓ Luminosité: Haute intensité pour visibilité
- ✓ Signal véhicule: Intégration sécurité
- ✓ Prêt animation: Peut pulser pour effet clignotant

---

### **#14 - PATTERN_TURN_RIGHT: Signal tournant à droite**
**État:** Flèche pointant vers la droite  
**Usage:** Clignotant véhicule (droite)

```
0,0,0,0,1,0,0,0
0,0,0,0,1,1,0,0    ← Pointe de flèche (droite)
0,0,0,0,0,1,1,0    ← Flèche pointant droite
0,0,0,0,0,0,1,1    ← Intensité du signal
0,0,0,0,0,0,1,1    ← Activation brillante
0,0,0,0,0,1,1,0
0,0,0,0,1,1,0,0    ← Clarté de direction
0,0,0,0,1,0,0,0
```

**Caractéristiques:**
- ✓ Directionnel: Indication droite claire
- ✓ Luminosité: Haute intensité pour visibilité
- ✓ Signal véhicule: Intégration sécurité
- ✓ Prêt animation: Peut pulser pour effet clignotant

---

### **#15 - PATTERN_HAZARD: Feux de détresse**
**État:** Les deux flèches actives (danger/avertissement)  
**Usage:** Signal d'alerte d'urgence

```
1,0,0,0,0,0,0,1    ← Bords gauche et droit
1,1,0,0,0,0,1,1    ← Flèches duales avertissement
0,1,1,0,0,1,1,0    ← Motif centre diamant
0,0,1,1,1,1,0,0    ← Point de convergence (danger)
0,0,1,1,1,1,0,0    ← Intensité centrale
0,1,1,0,0,1,1,0    ← Affichage symétrique
1,1,0,0,0,0,1,1    ← Sensibilisation duelle
1,0,0,0,0,0,0,1
```

**Caractéristiques:**
- ✓ Symétrique: Les deux côtés activés équitablement
- ✓ Avertissement: Motif diamant/convergence
- ✓ Intensité: Luminosité maximale pour alerte
- ✓ Urgence: Distinct des clignotants

---

### **#16 - PATTERN_CUSTOM: Motif défini par l'utilisateur**
**État:** Programmable via interface web  
**Usage:** Animations personnalisées, expressions utilisateur

```
Défini par l'utilisateur (16 valeurs RGB par ligne, 8 lignes)
Configurable via interface web
Mises à jour en temps réel sans redémarrage
Maximum 512 octets par motif personnalisé
```

**Caractéristiques:**
- ✓ Flexible: N'importe quel motif 8×8 pixels possible
- ✓ Interactif: Édition en direct via tableau de bord web
- ✓ Persistant: Sauvegardé en mémoire flash
- ✓ Créatif: Expressions utilisateur illimitées

---
## Schéma de couleurs recommandées

### Configuration #1: Cars (Orange vif)
```cpp
primaryColor   = 0x00FF4500   // Orange Pixar
secondaryColor = 0x00FFFFFF   // Blanc pur
tertiaryColor  = 0x001a0900   // Marron sombre
```

### Configuration #2: Mode nuit (Bleu)
```cpp
primaryColor   = 0x000066FF   // Bleu lumineux
secondaryColor = 0x0099DDFF   // Bleu clair
tertiaryColor  = 0x00001155   // Bleu très sombre
```

### Configuration #3: Mode amical (Vert)
```cpp
primaryColor   = 0x0033FF33   // Vert lime
secondaryColor = 0x00DDFFDD   // Vert très clair
tertiaryColor  = 0x00113311   // Vert très foncé
```

---

## Utilisation dans le code

### Changer d'animation
```cpp
// Dans web_ui.cpp ou autre contrôleur
NeoPixel::setAnimation(AnimationType::HAPPY);  // #08 Happy
```

### Changer les couleurs
```cpp
// Couleur primaire (iris)
NeoPixel::setPrimaryColor(NeoPixel::makeColor(255, 69, 0));  // Orange

// Couleur secondaire (highlights)
NeoPixel::setSecondaryColor(NeoPixel::makeColor(255, 255, 255));  // Blanc

// Couleur tertiaire (ombres)
NeoPixel::setTertiaryColor(NeoPixel::makeColor(26, 9, 0));  // Marron
```

### Activer le lecture auto
```cpp
NeoPixel::setAutoPlay(true);  // Alterne aléatoirement entre animations
```

---

## Notes techniques

### Résolution
- **Matrice:** 8×8 pixels (64 LED totales par oeil)
- **Couleurs par pixel:** 3 bits (0-7 niveaux théoriques, utilisons 0-3)
- **Profondeur:** 24 bits RGB (8 bits par canal)

### Performance
- **Temps d'affichage:** <10ms par oeil
- **Fréquence d'actualisation:** ~100 Hz
- **Mémoire des patterns:** ~11 Ko (11 patterns × 64 bytes)

### Animations futures possibles
- Clignotement automatique
- Transitions fluides entre états
- Réactions en temps réel
- Expressions dynamiques via IA

---

## Améliorations apportées (14/01/2026)

| Avant | Après |
|-------|-------|
| Patterns simples 2 couleurs | Patterns réalistes 3 couleurs |
| Pas de profondeur 3D | Ombrage et highlights réalistes |
| Documentation manquante | Documentation complète numérotée |
| Pas d'effet d'ombre | Shadows et highlights dynamiques |
| Expressions statiques | Expressions émotionnelles nuancées |

---

## Fichier source
[neopixel.cpp](../src/neopixel.cpp) - Définitions des patterns  
[neopixel.h](../include/neopixel.h) - Interface publique





