#include <Arduino.h>
#include <FastLED.h>
#include "SHT2x.h"

#define NUM_LEDS 20
#define DATA_PIN 23
#define MICROPHONE_PIN 13

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
  pinMode(19, OUTPUT);
  digitalWrite(19, HIGH);

  Serial.println("Startup: fastled...");
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical

  Serial.println("Startup: pin mode...");
  pinMode(MICROPHONE_PIN, INPUT);

  Serial.println("Startup: SHT LIB...");
  Serial.print("SHT2x_LIB_VERSION: \t");
  Serial.println(SHT2x_LIB_VERSION);
  sht.begin();

  Serial.println("Startup: done.");
}

void activateLEDs(CRGB colors[], int numToActivate)
{
  // Stelle sicher, dass die Anzahl der anzusteuernden LEDs nicht größer ist als NUM_LEDS
  if (numToActivate > NUM_LEDS)
  {
    numToActivate = NUM_LEDS;
  }

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

void tempMode()
{
  bool sht_status = sht.read();
  float temp = sht.getTemperature();
  int ledsOn = map((long)temp, 10, 35, 0, 20);
  activateLEDs(tempColors, ledsOn);
}

float volumeSmoothed = 0;

void loudMode()
{
  uint16_t volume = analogRead(MICROPHONE_PIN);
  float factor = volume > volumeSmoothed ? 1 : 0.005;
  volumeSmoothed = volumeSmoothed * (1.0 - factor) + ((float)volume) * factor;
  int ledsOn = map((long)volumeSmoothed, 310, 350, 0, 20);
  activateLEDs(loudColors, ledsOn);
}

void humidityMode()
{
  bool sht_status = sht.read();
  float humid = sht.getHumidity();
  int ledsOn = map((long)humid, 20, 100, 0, 20);
  Serial.println(humid);
  activateLEDs(humidityColors, ledsOn);
}

void loop()
{
  unsigned long startTime = millis(); // Startzeit der Schleife

  // Die Schleife soll für 15 Sekunden laufen (15.000 Millisekunden)
  while (millis() - startTime < 15000)
  {
    // Rufe hier deine Funktion auf, die in der Schleife ausgeführt werden soll
    loudMode();
  }
  // Diese Schleife soll ebenfalls für weitere 15 Sekunden laufen, also bis 30.000ms
  while (millis() - startTime < 30000)
  {
    // Rufe hier deine Funktion auf, die in der Schleife ausgeführt werden soll
    tempMode();
  }
  // Diese Schleife soll ebenfalls für weitere 15 Sekunden laufen, also bis 45.000ms
  while (millis() - startTime < 45000)
  {
    // Rufe hier deine Funktion auf, die in der Schleife ausgeführt werden soll
    humidityMode();
  }
}