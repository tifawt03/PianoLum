#include <WiFi.h>
#include <WebServer.h>
#include <MIDI.h>
#include <FastLED.h>
#include "config.h"

// === Structure note (déclarée en tout premier) ===
struct SongNote {
  byte notes[4];
  byte numNotes;
  int duration;
};

#define NOTE(n, d) {{n, 0, 0, 0}, 1, d}

// === Configuration WiFi ===
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// === Configuration LED ===
#define LED_PIN     13
#define NUM_LEDS    118
#define BRIGHTNESS  80

// === Configuration MIDI ===
#define MIDI_RX_PIN 16
#define FIRST_NOTE  36
#define NUM_KEYS    61

// === OPTIMISATION ===
#define TRANSITION_DELAY 100
#define PREVIEW_BRIGHTNESS 25

// === Mapping LED par touche (validé manuellement) ===
int ledsParTouche[NUM_KEYS] = {
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 1, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 1, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

int ledDebut[NUM_KEYS];

void precalculerMapping() {
  int idx = 0;
  for (int i = 0; i < NUM_KEYS; i++) {
    ledDebut[i] = idx;
    idx += ledsParTouche[i];
  }
}

int noteToKey(byte note) {
  int key = note - FIRST_NOTE;
  if (key < 0 || key >= NUM_KEYS) return -1;
  return key;
}

// === Objets globaux ===
WebServer server(80);
CRGB leds[NUM_LEDS];

struct Serial2MIDISettings : public midi::DefaultSettings {
  static const long BaudRate = 31250;
};
HardwareSerial Serial2MIDI(2);
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial2MIDI, MIDI, Serial2MIDISettings);

// =====================================================
//  BIBLIOTHÈQUE MUSICALE - 7 MORCEAUX VALIDÉS
// =====================================================

// 1. Twinkle Twinkle Little Star (14 notes)
// C C G G A A G | F F E E D D C
SongNote twinkle[] = {
  NOTE(60,400), NOTE(60,400), NOTE(67,400), NOTE(67,400),
  NOTE(69,400), NOTE(69,400), NOTE(67,800),
  NOTE(65,400), NOTE(65,400), NOTE(64,400), NOTE(64,400),
  NOTE(62,400), NOTE(62,400), NOTE(60,800)
};

// 2. Joyeux Anniversaire / Happy Birthday (24 notes)
// C C D C F E | C C D C G F | C C C' A F E D | Bb Bb A F G F
SongNote happyBirthday[] = {
  NOTE(60,300), NOTE(60,100), NOTE(62,400), NOTE(60,400), NOTE(65,400), NOTE(64,800),
  NOTE(60,300), NOTE(60,100), NOTE(62,400), NOTE(60,400), NOTE(67,400), NOTE(65,800),
  NOTE(60,300), NOTE(60,100), NOTE(72,400), NOTE(69,400), NOTE(65,400), NOTE(64,400), NOTE(62,800),
  NOTE(70,300), NOTE(70,100), NOTE(69,400), NOTE(65,400), NOTE(67,400), NOTE(65,800)
};

// 3. Ode à la Joie - Beethoven (30 notes)
// E E F G | G F E D | C C D E | E D D
// E E F G | G F E D | C C D E | D C C
SongNote odeJoie[] = {
  NOTE(64,400), NOTE(64,400), NOTE(65,400), NOTE(67,400),
  NOTE(67,400), NOTE(65,400), NOTE(64,400), NOTE(62,400),
  NOTE(60,400), NOTE(60,400), NOTE(62,400), NOTE(64,400),
  NOTE(64,600), NOTE(62,200), NOTE(62,800),
  NOTE(64,400), NOTE(64,400), NOTE(65,400), NOTE(67,400),
  NOTE(67,400), NOTE(65,400), NOTE(64,400), NOTE(62,400),
  NOTE(60,400), NOTE(60,400), NOTE(62,400), NOTE(64,400),
  NOTE(62,600), NOTE(60,200), NOTE(60,800)
};

