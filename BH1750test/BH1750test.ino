#include <BH1750.h>
#include <FastLED.h>
#include <Wire.h>

// Macro
#define NUM_LEDS 36   // Number of LED
#define DATA_PIN 3    // LED pin
#define TCAADDR 0x70  // I2C multiplexer

// Definition
BH1750 lightMeter;
CRGB leds[NUM_LEDS];

// Global variable
char buf[200];   //
uint16_t LM[9];  // Setor nilai lux
int treshold = 10;
int i;

void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

void readLight() {
    for (i = 1; i <= 8; i++) {
        tcaselect(i);
        LM[i] = lightMeter.readLightLevel();
        //snprintf(buf, "L[%d] : %d | ", i, LM[i]);
        Serial.print("L[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(LM[i]);
        Serial.print(" | ");
        delay(25);
    }
}

void serialNote() {
    for (i = 1; i <= 8; i++) {
        if (LM[i] > treshold) {
            switch (i) {
                case 1:
                    Serial.print("A");
                    break;
                case 2:
                    Serial.print("B");
                    break;
                case 3:
                    Serial.print("C");
                    break;
                case 4:
                    Serial.print("D");
                    break;
                case 5:
                    Serial.print("E");
                    break;
                case 6:
                    Serial.print("F");
                    break;
                case 7:
                    Serial.print("G");
                    break;
                case 8:
                    Serial.print("H");
                    break;
            }
        }
    }
}

void shiftLED() {
    /* LED breathing */
    for (int i = 0; i <= 36; i++) {
        switch (i % 3) {
            case 0:
                leds[i] = CRGB::Red;
                break;
            case 1:
                leds[i] = CRGB::Green;
                break;
            case 2:
                leds[i] = CRGB::Blue;
                break;
            default:
                leds[i] = CRGB::White;
                break;
        }
    }
    FastLED.show();
}

void setup() {
    Serial.begin(57600);
    Wire.begin();        // I2C begin
    lightMeter.begin();  // BH1750 begin

    if (lightMeter.begin()) {
        Serial.println(F("BH1750 initialized"));
    } else {
        Serial.println(F("Error initializing BH1750"));
    }

    Serial.println(F("BH1750 Test begin"));
    FastLED.addLeds<WS2811, 3, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 4, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 5, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 6, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 7, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 8, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 9, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 10, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, 11, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
}

void loop() {
    readLight();
    shiftLED();

    //Serial.print("Loop");
    Serial.println("");
    delay(25);
}