# Journal des modifications

## 2.0.3 — 18/01/2026

- Correction : erreur de linkage sur l’état global UI (g_uiState) résolue, restauration du code fonctionnel.
- Toutes les références à l’état UI utilisent désormais UiState::getState() pour la cohérence et la sécurité du linkage.
- Aucun changement fonctionnel, uniquement restauration de l’état stable précédent.
- Passage en version 2.0.3 (SEMVER).

## 2.0.2 — 18/01/2026

- Tous les fichiers sources et headers sont désormais documentés avec Doxygen.
- Ajout de recommandations d'optimisation mémoire dans chaque module (buffers statiques, limitation des allocations dynamiques, gestion RAM).
- Section "Documentation technique" ajoutée dans README.md et README_FR.md.

## 2.0.1 — 17/01/2026

- Documentation : Correction du mapping GPIO NeoPixel Oeil Gauche/Droit pour ESP32-S3 dans PIN_MAPPING_FR.md et PIN_MAPPING.md (Gauche=6, Droit=3) pour correspondre à board_config.h.
- Passage en version 2.0.1 (SEMVER).
## 1.6.2 — 17/01/2026

10) Correction affichage LCD pour l'état OFF
- Ajout de "OFF" au tableau des noms d'animations dans tft_display.cpp.
- Correction de la vérification des limites du tableau de 17 à 18 pour inclure l'état OFF.
- L'écran LCD affiche maintenant correctement "OFF" au lieu de "UNKNOWN" quand les LEDs sont éteintes.

9) Correction complète du bouton OFF - fonction turnOff()
- Ajout de l'état spécial AnimationType::OFF pour empêcher toute animation de redémarrer.
- Nouvelle fonction turnOff() : arrête l'autoplay, efface l'état des clignotants, force l'état OFF et éteint les LEDs.
- Modification de update() pour ignorer toute logique quand l'animation est OFF.
- L'endpoint /api/eyes/off utilise maintenant turnOff() au lieu de setAutoPlay(false) + clear().
- L'état OFF persiste jusqu'à ce que l'utilisateur sélectionne un autre bouton d'animation.

7) Correction des patterns LOOK_LEFT et LOOK_RIGHT
- Correction des codes d'iris dans LOOK_LEFT_FRAME0/1 : passage de 80/81 à 10/11 pour un rendu correct du noyau de la pupille.
- Correction des codes d'iris dans LOOK_RIGHT_FRAME0/1 : passage de 80/81 à 10/11 pour une structure de pupille cohérente.
- Amélioration de la cohérence visuelle entre tous les patterns de regard directionnel.

8) Patterns LOOK_UP et LOOK_DOWN - méthode de rotation de matrice
- LOOK_UP_FRAME0/1 : Généré par rotation de LOOK_LEFT à 90° sens horaire - iris maintenant correctement positionné en haut.
- LOOK_DOWN_FRAME0/1 : Généré par rotation de LOOK_LEFT à 90° sens antihoraire - iris maintenant correctement positionné en bas.
- Les quatre patterns directionnels partagent maintenant une structure cohérente avec positionnement vertical/horizontal de l'iris approprié.

6) Correction persistance bouton OFF
- Le bouton OFF désactive maintenant AutoPlay pour garder les LEDs éteintes.
- Repositionnement du bouton OFF : déplacé après le bouton Pixel Editor (dernière position).
- L'endpoint /api/eyes/off appelle maintenant setAutoPlay(false) avant d'éteindre les LEDs.

## 1.6.1 — 16/01/2026

5) Bouton OFF
- Nouveau bouton OFF dans l'UI Web pour éteindre toutes les LEDs instantanément.
- Nouveau endpoint API : GET /api/eyes/off
- Docs obsolètes supprimés : BLINK_ANIMATION, INDEX_PATTERNS_NEOPIXEL, LCD_SYSTEM (info maintenant dans README/board_config.h).
## 1.6.0 — 16/01/2026

1) Palettes 8 couleurs resserrées
- Cars Orange v2 : moins de blanc, ambres proches et bruns cohérents.
- Human Eye : blancs atténués, bleu acier plus sombre pour les reflets.

2) Atténuation perceptuelle à faible luminosité
- Quand la luminosité UI < 64, les couleurs très claires sont davantage réduites afin de mieux correspondre au niveau choisi.

3) Mise à jour des documents
- Guides visuels (17 animations, 8 couleurs) : docs/NEOPIXEL_PATTERNS_VISUAL_FR.md et docs/NEOPIXEL_PATTERNS_VISUAL.md.
- Schémas de palettes : docs/COLOR_SCHEMES_FR.md et docs/COLOR_SCHEMES.md.
- README_FR/README : note sur l’atténuation perceptuelle.

