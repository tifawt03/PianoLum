
#include <WiFi.h>
#include <WebServer.h>
#include <MIDI.h>
#include <FastLED.h>
#include "secrets.h"

// === Structure note ===
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

#define TRANSITION_DELAY    100
#define PREVIEW_BRIGHTNESS   25

// =====================================================
//  MAPPING LED - valide manuellement (v5 09/06/2026)
//  {index_led_debut, nb_leds}
// =====================================================
int noteToLED[NUM_KEYS][2] = {
  {0, 1},  {2, 1},  {4, 1},  {6, 1},  {8, 1},
  {10, 1}, {12, 1}, {14, 1}, {16, 1}, {18, 1},
  {19, 2}, {21, 2}, {23, 2}, {25, 2}, {27, 1},
  {29, 2}, {31, 2}, {33, 2}, {35, 1}, {37, 1},
  {39, 1}, {41, 1}, {43, 1}, {45, 1}, {47, 1},
  {48, 2}, {51, 1}, {52, 2}, {55, 1}, {57, 1},
  {58, 2}, {60, 2}, {62, 2}, {64, 1}, {66, 1},
  {68, 1}, {70, 1}, {71, 2}, {74, 1}, {76, 1},
  {77, 2}, {80, 1}, {81, 2}, {83, 2}, {85, 2},
  {87, 2}, {89, 1}, {91, 1}, {93, 1}, {95, 1},
  {97, 1}, {99, 1}, {101,1}, {103,1}, {105,1},
  {107,1}, {109,1}, {111,1}, {112,2}, {114,1},
  {116,1}
};

int noteToKey(byte note) {
  int k = note - FIRST_NOTE;
  return (k < 0 || k >= NUM_KEYS) ? -1 : k;
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
//  BIBLIOTHEQUE - 7 MORCEAUX
// =====================================================

SongNote twinkle[] = {
  NOTE(60,400), NOTE(60,400), NOTE(67,400), NOTE(67,400),
  NOTE(69,400), NOTE(69,400), NOTE(67,800),
  NOTE(65,400), NOTE(65,400), NOTE(64,400), NOTE(64,400),
  NOTE(62,400), NOTE(62,400), NOTE(60,800)
};

SongNote happyBirthday[] = {
  NOTE(60,300), NOTE(60,100), NOTE(62,400), NOTE(60,400), NOTE(65,400), NOTE(64,800),
  NOTE(60,300), NOTE(60,100), NOTE(62,400), NOTE(60,400), NOTE(67,400), NOTE(65,800),
  NOTE(60,300), NOTE(60,100), NOTE(72,400), NOTE(69,400), NOTE(65,400), NOTE(64,400), NOTE(62,800),
  NOTE(70,300), NOTE(70,100), NOTE(69,400), NOTE(65,400), NOTE(67,400), NOTE(65,800)
};

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

// === Liste des morceaux (ordre = frontend) ===
struct Song {
  const char* id;
  const char* name;
  SongNote*   notes;
  int         length;
};

Song songs[] = {
  {"twinkle","Twinkle Twinkle",     twinkle,      sizeof(twinkle)/sizeof(twinkle[0])},
  {"joyeux", "Joyeux Anniversaire", happyBirthday,sizeof(happyBirthday)/sizeof(happyBirthday[0])},
  {"ode",    "Ode a la Joie",       odeJoie,      sizeof(odeJoie)/sizeof(odeJoie[0])},
  {"bella",  "Bella Ciao",          bellaCiao,    sizeof(bellaCiao)/sizeof(bellaCiao[0])},
  {"jingle", "Jingle Bells",        jingleBells,  sizeof(jingleBells)/sizeof(jingleBells[0])},
  {"harry",  "Harry Potter",        harryPotter,  sizeof(harryPotter)/sizeof(harryPotter[0])},
  {"elise",  "Lettre a Elise",      furElise,     sizeof(furElise)/sizeof(furElise[0])}
};
const int numSongs = sizeof(songs)/sizeof(songs[0]);

int songIdToIndex(const String& id) {
  for (int i = 0; i < numSongs; i++) {
    if (id == songs[i].id) return i;
  }
  return -1;
}

// === Etat du tuteur ===
int  currentSong      = -1;
int  currentNoteIndex =  0;
bool tutorActive      = false;
bool notesPlayed[4]   = {false,false,false,false};
int  notesPlayedCount =  0;
bool previewEnabled   = true;

// =====================================================
//  FONCTIONS LED
// =====================================================

void allumerTouche(int key, CRGB color) {
  if (key < 0 || key >= NUM_KEYS) return;
  int debut = noteToLED[key][0];
  int count = noteToLED[key][1];
  for (int i = 0; i < count; i++)
    if (debut+i < NUM_LEDS) leds[debut+i] = color;
}

void eteindreTouche(int key) {
  if (key < 0 || key >= NUM_KEYS) return;
  int debut = noteToLED[key][0];
  int count = noteToLED[key][1];
  for (int i = 0; i < count; i++)
    if (debut+i < NUM_LEDS) leds[debut+i] = CRGB::Black;
}

void clearLEDs() { FastLED.clear(); FastLED.show(); }

SongNote* getNextSongNote() {
  if (!tutorActive || currentSong < 0) return nullptr;
  int next = currentNoteIndex + 1;
  if (next >= songs[currentSong].length) return nullptr;
  return &songs[currentSong].notes[next];
}

void showNotesWithPreview(SongNote* cur, SongNote* nxt, CRGB color) {
  FastLED.clear();
  if (previewEnabled && nxt != nullptr) {
    for (int i = 0; i < nxt->numNotes; i++) {
      int k = noteToKey(nxt->notes[i]);
      if (k >= 0) allumerTouche(k, CRGB(PREVIEW_BRIGHTNESS,PREVIEW_BRIGHTNESS,PREVIEW_BRIGHTNESS));
    }
  }
  if (cur != nullptr) {
    for (int i = 0; i < cur->numNotes; i++) {
      int k = noteToKey(cur->notes[i]);
      if (k >= 0) allumerTouche(k, notesPlayed[i] ? CRGB::Green : color);
    }
  }
  FastLED.show();
}

void celebration() {
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CHSV((i*8 + j*50) % 256, 255, 200);
    FastLED.show(); delay(80);
  }
  clearLEDs();
}

