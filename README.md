# 🎹 PianoLUM

ESP32-based LED system that lights up piano keys to guide you through songs.  
Open-source firmware · Web interface · MIDI song packs

---

## ✨ Features

- 💡 LED strip synchronized with piano keys (blue = left hand, orange = right hand)
- 🎵 Song selector via web interface on the ESP32
- 📁 MIDI-based — import your own songs
- 🖨️ 3D printable enclosure (STL files included)
- 📶 Wi-Fi configuration via web UI

---

## 🚀 Getting Started

**1. Hardware needed**
- ESP32 board
- LED strip (WS2812B)
- Your piano / keyboard

**2. Flash the firmware**
```bash
git clone https://github.com/tifawt03/PianoLUM.git
```
→ Open in Arduino IDE or PlatformIO  
→ Copy `config.example.h` to `config.h` and fill in your Wi-Fi credentials  
→ Flash to your ESP32

**3. Connect & play**
→ Open the web interface → select a song → follow the lights 🎹

---

## 🎵 Song Packs

Free songs are included.  
Additional MIDI packs available on [Gumroad](https://gumroad.com) *(coming soon)*

---

## 🖨️ Enclosure

Free STL files included in `/hardware` — print and clip onto your keyboard.

---

## 📄 License

MIT — free to use, modify, and share.
