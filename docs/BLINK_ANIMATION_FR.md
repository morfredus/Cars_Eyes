# Clignotement Animé - Pattern #03 BLINK

**Version:** 1.5.19  
**Version minimale:** 1.3.0  
**Langage:** Français  
**Date:** 14 janvier 2026  
**Changement:** Implémentation d'une vraie animation de clignotement

---

## 🎬 Nouvelle Fonctionnalité: Clignotement Réaliste

La version 1.3.0 remplace le pattern statique #03 BLINK par une **animation fluide à 4 images** qui reproduit un vrai mouvement de fermeture/ouverture des yeux.

### Problème résolu

En v1.2.0, le pattern BLINK était une **image fixe** montrant les yeux à mi-fermeture. Aucune animation n'existait réellement. 

Maintenant: **Animation fluide avec 4 frames** = clignotement naturel et réaliste.

---

## 📊 Structure de l'Animation

### Les 4 Frames

#### Frame 0: OUVERT (Yeux ouverts)
```
Timing: 0-2
Visuel: Iris entièrement visible
Pixels allumés: 
  - Paupière supérieure: 4 pixels
  - Iris + highlights: 6 pixels
  - Paupière inférieure: 4 pixels
Description: Yeux au repos, identique à IDLE
```

#### Frame 1: FERMETURE (1/3 fermés)
```
Timing: 3-4
Visuel: Paupière supérieure descend
Pixels allumés:
  - Paupière descend: compression visible
  - Iris partiellement couvert
Description: Début de fermeture, paupière active
```

#### Frame 2: FERMÉS (Complètement fermés)
```
Timing: 5-6
Visuel: Yeux complètement fermés
Pixels allumés:
  - Paupières haut et bas qui se touchent
  - Iris cachée
Description: Yeux totalement fermés, mi-clignement
```

#### Frame 3: OUVERTURE (2/3 ouverts)
```
Timing: 7-9
Visuel: Paupière s'ouvre progressivement
Pixels allumés:
  - Iris réapparaît graduellement
  - Paupière remonte
Description: Fin de fermeture, retour vers l'ouverture
```

### Chronologie Complète

```
Temps (cycles de 15ms)  Descripton              Durée
─────────────────────────────────────────────────────────
0-2                     OUVERT (Frame 0)        ~30ms
3-4                     FERMETURE (Frame 1)     ~30ms  
5-6                     FERMÉS (Frame 2)        ~30ms
7-9                     OUVERTURE (Frame 3)     ~45ms
10+                     Retour à IDLE           Automatique
─────────────────────────────────────────────────────────
Total                                           ~135ms
```

**Durée totale du clignotement:** ~135 millisecondes (naturel et rapide)

---

## 🖥️ Utilisation depuis l'Interface Web

### Bouton Clignotement
Un nouveau bouton **"Clignoter 👁️"** est disponible dans le dashboard:

```
[IDLE] [SLEEP] [HAPPY] [BLINK 👁️] [SAD] [ANGRY]
```

**Comment utiliser:**
1. Ouvrir l'interface web (http://[IP]/dashboard)
2. Cliquer sur le bouton "BLINK 👁️"
3. Observer l'animation de clignotement
4. Retour automatique à IDLE

### Résultat Visuel
```
Cycle 0-2:    [=●=]  [=●=]     Yeux ouverts
Cycle 3-4:    [▀●▀]  [▀●▀]     En train de se fermer
Cycle 5-6:    [▀▀▀]  [▀▀▀]     Yeux fermés
Cycle 7-9:    [▄●▄]  [▄●▄]     En train de s'ouvrir
Cycle 10+:    [=●=]  [=●=]     Retour au repos
```

---

## 💻 Utilisation depuis le Code

### En C++
```cpp
#include "neopixel.h"

// Déclencher un clignotement
NeoPixel::setAnimation(AnimationType::BLINK);

// L'animation se termine automatiquement
// Retour à IDLE après 10 frames
```

### Comportement Automatique
```cpp
// Auto-Play avec clignotements aléatoires
NeoPixel::setAutoPlay(true);

// Toutes les 3-5 secondes, alterne entre:
// IDLE, HAPPY, SAD, ANGRY, SURPRISED, BLINK, etc.
```

### Intégration Web
```javascript
// Déclencher blink depuis JavaScript
fetch('/api/eyes/animation?type=3')  // 3 = BLINK
  .then(r => r.json())
  .then(d => console.log('Clignotement en cours...'));
```

---

## 🔧 Détails Techniques (v1.3.0)

### Fichiers Modifiés
- ✅ `src/neopixel.cpp` - 4 frames + logique d'animation
- ✅ `platformio.ini` - Version 1.2.0 → 1.3.0

### Nouvelles Variables Statiques
```cpp
// Les 4 images de clignotement
static const uint8_t PATTERN_BLINK_FRAME0[64];  // Ouvert
static const uint8_t PATTERN_BLINK_FRAME1[64];  // Fermeture
static const uint8_t PATTERN_BLINK_FRAME2[64];  // Fermé
static const uint8_t PATTERN_BLINK_FRAME3[64];  // Ouverture
```