// 4. Jingle Bells (49 notes)
// E E E | E E E | E G C D E | F F F F | F E E E | E D D E D G |
// E E E | E E E | E G C D E | F F F F | F E E E | G G F D C
SongNote jingleBells[] = {
  NOTE(64,400), NOTE(64,400), NOTE(64,800),
  NOTE(64,400), NOTE(64,400), NOTE(64,800),
  NOTE(64,400), NOTE(67,400), NOTE(60,400), NOTE(62,400), NOTE(64,800),
  NOTE(65,400), NOTE(65,400), NOTE(65,400), NOTE(65,400),
  NOTE(65,400), NOTE(64,400), NOTE(64,400), NOTE(64,400),
  NOTE(64,400), NOTE(62,400), NOTE(62,400), NOTE(64,400), NOTE(62,800), NOTE(67,800),
  NOTE(64,400), NOTE(64,400), NOTE(64,800),
  NOTE(64,400), NOTE(64,400), NOTE(64,800),
  NOTE(64,400), NOTE(67,400), NOTE(60,400), NOTE(62,400), NOTE(64,800),
  NOTE(65,400), NOTE(65,400), NOTE(65,400), NOTE(65,400),
  NOTE(65,400), NOTE(64,400), NOTE(64,400), NOTE(64,400),
  NOTE(67,400), NOTE(67,400), NOTE(65,400), NOTE(62,400), NOTE(60,800)
};

// 5. Lettre à Élise / Für Elise (31 notes)
// E D# E D# E B D C | A | C E A | B | C E G# | B |
// E D# E D# E B D C | A | C E A | B | C B A
SongNote furElise[] = {
  NOTE(64,300), NOTE(63,300), NOTE(64,300), NOTE(63,300),
  NOTE(64,300), NOTE(59,300), NOTE(62,300), NOTE(60,300),
  NOTE(57,600),
  NOTE(60,300), NOTE(64,300), NOTE(69,600),
  NOTE(59,600),
  NOTE(60,300), NOTE(64,300), NOTE(68,600),
  NOTE(59,600),
  NOTE(64,300), NOTE(63,300), NOTE(64,300), NOTE(63,300),
  NOTE(64,300), NOTE(59,300), NOTE(62,300), NOTE(60,300),
  NOTE(57,600),
  NOTE(60,300), NOTE(64,300), NOTE(69,600),
  NOTE(59,600),
  NOTE(60,300), NOTE(59,300), NOTE(57,600)
};

// 6. Bella Ciao (34 notes) - Mi mineur avec F#
// E F# G E B | E F# G E B | E F# G F# E B F# E |
// B B B A B C | A C B A B | G A G F# | B F# G E
SongNote bellaCiao[] = {
  NOTE(64,400), NOTE(66,400), NOTE(67,400), NOTE(64,400), NOTE(71,800),
  NOTE(64,400), NOTE(66,400), NOTE(67,400), NOTE(64,400), NOTE(71,800),
  NOTE(64,400), NOTE(66,400), NOTE(67,400), NOTE(66,400), NOTE(64,400),
  NOTE(71,400), NOTE(66,400), NOTE(64,800),
  NOTE(71,400), NOTE(71,400), NOTE(71,400), NOTE(69,400), NOTE(71,400), NOTE(72,400),
  NOTE(69,400), NOTE(72,400), NOTE(71,400), NOTE(69,400), NOTE(71,400),
  NOTE(67,400), NOTE(69,400), NOTE(67,400), NOTE(66,800),
  NOTE(71,400), NOTE(66,400), NOTE(67,400), NOTE(64,800)
};

// 7. Harry Potter - Hedwig's Theme (31 notes)
// B3 E G F# | E B4 | A | F# | E G F# | D# F B3 |
// B3 E G F# | E B4 | D5 C#5 C5 | G# C5 B A# | G E
SongNote harryPotter[] = {
  NOTE(59,400), NOTE(64,300), NOTE(67,300), NOTE(66,600),
  NOTE(64,400), NOTE(71,800),
  NOTE(69,800),
  NOTE(66,800),
  NOTE(64,300), NOTE(67,300), NOTE(66,600),
  NOTE(63,400), NOTE(65,800), NOTE(59,800),
  NOTE(59,400), NOTE(64,300), NOTE(67,300), NOTE(66,600),
  NOTE(64,400), NOTE(71,800),
  NOTE(74,400), NOTE(73,400), NOTE(72,600),
  NOTE(68,400), NOTE(72,400), NOTE(71,400), NOTE(70,600),
  NOTE(67,400), NOTE(64,800)
};

// === Liste des morceaux ===
struct Song {
  const char* name;
  SongNote* notes;
  int length;
};

Song songs[] = {
  {"Twinkle Twinkle",       twinkle,       sizeof(twinkle)/sizeof(twinkle[0])},
  {"Joyeux Anniversaire",   happyBirthday, sizeof(happyBirthday)/sizeof(happyBirthday[0])},
  {"Ode a la Joie",         odeJoie,       sizeof(odeJoie)/sizeof(odeJoie[0])},
  {"Jingle Bells",          jingleBells,   sizeof(jingleBells)/sizeof(jingleBells[0])},
  {"Lettre a Elise",        furElise,      sizeof(furElise)/sizeof(furElise[0])},
  {"Bella Ciao",            bellaCiao,     sizeof(bellaCiao)/sizeof(bellaCiao[0])},
  {"Harry Potter",          harryPotter,   sizeof(harryPotter)/sizeof(harryPotter[0])}
};
int numSongs = sizeof(songs) / sizeof(songs[0]);

