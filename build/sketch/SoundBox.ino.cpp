#include <Arduino.h>
#line 1 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
/****** SOUNDBOX v1.0 ******/
/* To do:   1. Test WS2811 Color (RGB/GRB)
            2. Autocalibration (with button input)
            3. LED color breathing / shift
            4. Add continous trigger checking
            5. Randomize note location (sensor)
*/

#include <BH1750.h>
#include <FastLED.h>
#include <Wire.h>

/* Macro */
#define NUM_LEDS_PER_STRIP 36
#define DATA_PIN 3
#define NUM_STRIP 8
#define TCAADDR 0x70

/* Object Declaration */
BH1750 lightMeter;
CRGB leds[NUM_LEDS_PER_STRIP];

/* Global Variable */
uint16_t LM[9];         // Lux value [1-8]
int treshold = 10;      // Lux treshold to trigger note sound
int i;

/* I2C Multiplexer Selector */
#line 29 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void tcaselect(uint8_t i);
#line 37 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void readLight();
#line 53 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void serialNote();
#line 88 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void shiftLED();
#line 110 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void setup();
#line 140 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void loop();
#line 29 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

/* Reading lux value of BH1750 */
void readLight() {
    for (i = 1; i <= 8; i++) {
        tcaselect(i);
        LM[i] = lightMeter.readLightLevel();
        /* Debugging :
        Serial.print("L[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(LM[i]);
        Serial.print(" | ");
        delay(25);
        */
    }
}

/* Lux Tresholding for Serial Print */
void serialNote() {
    for (i = 1; i <= 8; i++) {
        if (LM[i] > treshold) {
            switch (i) {
                case 1:
                    Serial.println("C");
                    break;
                case 2:
                    Serial.println("D");
                    break;
                case 3:
                    Serial.println("E");
                    break;
                case 4:
                    Serial.println("F");
                    break;
                case 5:
                    Serial.println("G");
                    break;
                case 6:
                    Serial.println("A");
                    break;
                case 7:
                    Serial.println("B");
                    break;
                case 8:
                    Serial.println("H");
                    break;
            }
        }
    }
    Serial.println("");
}

/* LED Color (needs rework) */
void shiftLED() {
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

/* Auto-Calibrate bottom and upper  */

void setup(){
    /* Initialization */
    Serial.begin(57600);
    Wire.begin();        // I2C begin
    lightMeter.begin();  // BH1750 begin
    
    Serial.println("Starting SOUNDBOX v1.0");

    /* WS2811 Setup */
    Serial.print("Initializing WS2811 LED");
    FastLED.addLeds<WS2811, 3>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 4>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 5>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 6>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 7>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 8>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 9>(leds, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 10>(leds, NUM_LEDS_PER_STRIP);
    FastLED.setBrightness(255);
    Serial.println("Ready!");
    
    /* BH1750 Setup */
    Serial.print("Initializing BH1750");
    while (!lightMeter.begin()) {
        Serial.print(".");
    } else {
        Serial.println("Ready!");
    }
}

void loop(){
    readLight();
    shiftLED();
    
    delay(10);
}
#line 1 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox copy.ino"
/****** SOUNDBOX v1.0 ******/
/* To do:   1. Test WS2811 Color (RGB/GRB)
            2. Autocalibration (with button input)
            3. LED color breathing / shift
            4. Add continous trigger checking
            5. Randomize note location (sensor)
*/

#include <BH1750.h>
#include <FastLED.h>
#include <Wire.h>

/* Macro */
#define NUM_LEDS_PER_STRIP 36
#define DATA_PIN 3
#define NUM_STRIP 8
#define TCAADDR 0x70

/* Object Declaration */
BH1750 lightMeter;
CRGB leds[NUM_LEDS_PER_STRIP];

/* Global Variable */
uint16_t LM[9];         // Lux value [1-8]
int treshold = 10;      // Lux treshold to trigger note sound
int i;

/* I2C Multiplexer Selector */
void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

/* Reading lux value of BH1750 */
void readLight() {
    for (i = 1; i <= 8; i++) {
        tcaselect(i);
        LM[i] = lightMeter.readLightLevel();
        /* Debugging :
        Serial.print("L[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(LM[i]);
        Serial.print(" | ");
        delay(25);
        */
    }
}

/* Lux Tresholding for Serial Print */
void serialNote() {
    for (i = 1; i <= 8; i++) {
        if (LM[i] > treshold) {
            switch (i) {
                case 1:
                    Serial.println("C");
                    break;
                case 2:
                    Serial.println("D");
                    break;
                case 3:
                    Serial.println("E");
                    break;
                case 4:
                    Serial.println("F");
                    break;
                case 5:
                    Serial.println("G");
                    break;
                case 6:
                    Serial.println("A");
                    break;
                case 7:
                    Serial.println("B");
                    break;
                case 8:
                    Serial.println("H");
                    break;
            }
        }
    }
    Serial.println("");
}

/* LED Color (needs rework) */
void shiftLED() {
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

void setup(){
    /* Initialization */
    Serial.begin(57600);
    Wire.begin();        // I2C begin
    lightMeter.begin();  // BH1750 begin
    
    Serial.println("Starting SOUNDBOX v1.0");

    /* WS2811 Setup */
    Serial.print("Initializing WS2811 LED");
    for(i = DATA_PIN; i <= (DATA_PIN + NUM_STRIP); i++){
        FastLED.addLeds<WS2811, i>(leds, NUM_LEDS_PER_STRIP);
        Serial.print(".");
    }
    FastLED.setBrightness(255);
    Serial.println("Ready!");
    
    /* BH1750 Setup */
    Serial.print("Initializing BH1750");
    while (!lightMeter.begin()) {
        Serial.print(".");
    } else {
        Serial.println("Ready!");
    }
}

void loop(){
    readLight();
    shiftLED();
    
    delay(10);
}