4) UI Web
- Boutons de sélection pour Cars Orange v2 et Human Eye (8 couleurs).

# Journal des changements

Tous les changements notables de ce projet sont documentés dans ce fichier.
Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/).


## [1.6.0] - 2026-01-16

### Ajouté
- **Moteur de palette 8 couleurs** : Le rendu utilise désormais les emplacements de palette `1..8` avec variantes atténuées (`10..83`) pour un ombrage multi-niveaux sur chaque pixel.
- **Nouvelles palettes** : Ajout des palettes enrichies `CARS_ORANGE_V2` et réaliste `HUMAN_EYE` en 8 couleurs ; les anciens schémas 3 couleurs sont auto-étendus sur 8 emplacements (highlights/ombres) pour garder la cohérence.

### Modifié
- **Recolorisation des animations** : Toutes les animations intégrées (IDLE, BLINK, LOOK_*, SLEEP, HAPPY, SAD, ANGRY, SURPRISED) utilisent maintenant la palette 8 couleurs avec reflets et ombres plus profonds ; les clignotants restent en ambre imposé pour la sécurité.
- **Documentation & exemples** : README et exemple de motif personnalisé mis à jour avec le codage 8 couleurs et l'ensemble des 17 animations.

---

## [1.5.23] - 2026-01-15

### Corrigé
- **Reconnaissance du Mode d'Affichage LCD** : Correction de l'affichage LCD montrant "UNKNOWN" pour les modes TURN_LEFT, TURN_RIGHT, HAZARD et CUSTOM.
  - Extension du tableau des noms d'animations de 13 à 17 éléments dans tft_display.cpp.
  - Ajout du support d'affichage pour : TURN_L (signal tourner à gauche), TURN_R (signal tourner à droite), HAZARD (feux de détresse), et CUSTOM (mode motif personnalisé).
  - Maintenant tous les 17 modes d'animation s'affichent correctement sur l'écran LCD avec les abréviations appropriées.
  - Impact : Les utilisateurs peuvent maintenant voir quel mode d'animation est actif sur l'écran LCD pour tous les modes, y compris les animations de signalisation véhicule.

## [1.5.22] - 2026-01-15

### Corrigé
- **Stabilité de la Mise en Page de la Boîte de Notification** : Correction du problème cosmétique où les messages de notification causaient des changements de hauteur de page.
  - Changement de la boîte de notification de `display:none/block` dynamique à `height:48px` fixe avec transitions `opacity`.
  - La zone de notification réserve maintenant toujours de l'espace dans la mise en page, évitant les changements de disposition quand les messages apparaissent ou disparaissent.
  - Amélioration de la stabilité perçue de l'expérience du tableau de bord de l'interface web.
  - Effet visuel inchangé : les messages s'estompent encore en douceur sur 300ms avec masquage automatique après 5 secondes.

## [1.5.21] - 2026-01-15

### Modifié
- **Amélioration Animation SAD** : Approfondissement de l'expressivité émotionnelle du pattern SAD.
  - Frame 0 : Sourcils maintenant inclinés vers le bas aux coins internes avec ombrage renforcé (codes 33-32) pour mieux transmettre la tristesse.
  - Frame 1 : Sourcils complètement remplis avec effet de tremblement (rangée complète de code 32) simulant des pleurs actifs; larmes suggérées aux coins inférieurs (code 23).
  - Iris maintenant fortement plissée (réduction profondeur codes 10-31) montrant la tension émotionnelle.
  - Comportement de la paupière inférieure amélioré : animation tremblante crée un retour visuel de détresse émotionnelle.
  - Résultat : Expression de tristesse plus nuancée et identifiable qui transmet une profondeur émotionnelle authentique malgré les contraintes 8×8 pixels.

## [1.5.20] - 2026-01-15

### Modifié
- **Amélioration Patterns Multi-Couleurs** : Enrichissement de toutes les 11 animations avec ombrage couleur tertiaire pour plus de réalisme.
  - Ajout de gradations tonales secondaires/tertiaires sur iris, pupilles et paupières.
  - IDLE, SLEEP, BLINK utilisent désormais profondeur 3-tons pour apparence dimensionnelle.
  - LOOK_LEFT/RIGHT/UP/DOWN : Pupilles décalées avec ombrage multi-couches pour meilleure perception de direction.
  - HAPPY : Yeux plus brillants et expressifs avec reflets dorés et ombres externes.
  - SAD : Expression approfondie avec sourcils tertiaires sombres et paupières inférieures tombantes.
  - ANGRY : Regard féroce renforcé avec paupières supérieures sombres et joues ombrées.
  - SURPRISED : Iris 3-tons avec reflets externes pour effet de choc.
  - Résultat : Animations 8×8 pixels plus réalistes et expressives malgré les contraintes de résolution.