// === Etat du tuteur ===
int currentSong = -1;
int currentNoteIndex = 0;
bool tutorActive = false;
unsigned long correctTime = 0;
bool notesPlayed[4] = {false, false, false, false};
int notesPlayedCount = 0;
bool previewEnabled = true;

// === Fonctions LED avec mapping ===
void allumerTouche(int key, CRGB color) {
  if (key < 0 || key >= NUM_KEYS) return;
  int debut = ledDebut[key];
  for (int i = 0; i < ledsParTouche[key]; i++) {
    if (debut + i < NUM_LEDS) leds[debut + i] = color;
  }
}

void clearLEDs() {
  FastLED.clear();
  FastLED.show();
}

SongNote* getNextSongNote() {
  if (!tutorActive || currentSong < 0) return nullptr;
  int nextIndex = currentNoteIndex + 1;
  if (nextIndex >= songs[currentSong].length) return nullptr;
  return &songs[currentSong].notes[nextIndex];
}

void showNotesWithPreview(SongNote* current, SongNote* next, CRGB color) {
  FastLED.clear();
  if (previewEnabled && next != nullptr) {
    for (int i = 0; i < next->numNotes; i++) {
      int key = noteToKey(next->notes[i]);
      if (key >= 0) allumerTouche(key, CRGB(PREVIEW_BRIGHTNESS, PREVIEW_BRIGHTNESS, PREVIEW_BRIGHTNESS));
    }
  }
  if (current != nullptr) {
    for (int i = 0; i < current->numNotes; i++) {
      int key = noteToKey(current->notes[i]);
      if (key >= 0) {
        allumerTouche(key, notesPlayed[i] ? CRGB::Green : color);
      }
    }
  }
  FastLED.show();
}

void celebration() {
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV((i * 8 + j * 50) % 256, 255, 200);
    }
    FastLED.show();
    delay(80);
  }
  clearLEDs();
}

