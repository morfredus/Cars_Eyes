# Documentation des Patterns NeoPixel - Yeux Animés

**Version:** 1.5.20  
**Version minimale:** 1.0.0  
**Langage:** Français  
**Date de révision:** 14 janvier 2026  
**Améliorations:** Patterns réalistes avec effets d'ombre 3D

---

## Vue d'ensemble du système

### Architecture des couleurs
Le système d'affichage NeoPixel utilise **3 niveaux de couleur** pour créer des effets réalistes et de profondeur:

| Niveau | Code | Description |
|--------|------|-------------|
| **0** | `0` | Noir / Arrêt - Pixel désactivé |
| **1** | `1` | Couleur primaire - Iris/sclérotique principale |
| **2** | `2` | Couleur secondaire - Highlights/paupières/reflets |
| **3** | `3` | Couleur tertiaire - Ombres/pupille/profondeur |

### Configuration des couleurs par défaut
```cpp
primaryColor   = 0x00FF4500   // Orange vif (iris - Cars style)
secondaryColor = 0x00FFFFFF   // Blanc (highlights, paupières)
tertiaryColor  = 0x001a0900   // Marron foncé/noir (ombres, pupille)
```

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
**État:** Pupille décalée à gauche  
**Angle:** ~90° gauche

```
0,0,0,0,0,0,0,0
0,0,2,2,1,0,0,0    ← Paupière avec iris à l'extrémité
2,2,3,3,1,2,0,0    ← Pupille gauche (3 = ombre)
2,2,3,3,1,2,0,0    ← Centre pupille (intense)
2,2,1,1,1,2,0,0    ← Iris inférieur
0,0,2,2,1,0,0,0    ← Paupière bas
0,0,0,1,0,0,0,0    ← Détail de cil
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Décalage gauche: Pupille aux bords
- ✓ Ombrage latéral: Profondeur avec (3)
- ✓ Réalisme: Paupière externe disparaît

---

### **#05 - PATTERN_LOOK_RIGHT: Regard vers la droite**
**État:** Pupille décalée à droite  
**Angle:** ~90° droite

```
0,0,0,0,0,0,0,0
0,0,0,1,2,2,2,0    ← Paupière avec iris à l'extrémité
0,0,1,1,3,3,2,2    ← Pupille droite (3 = ombre)
0,0,1,1,3,3,2,2    ← Centre pupille
0,0,1,1,1,2,2,2    ← Iris inférieur
0,0,0,1,2,2,2,0    ← Paupière bas
0,0,0,0,1,0,0,0    ← Détail de cil
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Miroir de LOOK_LEFT
- ✓ Pupille au bord droit
- ✓ Ombrage opposé

---

### **#06 - PATTERN_LOOK_UP: Regard vers le haut**
**État:** Pupille décalée vers le haut  
**Angle:** ~90° haut

```
0,0,2,2,2,2,0,0    ← Paupière supérieure relevée
0,2,3,3,3,3,2,0    ← Pupille supérieure (dilatée)
0,2,3,3,3,3,2,0    ← Centre pupille (intense)
0,2,1,1,1,1,2,0    ← Iris portion inférieure
0,0,2,2,2,2,0,0    ← Paupière inférieure
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Paupière haute levée
- ✓ Pupille vers le haut
- ✓ Espaces vides bas

---

### **#07 - PATTERN_LOOK_DOWN: Regard vers le bas**
**État:** Pupille décalée vers le bas  
**Angle:** ~90° bas

```
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,2,2,2,2,0,0    ← Paupière supérieure
0,2,1,1,1,1,2,0    ← Iris portion supérieure
0,2,3,3,3,3,2,0    ← Pupille centre (vers bas)
0,2,3,3,3,3,2,0    ← Pupille inférieure (intense)
0,0,2,2,2,2,0,0    ← Paupière inférieure abaissée
0,0,0,0,0,0,0,0
```

**Caractéristiques:**
- ✓ Miroir de LOOK_UP
- ✓ Paupière basse abaissée
- ✓ Pupille vers le bas

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


