# ESP32 Sensor-Anzeige

Dieses Arduino-Programm verwendet einen ESP32-Mikrocontroller und LEDs, um Umgebungstemperatur, Luftfeuchtigkeit und Lautstärke anzuzeigen. Es ist eine einfache Anwendung, die die grundlegenden Funktionen des Mikrocontrollers und einiger Sensoren demonstriert.

## Hardware-Anforderungen

- ESP32-Mikrocontroller (getestet auf ESP32 DevKitC)
- WS2812B-kompatible RGB-LEDs (Neopixel)
- SHT2x Temperatur- und Luftfeuchtigkeitssensor
- Elektret-Mikrofon

## Bibliotheken

Um dieses Programm auszuführen, müssen Sie folgende Arduino-Bibliotheken installieren:

- FastLED: [https://github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)
- SHT2x: [https://github.com/practicalarduino/SHT2x](https://github.com/practicalarduino/SHT2x)

## Anzeigemodi

Das Programm verfügt über drei Anzeigemodi:

1. **Temperaturmodus:** In diesem Modus werden die LEDs entsprechend der gemessenen Temperatur angezeigt. Je wärmer es ist, desto mehr LEDs werden eingeschaltet.

2. **Lautstärkemodus:** Hier zeigt das Programm die Lautstärke der Umgebung anhand des Mikrofons an. Je lauter die Umgebung, desto mehr LEDs leuchten.

3. **Luftfeuchtigkeitsmodus:** In diesem Modus werden die LEDs entsprechend der gemessenen Luftfeuchtigkeit angezeigt. Höhere Luftfeuchtigkeit führt zu mehr eingeschalteten LEDs.

## Anwendung

1. Schließen Sie die Hardware entsprechend den angegebenen Anschlüssen an Ihren ESP32 an.

2. Installieren Sie die erforderlichen Arduino-Bibliotheken (FastLED und SHT2x).

3. Laden Sie das Programm auf Ihren ESP32 hoch.

4. Über die serielle Schnittstelle (115200 Baud) können Sie die Ausgabe des Programms verfolgen.

5. Die LEDs zeigen automatisch die gemessenen Werte an, je nachdem, welcher Anzeigemodus aktiv ist.

## Lizenz

tbd.

---

Viel Spaß beim Experimentieren und Lernen! 