String noteToName(byte note) {
  const char* names[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
  return String(names[note % 12]) + String((note / 12) - 1);
}

// === Page Web ===
String getHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Piano LED</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
      min-height: 100vh; color: white; padding: 20px;
    }
    .container { max-width: 400px; margin: 0 auto; }
    h1 {
      text-align: center; font-size: 1.8em; margin-bottom: 10px;
      background: linear-gradient(90deg, #00d4ff, #9b59b6);
      -webkit-background-clip: text; -webkit-text-fill-color: transparent;
    }
    .subtitle { text-align: center; color: #888; margin-bottom: 30px; }
    .song-list {
      background: rgba(255,255,255,0.05); border-radius: 15px;
      padding: 10px; margin-bottom: 20px;
    }
    .song-item {
      display: flex; align-items: center; padding: 15px; margin: 5px 0;
      background: rgba(255,255,255,0.03); border-radius: 10px;
      cursor: pointer; transition: all 0.2s;
    }
    .song-item:hover { background: rgba(255,255,255,0.1); }
    .song-item.selected {
      background: linear-gradient(90deg, rgba(0,212,255,0.3), rgba(155,89,182,0.3));
      border: 1px solid rgba(0,212,255,0.5);
    }
    .song-item .emoji { font-size: 1.5em; margin-right: 12px; }
    .song-item .info { flex: 1; }
    .song-item .title { font-weight: 600; font-size: 1em; }
    .song-item .detail { color: #888; font-size: 0.8em; margin-top: 2px; }
    .status-bar {
      background: rgba(0,212,255,0.1); border: 1px solid rgba(0,212,255,0.3);
      border-radius: 10px; padding: 15px; margin-bottom: 20px;
      text-align: center; min-height: 50px;
    }
    .status-text { font-size: 0.95em; color: #00d4ff; }
    .controls { display: flex; gap: 10px; margin-bottom: 15px; }
    .btn {
      flex: 1; padding: 14px; border: none; border-radius: 10px;
      font-size: 1em; font-weight: 600; cursor: pointer; transition: all 0.2s;
    }
    .btn-stop { background: rgba(231,76,60,0.3); color: #e74c3c; border: 1px solid rgba(231,76,60,0.5); }
    .btn-stop:hover { background: rgba(231,76,60,0.5); }
    .toggle-row {
      display: flex; justify-content: space-between; align-items: center;
      background: rgba(255,255,255,0.05); border-radius: 10px;
      padding: 12px 15px; margin-bottom: 20px;
    }
    .toggle-label { font-size: 0.9em; color: #ccc; }
    .toggle {
      width: 50px; height: 26px; border-radius: 13px; position: relative;
      background: #333; cursor: pointer; transition: background 0.3s;
    }
    .toggle.on { background: #00d4ff; }
    .toggle .knob {
      width: 22px; height: 22px; border-radius: 50%; background: white;
      position: absolute; top: 2px; left: 2px; transition: left 0.3s;
    }
    .toggle.on .knob { left: 26px; }
  </style>
</head>
<body>
  <div class="container">
    <h1>🎹 Piano LED</h1>
    <p class="subtitle">7 morceaux - Appuie sur les notes</p>

    <div class="toggle-row">
      <span class="toggle-label">Preview prochaine note</span>
      <div class="toggle on" id="previewToggle" onclick="togglePreview()">
        <div class="knob"></div>
      </div>
    </div>

    <div class="song-list" id="songList"></div>

    <div class="status-bar">
      <div class="status-text" id="status">Choisis un morceau</div>
    </div>

    <div class="controls">
      <button class="btn btn-stop" onclick="stopSong()">Arreter</button>
    </div>
  </div>

  <script>
    var songs = [
)rawliteral";

  for (int i = 0; i < numSongs; i++) {
    html += "      {name:\"" + String(songs[i].name) + "\", notes:" + String(songs[i].length) + "}";
    if (i < numSongs - 1) html += ",";
    html += "\n";
  }

  html += R"rawliteral(
    ];
    var emojis = ["⭐","🎂","🎵","🔔","🎼","✊","⚡"];
    var selected = -1;
    var previewOn = true;

    function buildList() {
      var list = document.getElementById('songList');
      list.innerHTML = '';
      for (var i = 0; i < songs.length; i++) {
        var div = document.createElement('div');
        div.className = 'song-item' + (i === selected ? ' selected' : '');
        div.innerHTML = '<div class="emoji">' + emojis[i] + '</div>' +
          '<div class="info"><div class="title">' + songs[i].name + '</div>' +
          '<div class="detail">' + songs[i].notes + ' notes</div></div>';
        div.onclick = (function(idx) { return function() { playSong(idx); }; })(i);
        list.appendChild(div);
      }
    }

    function playSong(idx) {
      selected = idx;
      buildList();
      fetch('/play?song=' + idx).then(r => r.text()).then(t => {
        document.getElementById('status').textContent = t;
      });
      updateStatus();
    }

    function stopSong() {
      selected = -1;
      buildList();
      fetch('/stop').then(r => r.text()).then(t => {
        document.getElementById('status').textContent = t;
      });
    }

    function togglePreview() {
      previewOn = !previewOn;
      var tog = document.getElementById('previewToggle');
      tog.className = 'toggle' + (previewOn ? ' on' : '');
      fetch('/preview?enabled=' + (previewOn ? '1' : '0'));
    }

    function updateStatus() {
      fetch('/status').then(r => r.text()).then(t => {
        if (t) document.getElementById('status').textContent = t;
      });
      if (selected >= 0) setTimeout(updateStatus, 500);
    }

    buildList();
  </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() { server.send(200, "text/html", getHTML()); }

void handlePlay() {
  if (server.hasArg("song")) {
    int idx = server.arg("song").toInt();
    if (idx >= 0 && idx < numSongs) {
      currentSong = idx;
      currentNoteIndex = 0;
      tutorActive = true;
      notesPlayedCount = 0;
      memset(notesPlayed, false, sizeof(notesPlayed));

      Serial.println("\n=== " + String(songs[idx].name) + " ===");

      SongNote* first = &songs[currentSong].notes[0];
      SongNote* second = songs[currentSong].length > 1 ? &songs[currentSong].notes[1] : nullptr;
      showNotesWithPreview(first, second, CRGB::Blue);
      server.send(200, "text/plain", "En cours: " + String(songs[currentSong].name));
      return;
    }
  }
  server.send(400, "text/plain", "Erreur");
}

void handleStop() {
  tutorActive = false;
  currentSong = -1;
  clearLEDs();
  Serial.println("\n=== Arret ===");
  server.send(200, "text/plain", "Choisis un morceau");
}

void handleStatus() {
  if (!tutorActive) { server.send(200, "text/plain", ""); return; }
  String status = String(songs[currentSong].name) + " - Note " +
                  String(currentNoteIndex + 1) + "/" + String(songs[currentSong].length);
  server.send(200, "text/plain", status);
}

void handlePreview() {
  if (server.hasArg("enabled")) {
    previewEnabled = server.arg("enabled") == "1";
    if (tutorActive && currentSong >= 0) {
      showNotesWithPreview(&songs[currentSong].notes[currentNoteIndex], getNextSongNote(), CRGB::Blue);
    }
  }
  server.send(200, "text/plain", "OK");
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  delay(500);
  precalculerMapping();

  Serial.println("\n========================================");
  Serial.println("   PIANO LED v4.0 - 7 morceaux");
  Serial.println("========================================\n");

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  clearLEDs();
  Serial.println("LED OK (118 LEDs, mapping manuel)");

  Serial2MIDI.begin(31250, SERIAL_8N1, MIDI_RX_PIN, -1);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  Serial.println("MIDI OK (GPIO 16)");

  WiFi.softAP(ssid, password);
  Serial.print("WiFi: "); Serial.println(ssid);
  Serial.print("IP: "); Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/play", handlePlay);
  server.on("/stop", handleStop);
  server.on("/status", handleStatus);
  server.on("/preview", handlePreview);
  server.begin();

  // Animation démarrage
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(map(i, 0, NUM_LEDS, 0, 255), 255, 150);
    FastLED.show();
    delay(5);
  }
  delay(300);
  clearLEDs();

  Serial.println("\n>>> Connecte-toi au WiFi 'PianoLED' puis va sur http://192.168.4.1\n");
  Serial.println("Morceaux disponibles:");
  for (int i = 0; i < numSongs; i++) {
    Serial.println("  " + String(i+1) + ". " + String(songs[i].name) + " (" + String(songs[i].length) + " notes)");
  }
}

// === Loop ===
void loop() {
  server.handleClient();

  if (MIDI.read()) {
    byte type = MIDI.getType();
    byte note = MIDI.getData1();
    byte velocity = MIDI.getData2();

    if (type == midi::NoteOn && velocity > 0) {
      if (tutorActive && currentSong >= 0) {
        // MODE TUTEUR
        SongNote* current = &songs[currentSong].notes[currentNoteIndex];
        SongNote* next = getNextSongNote();
        int matchIndex = -1;
        for (int i = 0; i < current->numNotes; i++) {
          if (note == current->notes[i] && !notesPlayed[i]) {
            matchIndex = i;
            break;
          }
        }

        if (matchIndex >= 0) {
          notesPlayed[matchIndex] = true;
          notesPlayedCount++;
          showNotesWithPreview(current, next, CRGB::Blue);

          if (notesPlayedCount >= current->numNotes) {
            delay(TRANSITION_DELAY);
            currentNoteIndex++;
            notesPlayedCount = 0;
            memset(notesPlayed, false, sizeof(notesPlayed));

            if (currentNoteIndex >= songs[currentSong].length) {
              Serial.println("\n*** BRAVO ! Morceau termine ! ***\n");
              celebration();
              tutorActive = false;
              currentSong = -1;
              return;
            }

            current = &songs[currentSong].notes[currentNoteIndex];
            next = getNextSongNote();

            Serial.print("Note ");
            Serial.print(currentNoteIndex + 1);
            Serial.print(": ");
            for (int i = 0; i < current->numNotes; i++) {
              Serial.print(noteToName(current->notes[i]));
              if (i < current->numNotes - 1) Serial.print(" + ");
            }
            Serial.println();

            showNotesWithPreview(current, next, CRGB::Blue);
          }
        } else {
          // Mauvaise note - flash rouge
          int key = noteToKey(note);
          if (key >= 0) {
            allumerTouche(key, CRGB::Red);
            FastLED.show();
            delay(150);
            showNotesWithPreview(current, next, CRGB::Blue);
          }
        }
      } else {
        // MODE LIBRE - afficher la note jouée
        int key = noteToKey(note);
        if (key >= 0) {
          allumerTouche(key, CHSV(map(note, FIRST_NOTE, FIRST_NOTE + NUM_KEYS, 0, 255), 255, 200));
          FastLED.show();
        }
      }
    }

    if (type == midi::NoteOff || (type == midi::NoteOn && velocity == 0)) {
      if (!tutorActive) {
        int key = noteToKey(note);
        if (key >= 0) {
          int debut = ledDebut[key];
          for (int i = 0; i < ledsParTouche[key]; i++) {
            if (debut + i < NUM_LEDS) leds[debut + i] = CRGB::Black;
          }
          FastLED.show();
        }
      }
    }
  }
}