## [1.5.19] - 2026-01-15

### Modifié
- **Cohérence du Nom du Projet** : Nom officiel fixé à "Cars Eyes" dans le README et la documentation.
- **Animation SLEEP** : Paupières fermées redessinées avec ombrage et léger mouvement respiratoire pour un rendu plus naturel.

## [1.5.18] - 2026-01-15

### Modifié
- **Animation au Boot - Toujours IDLE** : Modification de la séquence de démarrage pour toujours commencer avec l'animation IDLE indépendamment de l'état sauvegardé.
  - Modification de la fonction `init()` pour forcer `AnimationType::IDLE` au démarrage.
  - Les paramètres (luminosité, couleurs, schéma) sont toujours chargés depuis le stockage persistant.
  - Seul l'état de l'animation est remplacé pour assurer une expérience de démarrage cohérente.
  - Évite que des animations inattendues (comme CUSTOM ou SLEEP) persistent après un redémarrage.

### Corrigé
- **Mises à jour de la Documentation** : Mise à jour des numéros de version dans tous les fichiers de documentation (README, /docs).
  - README.md et README_FR.md mis à jour à la version 1.5.18.
  - Les 20 fichiers de documentation dans /docs mis à jour à la version 1.5.18.
  - Assure la cohérence des versions dans toute la documentation du projet.

## [1.5.17] - 2026-01-15

### Modifié
- **Améliorations UI - Aucun Popup Bloquant** : Suppression de tous les popups JavaScript bloquants (alert) et remplacement par des messages de notification inline.
  - Ajout d'une boîte de notification à hauteur fixe en haut du tableau de bord (évite les changements de longueur de page).
  - Création de la fonction `showNotification(message, type)` avec masquage automatique après 5 secondes.
  - Support de plusieurs types de notification : succès (vert), erreur (rouge), avertissement (orange), info (bleu).
  - Tous les messages d'erreur et d'avertissement s'affichent maintenant inline sans interrompre le flux de travail.
  - Expérience utilisateur améliorée : retours d'information non intrusifs pour toutes les opérations.

## [1.5.16] - 2026-01-15

### Corrigé
- **Stockage Persistant Pattern CUSTOM** : Implémentation du stockage persistant complet pour les patterns custom utilisant Preferences (NVS).
  - Les patterns custom sont maintenant sauvegardés en mémoire flash et survivent aux redémarrages.
  - Ajout de nouvelles fonctions API NeoPixel : `setCustomPatternLeft()`, `setCustomPatternRight()`, `getCustomPatternLeft()`, `getCustomPatternRight()`, `hasCustomPatterns()`.
  - Modification de la fonction `update()` pour afficher correctement les patterns custom lorsque `AnimationType::CUSTOM` est actif.
  - Migration du stockage des patterns de la RAM web_server.cpp vers le stockage persistant neopixel.cpp.
  - Les patterns custom sont automatiquement chargés au démarrage et sauvegardés à chaque modification.
  - Correction du problème où le bouton CUSTOM n'affichait pas les patterns créés dans l'Éditeur de Pixels.

## [1.5.15] - 2026-01-15

### Corrigé
- **Chargement Pattern Éditeur de Pixels** : Correction de l'éditeur pour charger les patterns depuis le stockage serveur au lieu du localStorage non fiable.
  - Modification de `loadPatterns()` pour récupérer depuis `/api/custom/get` (RAM serveur)
  - localStorage utilisé uniquement comme fallback et pour l'état UI temporaire pendant l'édition
  - L'éditeur affiche maintenant les patterns précédemment sauvegardés à l'ouverture
  - Assure la cohérence entre le bouton CUSTOM du Tableau de Bord et l'Éditeur de Pixels
  - Les deux utilisent maintenant le stockage côté serveur comme source unique de vérité

## [1.5.14] - 2026-01-15