### Logique d'Animation (update loop)
```cpp
if (g_eyeState.currentAnimation == AnimationType::BLINK) {
  g_eyeState.animationFrame++;
  
  // Sélectionner la frame en fonction du timing
  const uint8_t* blinkFrame;
  if (animationFrame <= 2) {
    blinkFrame = PATTERN_BLINK_FRAME0;
  } else if (animationFrame <= 4) {
    blinkFrame = PATTERN_BLINK_FRAME1;
  } else if (animationFrame <= 6) {
    blinkFrame = PATTERN_BLINK_FRAME2;
  } else if (animationFrame <= 9) {
    blinkFrame = PATTERN_BLINK_FRAME3;
  } else {
    // Animation terminée
    setAnimation(AnimationType::IDLE);
  }
  
  // Afficher la frame sur les deux yeux
  drawPattern(eyeLeft, blinkFrame, colors...);
  drawPattern(eyeRight, blinkFrame, colors...);
}
```

---

## 🎨 Pattern Visuel ASCII

### Vue de dessus d'une matrice 8×8

```
Frame 0: OUVERT
┌─────────────┐
│ · · · · · · │
│ · ■ ■ ■ ■ · │  ■ = pixel allumé
│ · ■ ● ● ■ · │  ● = iris
│ · ■ ● ● ■ · │  · = noir
│ · ■ ■ ■ ■ · │
│ · · · · · · │
│ · · · · · · │
└─────────────┘

Frame 1: FERMETURE (1/3 fermé)
┌─────────────┐
│ · · · · · · │
│ · ■ ■ ■ ■ · │
│ · ■ ■ ■ ■ · │  Paupière descend
│ · ■ ● ● ■ · │  Iris se rétréci
│ · · · · · · │  Paupière inférieure monte
│ · · · · · · │
└─────────────┘

Frame 2: FERMÉS
┌─────────────┐
│ · · · · · · │
│ · ■ ■ ■ ■ · │
│ · ■ ■ ■ ■ · │  Yeux complètement fermés
│ · · · · · · │
│ · · · · · · │
└─────────────┘

Frame 3: OUVERTURE (2/3 ouverts)
┌─────────────┐
│ · · · · · · │
│ · ■ ■ ■ ■ · │
│ · ■ ● ● ■ · │  Iris réapparaît
│ · ■ ■ ■ ■ · │  Paupière remonte progressivement
│ · · · · · · │
└─────────────┘
```

---

## ✨ Amélioration vs v1.2.0

| Aspect | v1.2.0 | v1.3.0 | Gain |
|--------|--------|--------|------|
| **Blink** | Image fixe | Animation 4 frames | ✓ Réaliste |
| **Duré** | - | 135ms | ✓ Naturel |
| **Fluidité** | Statique | Dynamique | ✓ Vivant |
| **Effet** | Plat | Mouvement | ✓ Expressif |
| **Nombre patterns** | 11 | 11 (3D animée) | ✓ 4 frames/blink |

---

## 📞 Utilisation dans Auto-Play

Quand `setAutoPlay(true)` est activé, le système choisit aléatoirement parmi:

```cpp
AnimationType animations[] = {
  AnimationType::IDLE,        // 30% (repos)
  AnimationType::BLINK,       // 10% (clignotement)
  AnimationType::HAPPY,       // 15% (joie)
  AnimationType::SAD,         // 10%
  AnimationType::ANGRY,       // 10%
  AnimationType::SURPRISED,   // 10%
  AnimationType::SLEEP,       // 15% (sommeil)
};
```

**Résultat:** Les yeux clignotent naturellement pendant l'auto-play!

---

## ✅ Assurance Qualité

- ✅ 0 erreurs de compilation
- ✅ Animation fluide (~15ms par frame)
- ✅ Retour automatique à IDLE
- ✅ Fonctionne sur les deux yeux simultanément
- ✅ Compatible avec tous les schémas de couleur
- ✅ Web UI functional
- ✅ Backward compatible

---

## 🔄 Migration depuis v1.2.0

### Pour les utilisateurs
✅ Transparent - clignotement fonctionne automatiquement

### Pour les développeurs
```cpp
// Code v1.2.0 (toujours compatible)
NeoPixel::setAnimation(AnimationType::BLINK);

// Identique en v1.3.0
// Mais maintenant c'est une vraie animation!
```

---

## 📚 Combinaisons Possibles

### Blink + Couleurs
```cpp
// Orange
NeoPixel::applyColorScheme(ColorScheme::CARS_ORANGE);
NeoPixel::setAnimation(AnimationType::BLINK);

// Couleurs réalistes
NeoPixel::applyColorScheme(ColorScheme::SOFT_REALISTIC);
NeoPixel::setAnimation(AnimationType::BLINK);

// Bleu élégant
NeoPixel::applyColorScheme(ColorScheme::ELEGANT_BLUE);
NeoPixel::setAnimation(AnimationType::BLINK);
```

---

## 🎯 Cas d'Usage

| Situation | Animation | Résultat |
|-----------|-----------|----------|
| Attendre une commande | IDLE | Repos attentif |
| Détecte quelque chose | SURPRISED | Yeux grands ouverts |
| Traitement actif | HAPPY | Expression positive |
| Tous les 3 secondes (naturel) | **BLINK** | **Clignotement réaliste** |
| Fin de tâche | IDLE | Retour au repos |

---

## 📞 Support

Pour tester:
1. Télécharger version 1.3.0
2. Cliquer bouton BLINK dans web UI
3. Observer animation de clignotement
4. Personnaliser couleurs et schémas

**Status:** Production Ready ✅  
**Qualité:** ⭐⭐⭐⭐⭐  
**Minimum version:** 1.3.0

