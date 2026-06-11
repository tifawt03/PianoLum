# PianoLUM

Un système open source d'apprentissage du piano par LED, conçu avec passion par un maker.

Une barre de 118 LEDs WS2812B se fixe au-dessus des touches d'un piano électrique et les illumine en temps réel : la LED s'allume sur la touche à jouer, passe au vert quand c'est correct, flash rouge si erreur. Pas besoin de lire une partition.

---

## Fonctionnement

1. Brancher l'ESP32 au piano via un câble MIDI DIN 5 broches
2. Connecter son smartphone au WiFi **PianoLUM**
3. Ouvrir `http://192.168.4.1` dans le navigateur
4. Choisir un morceau et suivre les LEDs

---

## Matériel nécessaire

| Composant | Rôle | Prix indicatif |
|-----------|------|---------------|
| ESP32 DevKit V1 (WROOM-32) | Microcontrôleur principal | ~9 € |
| Strip WS2812B 144 LEDs/m (1 m) | Barre lumineuse (118 LEDs utilisées) | ~12 € |
| Shield MIDI avec optocoupler 6N138 | Réception MIDI isolée | ~6 € |
| Câble MIDI DIN 5 broches | Liaison piano → shield | ~1 € |
| Alimentation 5V 10A | Alimentation strip LED | ~10 € |
| Tube silicone diffuseur | Diffusion et protection | ~5 € |

**Total estimé : ~43 €**

---

## Câblage

```
Alimentation 5V 10A
├── 5V  → Strip WS2812B (VCC)
├── 5V  → ESP32 (VIN)
└── GND → GND commun (strip + ESP32 + shield)

ESP32
├── GPIO 13 → Strip WS2812B (DIN)
├── GPIO 16 → Shield MIDI (TX)
└── GND     → GND commun

Shield MIDI
├── DIN 5 broches ← Câble MIDI depuis sortie MIDI OUT du piano
└── TX → ESP32 GPIO 16
```

---

## Installation du firmware

### Prérequis

- [Arduino IDE](https://www.arduino.cc/en/software) avec le support ESP32 (Espressif 2.x)
- Bibliothèques Arduino :
  - `FastLED` v3.x
  - `MIDI Library` v5.x
  - `WebServer` (incluse avec le support ESP32)

### Étapes

1. Cloner le repo :
   ```bash
   git clone https://github.com/tifawt03/PianoLum.git
   cd PianoLum
   ```

2. Copier le fichier de configuration :
   ```bash
   cp secrets.h.example secrets.h
   ```

3. Modifier `secrets.h` avec le nom et mot de passe WiFi souhaités :
   ```cpp
   #define WIFI_SSID     "PianoLUM"
   #define WIFI_PASSWORD "votre_mot_de_passe"
   ```

4. Ouvrir `PianoLum.ino` dans Arduino IDE

5. Sélectionner la carte **ESP32 Dev Module** et le bon port COM

6. Compiler et uploader

7. Le moniteur série (115200 baud) confirme le démarrage :
   ```
   LED OK (118 LEDs, mapping v5)
   MIDI OK (GPIO 16)
   WiFi: PianoLUM
   IP: 192.168.4.1
   ```

---

## Structure du projet

```
PianoLum/
├── PianoLum.ino          # Firmware principal (C++/Arduino)
├── html_content.h        # Interface web embarquée (HTML/CSS/JS)
├── secrets.h             # Credentials WiFi — NE PAS COMMITER (gitignore)
├── secrets.h.example     # Template à copier en secrets.h
└── .gitignore
```

---

## Morceaux intégrés (7)

| # | Morceau | Notes | Difficulté | Tonalité |
|---|---------|-------|------------|----------|
| 1 | Twinkle Twinkle | 14 | ★☆☆ | Do majeur |
| 2 | Joyeux Anniversaire | 25 | ★☆☆ | Do majeur |
| 3 | Ode à la Joie | 30 | ★★☆ | Do majeur |
| 4 | Bella Ciao | 37 | ★★☆ | Mi mineur |
| 5 | Jingle Bells | 49 | ★★☆ | Do majeur |
| 6 | Harry Potter (Hedwig's Theme) | 29 | ★☆☆ | Si mineur |
| 7 | Lettre à Élise | 33 | ★★☆ | La mineur |

---

## Architecture technique

### Mapping LED (v5)

Chaque touche est associée à 1 ou 2 LEDs selon la densité du strip. Le mapping est stocké dans `noteToLED[61][2]` : `{index_led_debut, nombre_leds}`. Calibré manuellement touche par touche.

### Mode tutoring

```
Sélection morceau (/play?song=id)
        ↓
Affichage note courante (bleu) + note suivante (blanc 25%)
        ↓
Note MIDI reçue
   ├── Correcte → LED verte → note suivante
   ├── Mauvaise → flash rouge 150ms → retour affichage
   └── Dernière note → animation arc-en-ciel → fin
```

### Mode libre

Chaque touche jouée s'allume dans une couleur HSV dépendante de la hauteur de la note. Les LEDs s'éteignent au relâchement.

### Serveur web embarqué

| Route | Méthode | Description |
|-------|---------|-------------|
| `/` | GET | Interface HTML complète |
| `/play?song=id` | GET | Démarre le morceau (`id` = twinkle, joyeux, ode, bella, jingle, harry, elise) |
| `/stop` | GET | Arrête le mode tutoring |
| `/status` | GET | Progression en cours |
| `/preview?enabled=0\|1` | GET | Active/désactive la preview |
| `/ping` | GET | Keepalive |

### Interface web

- Design mobile-first : plein écran sur téléphone, mockup téléphone sur desktop
- Filtre par niveau (Débutant / Intermédiaire)
- Sélection visuelle par touche de piano cliquable
- `XMLHttpRequest` avec timeout 4s (compatible iOS Safari sur réseau local)
- `Cache-Control: no-store` pour éviter le cache iOS
- Zéro dépendance externe, 100% embarqué dans le firmware

---

## Boîtier 3D

Le boîtier a été modélisé sur Autodesk Fusion. Il intègre :
- Un logement pour l'ESP32 et le shield MIDI
- Accès aux ports USB et connecteur DIN 5 broches
- Terminal blocks pour la distribution électrique
- Système de fixation au piano

Impression recommandée en PLA, 0.2 mm, 20% de remplissage. Coût matière : ~1-2 €.

---

## Contribuer

Les contributions sont les bienvenues :

- **Nouveaux morceaux** : ajouter un tableau `SongNote[]` dans `PianoLum.ino` et l'entrée correspondante dans `songs[]` et dans le JS de `html_content.h`
- **Améliorations firmware** : fork + pull request
- **Boîtier** : variantes de design acceptées

---

## Limites connues

- Pas de gestion du tempo : le système attend la bonne note sans contrainte de rythme
- Morceaux codés en dur : ajouter un morceau nécessite une recompilation
- MIDI DIN uniquement (pas de MIDI USB ni Bluetooth MIDI)
- Portée WiFi Access Point : quelques mètres (usage domestique)

---

## Licence

Code source sous licence **MIT**. Fichiers STL sous licence **CC BY 4.0** (attribution requise).

---

*Projet réalisé avec passion dans le cadre d'un cursus M2 ingénierie à l'EPSI Montpellier.*
