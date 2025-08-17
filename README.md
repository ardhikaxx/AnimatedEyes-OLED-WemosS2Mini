# Animated Eyes for ESP32 with OLED Display

Animasi mata ekspresif untuk Wemos S2 Mini dengan display OLED 128x64. Project ini menciptakan berbagai ekspresi mata yang dapat digunakan untuk robot atau proyek interaktif lainnya.

## Fitur Utama
- Berbagai animasi mata: berkedip, melihat kiri/kanan, ekspresi senang
- Kontrol mudah melalui urutan animasi
- Kompatibel dengan Wemos S2 Mini ESP32-S2
- Menggunakan display OLED 0.96" I2C 128x64

## Hardware Required
- Wemos S2 Mini (ESP32-S2)
- OLED Display 0.96" I2C 128x64
- Kabel jumper

## Wiring
| Wemos S2 Mini | OLED Display |
|---------------|--------------|
| 3.3V          | VCC          |
| GND           | GND          |
| GPIO8         | SCL          |
| GPIO9         | SDA          |

## Instalasi
1. Clone repository ini
2. Buka di Arduino IDE
3. Install library:
   - [U8g2](https://github.com/olikraus/u8g2)
4. Pilih board: `ESP32S2 Dev Module`
5. Upload ke board

## Penggunaan
- Animasi akan berjalan otomatis dalam urutan:
  1. Berkedip
  2. Berkedip lagi 
  3. Melihat ke kanan
  4. Melihat ke kiri
  5. Ekspresi senang
  6. Berkedip
  7. Berkedip lagi

- Untuk kustomisasi, ubah array `animation_sequence` di kode

## Contoh Animasi
```cpp
const int animation_sequence[] = {4, 5, 2, 3, 6, 4, 5};
// 1: Blink 
// 2: Look Right 
// 3: Look Left  
// 4: Blink 
// 5: Blink 
// 6: Happy
