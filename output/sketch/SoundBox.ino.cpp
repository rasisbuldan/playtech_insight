#include <Arduino.h>
#line 1 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
/****** SOUNDBOX v1.0 ******/
/* To do:   1. Test WS2811 Color (RGB/GRB) => GRB
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
CRGB leds[NUM_STRIP][NUM_LEDS_PER_STRIP];

/* Global Variable */
uint16_t LM[8];  // Lux value [1-8]
boolean LMx[8];
char note[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'H'};
char noteRandom[8];
int treshold = 10;  // Lux treshold to trigger note sound
int i, j;

/* I2C Multiplexer Selector */
#line 32 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void tcaselect(uint8_t i);
#line 40 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void readLight();
#line 53 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void serialNote(boolean random);
#line 130 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void randomizeNote();
#line 150 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void shiftLED();
#line 166 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void setup();
#line 203 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void loop();
#line 32 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

/* Reading lux value of BH1750 */
void readLight() {
    for (i = 0; i < 8; i++) {
        Serial.print("L[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(LM[i]);
        Serial.print(".");
        Serial.print(LMx[i]);
        Serial.print(" | ");
    }
    Serial.println("");
}

void serialNote(boolean random) {
    for (i = 0; i < 8; i++) {
        //Serial.println("Serial note!");
        /* Multiplex */
        tcaselect(i);
        LM[i] = lightMeter.readLightLevel();

        /* Sensor treshold checking */
        if (LM[i] <= treshold) {
            if (!LMx[i]) {
                LMx[i] = !LMx[i];

                /* Print note */
                if (random) {
                    switch (i) {
                        case 0:
                            Serial.println(noteRandom[3]);
                            break;  // N
                        case 1:
                            Serial.println(noteRandom[4]);
                            break;  // D
                        case 2:
                            Serial.println(noteRandom[2]);
                            break;  // U
                        case 3:
                            Serial.println(noteRandom[5]);
                            break;  // B
                        case 4:
                            Serial.println(noteRandom[1]);
                            break;  // OL
                        case 5:
                            Serial.println(noteRandom[6]);
                            break;  // OR
                        case 6:
                            Serial.println(noteRandom[0]);
                            break;  // S
                        case 7:
                            Serial.println(noteRandom[7]);
                            break;  // X
                    }
                } else {
                    switch (i) {
                        case 0:
                            Serial.println(note[3]);
                            break;  // N
                        case 1:
                            Serial.println(note[4]);
                            break;  // D
                        case 2:
                            Serial.println(note[2]);
                            break;  // U
                        case 3:
                            Serial.println(note[5]);
                            break;  // B
                        case 4:
                            Serial.println(note[1]);
                            break;  // OL
                        case 5:
                            Serial.println(note[6]);
                            break;  // OR
                        case 6:
                            Serial.println(note[0]);
                            break;  // S
                        case 7:
                            Serial.println(note[7]);
                            break;  // X
                    }
                }
            }
        } else {
            LMx[i] = false;
        }
        delay(25);
    }
}

/* Randomize Note */
void randomizeNote() {
    Serial.println("Randomize note!");
    int x;

    /* Isi dengan X */
    for (int i = 0; i <= 7; i++) {
        noteRandom[i] = 'X';
    }

    /* Isi dengan note */
    for (int i = 0; i <= 7; i++) {
        x = random(0, 7);
        while (noteRandom[x] != 'X') {
            x = (x + 1) % 8;
        }
        noteRandom[x] = note[i];
    }
}

/* LED Color (needs rework) */
void shiftLED() {
    for (int i = 0; i <= 36; i++) {
        leds[0][i] = CRGB::White;  // X
        leds[1][i] = CRGB::White;  // S
        leds[2][i] = CRGB::White;  // OR
        leds[3][i] = CRGB::White;  // OL
        leds[4][i] = CRGB::White;  //  B
        leds[5][i] = CRGB::White;  // U
        leds[6][i] = CRGB::White;  // D
        leds[7][i] = CRGB::White;  // N
    }
    FastLED.show();
}

/* Auto-Calibrate bottom and upper */

void setup() {
    /* Initialization */
    Serial.begin(57600);
    Wire.setClock(400000);
    Wire.begin();                                       // I2C begin
    lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);  // BH1750 begin
    randomSeed(analogRead(A0));

    //Serial.println("Starting SOUNDBOX v1.0");

    /* WS2811 Setup */
    //Serial.print("Initializing WS2811 LED");
    FastLED.addLeds<WS2811, 3, BRG>(leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 4, BRG>(leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 5, BRG>(leds[2], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 6, BRG>(leds[3], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 7, BRG>(leds[4], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 8, BRG>(leds[5], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 9, BRG>(leds[6], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 10, BRG>(leds[7], NUM_LEDS_PER_STRIP);

    FastLED.setBrightness(255);
    //Serial.println("Ready!");

    /* BH1750 Setup */
    // Serial.print("Initializing BH1750");
    /* while (!lightMeter.begin()) {
        Serial.print(".");
    } */
    //Serial.println("Ready!");
    for (i = 0; i < 8; i++) {
        LMx[i] = false;
    }
    j = 0;
    randomizeNote();
}

void loop() {
    //j++;
    //j = j % 3;
    //readLight();

    shiftLED();
    serialNote(1);
}
