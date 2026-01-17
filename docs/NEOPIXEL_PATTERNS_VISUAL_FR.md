# Visualisation des patterns NeoPixel - Guide Graphique

**Version :** 1.6.0  
**Version minimale :** 1.6.0  
**Langue :** Français  
**Date :** 16 janvier 2026  

Référence visuelle des 17 animations actives avec le moteur à 8 couleurs.

## Légende (codes palette)
```
0   = .   (éteint)
C1  = palette case 1 (iris cœur / forcé ambre pour signaux)
C2  = case 2
C3  = case 3
C4  = case 4 (highlight)
C5  = case 5 (highlight froid)
C6  = case 6 (ombre/contraste)
C7  = case 7 (contour/profondeur ambre)
C8  = case 8 (noir/pupille)
Suffixes : h=70%, m=40%, l=20% (ex: C7h = case 7 à 70%)
100 (flèches) = C1 imposé (ambre forcé)
```

---

## #01 - REPOS (Idle)
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C5 C1h C1 C1 C1h C5 C5h
C5 C1h C8 C8h C8h C8 C1h C5
C5 C1h C8h C8m C8m C8h C1h C5
C5h C5 C1h C1 C1 C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #02 - SOMMEIL (Fermé)
```
0 C7h C7h C7h C7h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C7h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C7h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #03 - CLIGNEMENT
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C5 C1h C1 C1 C1h C5 C5h
C5 C1h C8 C8h C8h C8 C1h C5
C5 C1h C8h C8m C8m C8h C1h C5
C5h C5 C1h C1 C1 C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```
(Frame 1/2 se comprime davantage en fente.)

## #04 - REGARD_GAUCHE
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C5h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #05 - REGARD_DROITE
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C7h C5h C4h C1h C5 C5h
0 C7h C7h C5 C8h C8 C1 C5
0 C7h C7h C5 C8 C8h C8 C5
0 C7h C7h C5h C4h C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #06 - REGARD_HAUT
```
0 C7h C7h C7h C7h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C7h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C7h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #07 - REGARD_BAS
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C5h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C7h C7h C7h C7h C7h 0
```

## #08 - JOIE
```
0 C7h C5h C5 C5 C5h C7h 0
0 C5h C5 C4h C4h C5 C5h 0
C5h C4h C1h C1 C1 C1h C4h C5h
C4h C1h C8 C8h C8h C8 C1h C4h
C4h C1 C8 C8h C8h C8 C1 C4h
C5h C4h C1h C1 C1 C1h C4h C5h
C7h C5 C4h C1h C1h C4h C5 C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #09 - TRISTE
```
0 C7h 0 0 0 0 C7h 0
C7h C7h C7 0 0 C7 C7h C7h
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C4h C1 C1 C4h C5h C7h
C5 C4h C1 C8 C8 C1 C4h C5
C4h C1 C8 C8h C8h C8 C1 C4h
C7h C5h C5h C4h C4h C5h C5h C7h
0 0 C7h C5h C5h C7h 0 0
```

## #10 - COLERE
```
0 0 0 0 0 0 0 0
0 0 C7h C5h C5h C7h 0 0
0 0 C5 C4h C4h C5 C7h 0
C5h C5 C8 C8h C8h C8 C5 C5h
C5 C4h C8 C8m C8m C8 C4h C5
C5h C4h C1 C8 C8 C1 C4h C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #11 - SURPRIS
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C5 C1h C1 C1 C1h C5 C5h
C5 C4h C1 C1h C1h C1 C4h C5
C5 C4h C1 C1h C1h C1 C4h C5
C5h C5 C1h C1 C1 C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #12 - CLIN_GAUCHE
Oeil gauche : CLIGNEMENT ; Oeil droit : REPOS.

## #13 - CLIN_DROIT
Oeil gauche : REPOS ; Oeil droit : CLIGNEMENT.

## #14 - CLIGNOTANT_GAUCHE (Flèche ambre)
```
0 0 A 0 0 0 0 0
0 A A 0 0 0 0 0
0 A A A A A A A
A A A A A A A A
A A A A A A A A
0 A A A A A A A
0 A A 0 0 0 0 0
0 0 A 0 0 0 0 0
```
(A = ambre forcé via code 100 → C1)

## #15 - CLIGNOTANT_DROIT (Flèche ambre)
```
0 0 0 0 0 A 0 0
0 0 0 0 0 A A 0
A A A A A A A 0
A A A A A A A A
A A A A A A A A
A A A A A A A 0
0 0 0 0 0 A A 0
0 0 0 0 0 A 0 0
```

## #16 - WARNING
Oeil gauche : flèche CLIGNOTANT_GAUCHE ; oeil droit : CLIGNOTANT_DROIT (ambre fixe jusqu'à une nouvelle animation).

## #17 - PERSONNALISE
Pattern 8×8 par œil défini par l'utilisateur. Codes selon la légende ci-dessus ; diminuer via suffixes `h/m/l` (70/40/20%).





