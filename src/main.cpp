#include <Arduino.h>
#include <FastLED.h>
#include "SHT2x.h"

#define NUM_LEDS 20
#define DATA_PIN 23
#define MICROPHONE_PIN 13
#define SHT_POWER_PIN 19

CRGB leds[NUM_LEDS];
SHT2x sht;

CRGB tempColors[NUM_LEDS] = {
    CRGB(7, 38, 126),
    CRGB(16, 36, 122),
    CRGB(25, 34, 118),
    CRGB(34, 32, 114),
    CRGB(44, 30, 110),
    CRGB(53, 28, 106),
    CRGB(62, 26, 102),
    CRGB(71, 24, 98),
    CRGB(80, 22, 94),
    CRGB(89, 20, 90),
    CRGB(99, 18, 87),
    CRGB(108, 16, 83),
    CRGB(117, 14, 79),
    CRGB(126, 12, 75),
    CRGB(135, 10, 71),
    CRGB(144, 8, 67),
    CRGB(154, 6, 63),
    CRGB(163, 4, 59),
    CRGB(172, 2, 55),
    CRGB(181, 0, 51)};

CRGB loudColors[NUM_LEDS] = {
    CRGB(0, 255, 0),
    CRGB(13, 242, 0),
    CRGB(27, 228, 0),
    CRGB(40, 215, 0),
    CRGB(54, 201, 0),
    CRGB(67, 188, 0),
    CRGB(81, 174, 0),
    CRGB(94, 161, 0),
    CRGB(107, 148, 0),
    CRGB(121, 134, 0),
    CRGB(134, 121, 0),
    CRGB(148, 107, 0),
    CRGB(161, 94, 0),
    CRGB(174, 81, 0),
    CRGB(188, 67, 0),
    CRGB(201, 54, 0),
    CRGB(215, 40, 0),
    CRGB(228, 27, 0),
    CRGB(242, 13, 0),
    CRGB(255, 0, 0)};

CRGB humidityColors[NUM_LEDS] = {
    CRGB(255, 255, 255),
    CRGB(242, 245, 255),
    CRGB(228, 235, 255),
    CRGB(215, 225, 255),
    CRGB(201, 215, 255),
    CRGB(188, 205, 255),
    CRGB(174, 195, 255),
    CRGB(161, 185, 255),
    CRGB(148, 175, 255),
    CRGB(134, 165, 255),
    CRGB(121, 156, 255),
    CRGB(107, 146, 255),
    CRGB(94, 136, 255),
    CRGB(81, 126, 255),
    CRGB(67, 116, 255),
    CRGB(54, 106, 255),
    CRGB(40, 96, 255),
    CRGB(27, 86, 255),
    CRGB(13, 76, 255),
    CRGB(0, 66, 255)};

void setup()
{
  Serial.begin(115200);
  Serial.println("Startup...");

  Serial.println("Startup: fastled...");
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical

  Serial.println("Startup: pin mode...");
  pinMode(MICROPHONE_PIN, INPUT);

  Serial.println("Startup: SHT...");
  pinMode(SHT_POWER_PIN, OUTPUT);
  digitalWrite(SHT_POWER_PIN, HIGH);
  sht.begin();

  delay(100);

  Serial.println("Startup: done.");
}

void activateLEDs(CRGB colors[], float value, float min, float max)
{
  // Anzahl der zu aktivierenden LEDs herausfinden
  int numToActivate = map((long)value * 1000, min * 1000, max * 1000, 0, 20);

  // Stelle sicher, dass die Anzahl der anzusteuernden LEDs nicht ausserhalb von 0 bis NUM_LEDS ist
  numToActivate = constrain(numToActivate, 0, NUM_LEDS);

  // Setze alle LEDs auf Schwarz (ausgeschaltet)
  for (int i = numToActivate; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }

  // Kopiere die RGB-Farben in das LED-Array für die aktivierten LEDs
  for (int i = 0; i < numToActivate; i++)
  {
    leds[i] = colors[i];
  }

  // Aktualisiere die LEDs
  FastLED.show();
}

float volumeSmoothed = 0;

// Anzeigemodus für die Lautstärke
void loudMode()
{
  uint16_t volume = analogRead(MICROPHONE_PIN);
  float factor = volume > volumeSmoothed ? 1 : 0.005;
  volumeSmoothed = volumeSmoothed * (1.0 - factor) + ((float)volume) * factor;

  Serial.print("Lautstaerke: ");
  Serial.println(volumeSmoothed);

  activateLEDs(loudColors, volumeSmoothed, 300, 340);
}

// Anzeigemodus für die Temperatur
void tempMode()
{
  bool sht_status = sht.read();
  float temp = sht.getTemperature();

  Serial.print("Temperatur: ");
  Serial.println(temp);

  activateLEDs(tempColors, temp, 10, 35);
}

// Anzeigemodus für die Luftfeuchtigkeit
void humidityMode()
{
  bool sht_status = sht.read();
  float humid = sht.getHumidity();

  Serial.print("Luftfeuchte: ");
  Serial.println(humid);

  activateLEDs(humidityColors, humid, 20, 100);
}

void loop()
{
  // Erfasse die aktuelle Millisekunde seit dem Start des Programms und berechne den aktuellen Schritt innerhalb eines 30-Sekunden-Zyklus.
  unsigned long currentStepMillis = millis() % 30000;

  // Je nachdem, in welchem Teil des 30-Sekunden-Zyklus wir uns befinden, wird der entsprechende Anzeigemodus aufgerufen.
  if (currentStepMillis < 10000)
    loudMode(); // In den ersten 10 Sekunden des Zyklus wird der "Lautstärke"-Anzeigemodus aufgerufen.
  else if (currentStepMillis < 20000)
    tempMode(); // In den nächsten 10 Sekunden wird der "Temperatur"-Anzeigemodus aufgerufen.
  else
    humidityMode(); // In den letzten 10 Sekunden wird der "Luftfeuchtigkeit"-Anzeigemodus aufgerufen.
}