String noteToName(byte note) {
  const char* n[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
  return String(n[note%12]) + String((note/12)-1);
}

// =====================================================
//  HTML FRONTEND (v10 - JS 100% ASCII / Unicode escapes)
// =====================================================
#include "html_content.h"

// =====================================================
//  HANDLERS HTTP
// =====================================================

void handleRoot() {
  server.sendHeader("Cache-Control","no-store, no-cache, must-revalidate");
  server.sendHeader("Pragma","no-cache");
  server.send_P(200, "text/html; charset=utf-8", index_html);
}

void handlePlay() {
  int idx = -1;
  if (server.hasArg("song")) idx = songIdToIndex(server.arg("song"));
  if (idx < 0 || idx >= numSongs) { server.send(400,"text/plain","song inconnu"); return; }

  currentSong      = idx;
  currentNoteIndex = 0;
  tutorActive      = true;
  notesPlayedCount = 0;
  memset(notesPlayed, false, sizeof(notesPlayed));
  if (server.hasArg("preview")) previewEnabled = server.arg("preview") == "1";

  Serial.println("\n=== " + String(songs[idx].name) + " ===");
  SongNote* first  = &songs[currentSong].notes[0];
  SongNote* second = songs[currentSong].length > 1 ? &songs[currentSong].notes[1] : nullptr;
  showNotesWithPreview(first, second, CRGB::Blue);
  server.send(204,"text/plain","");
}

void handleStop() {
  tutorActive = false; currentSong = -1; clearLEDs();
  Serial.println("=== Arret ===");
  server.send(204,"text/plain","");
}

void handleStatus() {
  if (!tutorActive) { server.send(200,"text/plain",""); return; }
  String s = String(songs[currentSong].name)
           + " note " + String(currentNoteIndex+1)
           + "/" + String(songs[currentSong].length);
  server.send(200,"text/plain",s);
}

void handlePreview() {
  if (server.hasArg("enabled")) {
    previewEnabled = server.arg("enabled") == "1";
    if (tutorActive && currentSong >= 0)
      showNotesWithPreview(&songs[currentSong].notes[currentNoteIndex], getNextSongNote(), CRGB::Blue);
  }
  server.send(204,"text/plain","");
}

// =====================================================
//  SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n========================================");
  Serial.println("   PianoLUM v11 - 7 morceaux");
  Serial.println("========================================\n");

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  clearLEDs();
  Serial.println("LED OK (" + String(NUM_LEDS) + " LEDs, mapping v5)");

  Serial2MIDI.begin(31250, SERIAL_8N1, MIDI_RX_PIN, -1);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  Serial.println("MIDI OK (GPIO " + String(MIDI_RX_PIN) + ")");

  WiFi.softAP(ssid, password);
  Serial.print("WiFi: ");  Serial.println(ssid);
  Serial.print("IP: ");    Serial.println(WiFi.softAPIP());

  server.on("/",            handleRoot);
  server.on("/play",        handlePlay);
  server.on("/stop",        handleStop);
  server.on("/status",      handleStatus);
  server.on("/preview",     handlePreview);
  server.on("/ping",        []() { server.send(204,"text/plain",""); });
  server.on("/favicon.ico", []() { server.send(204,"text/plain",""); });
  server.onNotFound(        []() { server.send(204,"text/plain",""); });
  server.begin();

  // Animation demarrage
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(map(i,0,NUM_LEDS,0,255), 255, 150);
    FastLED.show(); delay(5);
  }
  delay(300); clearLEDs();

  Serial.println("\n>>> WiFi 'PianoLUM' -> http://192.168.4.1\n");
  Serial.println("Morceaux:");
  for (int i = 0; i < numSongs; i++)
    Serial.println("  " + String(i+1) + ". " + String(songs[i].name) + " (" + String(songs[i].length) + " notes)");
}

