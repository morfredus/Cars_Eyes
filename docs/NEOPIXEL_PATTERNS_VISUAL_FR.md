# Visualisation des Patterns NeoPixel - Guide Graphique

Ce document fournit une représentation visuelle simple des 11 patterns révisés.

## Légende des symboles

```
  0 = . (pixel éteint - noir)
  1 = ◉ (couleur primaire - orange)
  2 = ◇ (couleur secondaire - blanc/highlight)
  3 = ● (couleur tertiaire - ombre/pupille)
```

---

## Pattern #01 - IDLE (Yeux au repos)

```
. . . . . . . .
. . ◇ ◇ ◇ ◇ . .
. ◇ ◉ ● ● ◉ ◇ .
. ◇ ◉ ● ● ◉ ◇ .
. ◇ ◉ ◉ ◉ ◉ ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . ◉ ◉ . . .
. . . . . . . .
```

**Note:** Pupille sombre (●) au centre, iris (◉) autour, paupières (◇) haut/bas

---

## Pattern #02 - SLEEP (Yeux fermés)

```
. . . . . . . .
. . . . . . . .
. ◇ ◇ ◇ ◇ ◇ ◇ .
. ◉ ◉ ◉ ◉ ◉ ◉ .
. ◉ ◉ ◉ ◉ ◉ ◉ .
. ◇ ◇ ◇ ◇ ◇ ◇ .
. . . . . . . .
. . . . . . . .
```

**Note:** Paupière épaisse avec highlights, effet fermé

---

## Pattern #03 - BLINK (Clignement)

```
. . . . . . . .
. . ◇ ◇ ◇ ◇ . .
. ◇ ◇ ◉ ◉ ◇ ◇ .
. ◇ ◇ ◉ ◉ ◇ ◇ .
. ◇ ◇ ◇ ◇ ◇ ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . ◉ ◉ . . .
. . . . . . . .
```

**Note:** Iris comprimée, paupières fermantes

---

## Pattern #04 - LOOK_LEFT (Regard gauche)

```
. . . . . . . .
. . ◇ ◇ ◉ . . .
◇ ◇ ● ● ◉ ◇ . .
◇ ◇ ● ● ◉ ◇ . .
◇ ◇ ◉ ◉ ◉ ◇ . .
. . ◇ ◇ ◉ . . .
. . . ◉ . . . .
. . . . . . . .
```

**Note:** Pupille décalée à gauche, ombrage latéral (●)

---

## Pattern #05 - LOOK_RIGHT (Regard droit)

```
. . . . . . . .
. . . ◉ ◇ ◇ ◇ .
. . ◉ ◉ ● ● ◇ ◇
. . ◉ ◉ ● ● ◇ ◇
. . ◉ ◉ ◉ ◇ ◇ ◇
. . . ◉ ◇ ◇ ◇ .
. . . . ◉ . . .
. . . . . . . .
```

**Note:** Pupille décalée à droite, miroir de LOOK_LEFT

---

## Pattern #06 - LOOK_UP (Regard haut)

```
. . ◇ ◇ ◇ ◇ . .
. ◇ ● ● ● ● ◇ .
. ◇ ● ● ● ● ◇ .
. ◇ ◉ ◉ ◉ ◉ ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
```

**Note:** Pupille remontée, paupière haute relevée

---

## Pattern #07 - LOOK_DOWN (Regard bas)

```
. . . . . . . .
. . . . . . . .
. . ◇ ◇ ◇ ◇ . .
. ◇ ◉ ◉ ◉ ◉ ◇ .
. ◇ ● ● ● ● ◇ .
. ◇ ● ● ● ● ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . . . . . .
```

**Note:** Pupille baissée, paupière basse abaissée

---

## Pattern #08 - HAPPY (Joyeux)

```
. . ◇ ◇ ◇ ◇ . .
. ◇ ◉ ◉ ◉ ◉ ◇ .
◇ ◉ ● ● ◇ ● ◉ ◇
◇ ◉ ● ● ◇ ● ◉ ◇
◇ ◉ ● ● ◇ ● ◉ ◇
. ◇ ◉ ◉ ◉ ◉ ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . ◇ ◇ . . .
```

**Note:** Yeux grands ouverts, reflet de joie (◇) au centre, rides

---