### Corrigé
- **Stockage Pattern CUSTOM - Correction Racine** : Identification et résolution du problème fondamental de persistance localStorage sur ESP32.
  - **Cause Racine** : Le localStorage du navigateur n'est PAS persistant entre les rechargements de page sur les appareils embarqués ; les données sont perdues lors de la navigation.
  - **Solution** : Déplacement du stockage des patterns de localStorage côté client vers RAM côté serveur (tableaux globaux).
  - **Nouvelle API** : Ajout du point de terminaison `/api/custom/get` pour récupérer les patterns sauvegardés depuis le serveur.
  - **Flux Mis à Jour** :
    1. Éditeur de Pixels enregistre les patterns → `/api/custom/apply` → stockés en RAM du serveur
    2. Bouton CUSTOM du Tableau de Bord → charge depuis `/api/custom/get` → applique via `/api/custom/apply`
    3. Les patterns persistent maintenant tout au long de la session

## [1.5.13] - 2026-01-15

### Corrigé
- **Chargement Pattern CUSTOM - Correction Finale** : Résolution des problèmes persistants de chargement et d'application des patterns du bouton CUSTOM.
  - Format des données changé de l'hexadécimal au décimal (conforme au stockage interne).
  - L'Éditeur de Pixels et le Tableau de Bord utilisent maintenant le même format : valeurs décimales séparées par des virgules.
  - Validation simplifiée : accepte maintenant les patterns avec au moins un pixel dessiné au lieu d'exiger les deux yeux.
  - Journalisation console complète pour déboguer le flux d'application des patterns.
  - Encodage d'URL implémenté pour une transmission sécurisée des données de pattern.
  - Gestion d'erreur amliorée avec retours détaillés à l'utilisateur via alertes et messages.
  - Compatibilité du point de terminaison API vérifiée avec les deux formats (support hex et décimal via strtol base-0).

## [1.5.12] - 2026-01-15

### Corrigé
- **Disposition Affichage LCD** : Correction du positionnement de la ligne de séparation rouge et de l'information MODE sur l'écran LCD.
  - Ligne de séparation rouge maintenant correctement positionnée après l'information mDNS au lieu de chevaucher.
  - Section MODE placée dynamiquement sous mDNS avec un espacement approprié de 20px.
  - Amélioration de la hiérarchie visuelle et de la lisibilité de l'affichage d'état LCD.

## [1.5.11] - 2026-01-15

### Corrigé
- **Chargement Pattern CUSTOM** : Correction du bouton CUSTOM (animation 16) pour charger et appliquer correctement les motifs depuis localStorage avant d'activer l'animation.
  - Ajout de la fonction JavaScript `applyCustomFromStorage()` pour récupérer les motifs sauvegardés.
  - Le bouton CUSTOM affiche maintenant les motifs créés dans l'Éditeur de Pixels.
  - Affiche une alerte si aucun motif personnalisé n'est trouvé, invitant l'utilisateur à en créer un d'abord.

### Modifié
- **Animation HAPPY** : Amélioration significative de l'expressivité de l'animation HAPPY pour représenter une joie plus intense.
  - Yeux plus larges et grand ouverts.
  - Pupilles plus brillantes avec effet d'éclat amélioré.
  - Courbure de sourire plus prononcée dans les paupières inférieures.
  - Contraste augmenté entre les frames pour une apparence plus dynamique.
- **Affichage LCD** : Ajout du numéro de version sous le titre "CARS EYES" sur l'écran LCD.
  - Version affichée en petit texte jaune (taille 1).
  - Bannière rouge élargie de 30px à 45px de hauteur.
  - Tout le contenu décalé vers le bas à y=75 pour accueillir l'affichage de la version.

## [1.5.10] - 2026-01-15

### Ajouté
- **Éditeur de pixels personnalisé** : Nouvel éditeur de pixels basé sur le web pour créer des motifs personnalisés.
  - Interface de grille 8x8 pour chaque oeil
  - Sélection de couleur par pixel
  - Aperçu en temps réel du motif
  - Fonctions appliquer, effacer et aléatoire
  - Accessible via le point de terminaison `/custom`
- **Animation CUSTOM** : ID d'animation 16 maintenant disponible pour afficher des motifs de pixels personnalisés.
- **Points de terminaison API**:
  - `/api/eyes/custom/pixel` : Définir la couleur de pixel individuelle (oeil, x, y, couleur)
  - `/api/eyes/custom/clear` : Effacer tous les pixels personnalisés
- **Interface Web UI** : Bouton CUSTOM ajouté aux contrôles d'animation et lien "Pixel Editor" sur le tableau de bord.

## [1.5.9] - 2026-01-15