// =====================================================
//  LOOP
// =====================================================
void loop() {
  server.handleClient();

  if (!MIDI.read()) return;
  byte type     = MIDI.getType();
  byte note     = MIDI.getData1();
  byte velocity = MIDI.getData2();

  if (type == midi::NoteOn && velocity > 0) {
    if (tutorActive && currentSong >= 0) {
      // ---- MODE TUTEUR ----
      SongNote* cur = &songs[currentSong].notes[currentNoteIndex];
      SongNote* nxt = getNextSongNote();
      int matchIndex = -1;
      for (int i = 0; i < cur->numNotes; i++) {
        if (note == cur->notes[i] && !notesPlayed[i]) { matchIndex = i; break; }
      }

      if (matchIndex >= 0) {
        notesPlayed[matchIndex] = true;
        notesPlayedCount++;
        showNotesWithPreview(cur, nxt, CRGB::Blue);

        if (notesPlayedCount >= cur->numNotes) {
          delay(TRANSITION_DELAY);
          currentNoteIndex++;
          notesPlayedCount = 0;
          memset(notesPlayed, false, sizeof(notesPlayed));

          if (currentNoteIndex >= songs[currentSong].length) {
            Serial.println("\n*** BRAVO ! Morceau termine ! ***\n");
            celebration();
            tutorActive = false; currentSong = -1;
            return;
          }

          cur = &songs[currentSong].notes[currentNoteIndex];
          nxt = getNextSongNote();
          Serial.print("Note "); Serial.print(currentNoteIndex+1); Serial.print(": ");
          for (int i = 0; i < cur->numNotes; i++) {
            Serial.print(noteToName(cur->notes[i]));
            if (i < cur->numNotes-1) Serial.print(" + ");
          }
          Serial.println();
          showNotesWithPreview(cur, nxt, CRGB::Blue);
        }

      } else {
        // Mauvaise note - flash rouge
        int key = noteToKey(note);
        if (key >= 0) {
          allumerTouche(key, CRGB::Red); FastLED.show(); delay(150);
          showNotesWithPreview(cur, nxt, CRGB::Blue);
        }
      }

    } else {
      // ---- MODE LIBRE ----
      int key = noteToKey(note);
      if (key >= 0) {
        allumerTouche(key, CHSV(map(note,FIRST_NOTE,FIRST_NOTE+NUM_KEYS,0,255),255,200));
        FastLED.show();
      }
    }
  }

  if (type == midi::NoteOff || (type == midi::NoteOn && velocity == 0)) {
    if (!tutorActive) {
      int key = noteToKey(note);
      if (key >= 0) { eteindreTouche(key); FastLED.show(); }
    }
  }
}