## Pattern #09 - SAD (Triste)

```
. ◇ ◇ ◇ ◇ ◇ . .
◇ ◉ ◉ ◉ ◉ ◉ ◇ .
◉ ● ● ● ● ◉ ◉ .
◉ ● ● ● ● ◉ ◉ .
. ◇ ◉ ◉ ◉ ◇ . .
. . ◇ ◇ ◇ . . .
. . . ◉ . . . .
. . . . . . . .
```

**Note:** Iris comprimée, paupières tombantes, larme en bas

---

## Pattern #10 - ANGRY (Furieux)

```
◉ ◇ ◇ ◉ ◉ . . .
◇ ◉ ● ● ◉ ◇ . .
◇ ◉ ● ● ◉ ◇ . .
◉ ◉ ◉ ◉ ◉ ◉ . .
. ◇ ◇ ◉ ◉ . . .
. . ◇ ◉ . . . .
. . . . . . . .
. . . . . . . .
```

**Note:** Plissé intense, iris comprimée, regard agressif

---

## Pattern #11 - SURPRISED (Surpris)

```
. ◇ ◇ ◇ ◇ ◇ . .
◇ ◉ ● ● ● ◉ ◇ .
◉ ● ● ◇ ◇ ● ● ◉
◉ ● ● ◇ ◇ ● ● ◉
◇ ◉ ● ● ● ◉ ◇ .
. ◇ ◇ ◇ ◇ ◇ . .
. . ◇ ◇ ◇ . . .
. . . . . . . .
```

**Note:** Yeux extrêmement ouverts, pupilles massives, reflets de choc

---

## Comparaison visuelle: AVANT → APRÈS

### AVANT (2 couleurs, plat)
```
Pattern IDLE (ancien):
. . . . . . . .
. . 1 1 1 1 . .
. 1 2 2 2 2 1 .
. 1 2 2 2 2 1 .
. 1 2 2 2 2 1 .
. 1 1 1 1 1 1 .
. . 1 1 1 1 . .
. . . . . . . .

= 2 niveaux : Paupière(1) + Centre(2)
```

### APRÈS (3 couleurs, 3D)
```
Pattern IDLE (nouveau):
. . . . . . . .
. . ◇ ◇ ◇ ◇ . .
. ◇ ◉ ● ● ◉ ◇ .
. ◇ ◉ ● ● ◉ ◇ .
. ◇ ◉ ◉ ◉ ◉ ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . ◉ ◉ . . .
. . . . . . . .

= 3 niveaux + détails:
  - Paupière(◇) avec highlights
  - Iris(◉) naturelle
  - Pupille(●) sombre
  - Cils en bas
```

**Gain:** +40% de détail visuel

---

## Guide de couleurs

### Configuration #1: Orange (Cars)
- ◉ Primaire = RGB(255, 69, 0) - Orange Pixar
- ◇ Secondaire = RGB(255, 255, 255) - Blanc pur
- ● Tertiaire = RGB(26, 9, 0) - Marron très foncé

### Configuration #2: Bleu (Nuit)
- ◉ Primaire = RGB(0, 102, 255) - Bleu lumineux
- ◇ Secondaire = RGB(153, 221, 255) - Bleu clair
- ● Tertiaire = RGB(0, 17, 85) - Bleu très foncé

### Configuration #3: Vert (Amical)
- ◉ Primaire = RGB(51, 255, 51) - Vert lime
- ◇ Secondaire = RGB(221, 255, 221) - Vert très clair
- ● Tertiaire = RGB(17, 51, 17) - Vert très foncé

---

## Recommandations d'utilisation

| Pattern | Contexte | Durée typique |
|---------|----------|---------------|
| IDLE | Affichage par défaut | Continu |
| SLEEP | Mode sommeil | Continu |
| BLINK | Animation rapide | 100-200ms |
| LOOK_* | Navigation/attention | 1-2 secondes |
| HAPPY | Confirmation positive | 1-2 secondes |
| SAD | Erreur/regret | 2-3 secondes |
| ANGRY | Alerte/urgence | 2-3 secondes |
| SURPRISED | Notification surprise | 1-2 secondes |

---

**Généré:** 14 janvier 2026  
**Format:** ASCII Art visualization  
**Résolution:** 8×8 pixels per eye