### Corrigé
- **Comportement du mode Hazard** : Correction du bouton warning/hazard pour rester actif en tant qu'animation normale au lieu de s'arrêter après un délai d'expiration.
  - **TURN_LEFT** (13) : Répond au timeout configuré.
  - **TURN_RIGHT** (14) : Répond au timeout configuré.
  - **HAZARD** (15) : Reste maintenant actif indéfiniment jusqu'à la sélection d'une autre animation (comme les animations standard).
- **Logique des signaux Web UI** : Différenciation du comportement des signaux - seuls les clignotants utilisent le timeout, le mode hazard se comporte comme les animations persistantes.

## [1.5.8] - 2026-01-15

### Corrigé
- **Clignotants Web UI** : Correction du dysfonctionnement des boutons clignotants dans l'interface web.
  - **TURN_LEFT** : S'arrête maintenant correctement après la durée configurée (comme le bouton physique).
  - **TURN_RIGHT** : Maintenant réactif et fonctionnel.
  - **HAZARD** : Maintenant réactif et fonctionnel.
- **Validation ID Animation** : Extension de la validation API de 13 à 16 IDs d'animation.
- **Timeout Signal** : Les clignotants via web UI utilisent maintenant correctement `toggleTurnSignal()` avec timeout basé sur la durée au lieu d'un `setAnimation()` permanent.

## [1.5.7] - 2026-01-15

### Ajouté
- **Clignotants Interface Web**: Ajout des boutons manquants pour le contrôle des clignotants au tableau de bord web.
  - Bouton Clignotant Gauche (animation TURN_LEFT)
  - Bouton Clignotant Droit (animation TURN_RIGHT)
  - Bouton Warning/Hazard (animation HAZARD avec les deux flèches)
- **Liste Animations UI**: Mise à jour de la liste des animations du web UI pour inclure les 16 animations complètes incluant les clignotants.

## [1.5.6] - 2026-01-15

### Corrigé
- **Compilation** : Correction d'une erreur de redéclaration de variable dans `tft_display.cpp` qui empêchait la compilation de la version 1.5.5.

## [1.5.5] - 2026-01-15

### Corrigé
- **Écran LCD** : Correction d'un bug empêchant la mise à jour du statut "MODE" sur l'écran lorsque l'animation était changée depuis l'interface Web.

## [1.5.4] - 2026-01-15

### Modifié
- **Design Clignotant** : Mise à jour des motifs de flèches pour utiliser toute la largeur de la matrice (8 pixels), simulant un style de flèche automobile plus réaliste.

## [1.5.3] - 2026-01-15

### Corrigé
- **Rendu** : Correction du moteur de rendu qui n'interprétait pas correctement le code pixel spécial (100) utilisé pour les motifs de clignotants, ce qui entraînait un affichage vide.

## [1.5.2] - 2026-01-15

### Corrigé
- **Logique Boutons** : Réécriture complète de la gestion des boutons pour corriger l'absence de réponse du Bouton 1 et améliorer la détection des appuis.
- **Visuel Clignotants** : L'œil "non clignotant" reste maintenant ouvert (animation Idle) au lieu de s'éteindre complètement.
- **Mode Warning** : Correction de la détection de l'appui simultané (Warning).

## [1.5.1] - 2026-01-15

### Corrigé
- **Logique Bouton Boot**: Correction d'une inversion dans la logique de détection du bouton boot introduite en 1.5.0 qui causait une boucle de redémarrage.

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

### Création du projet Cars Eyes
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

## [2.0.0] - 17/01/2026

### Modifié
- **MAJEUR : Refonte complète du mapping GPIO**
  - Toutes les affectations GPIO pour ESP32-S3 DevKitC-1 N16R8 et ESP32 DevKit V1 sont désormais strictement définies dans `board_config.h`.
  - NeoPixel Œil Gauche : GPIO19, Œil Droit : GPIO20, Statut : GPIO48, LCD BL : GPIO14, Boutons : 0/38/39 (S3).
  - Tout le code et la documentation référencent uniquement ces macros ; plus aucun numéro de pin en dur.
  - Toute la documentation utilisateur (README, GUIDE UTILISATEUR, PIN_MAPPING) est synchronisée sur ce mapping et la version 2.0.0.
  - La version minimale de tous les documents est désormais 2.0.0.
- **Documentation bilingue stricte**
  - Toutes les modifications sont répercutées dans la documentation française et anglaise, strictement équivalentes.

### RUPTURE
- Tout câblage ou code utilisant les anciens numéros de pins doit être mis à jour selon le nouveau mapping de `board_config.h`.
- Cette version n'est pas rétrocompatible avec les anciens mappings.
