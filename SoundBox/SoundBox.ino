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
int treshold = 24;  // Lux treshold to trigger note sound
int i, j;

/* I2C Multiplexer Selector */
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

/* Lux Tresholding for Serial Print */
void serialNote() {
    for (i = 0; i < 8; i++) {
        tcaselect(i);
        LM[i] = lightMeter.readLightLevel();
        if (LM[i] < treshold) {
            if (!LMx[i]) {
                LMx[i] = !LMx[i];
                switch (i) {
                    case 0:
                        Serial.println("G");
                        break;
                    case 1:
                        Serial.println("F");
                        break;
                    case 2:
                        Serial.println("A");
                        break;
                    case 3:
                        Serial.println("E");
                        break;
                    case 4:
                        Serial.println("B");
                        break;
                    case 5:
                        Serial.println("D");
                        break;
                    case 6:
                        Serial.println("H");
                        break;
                    case 7:
                        Serial.println("C");
                        break;
                }
            }
        } else {
            LMx[i] = false;
        }
        delay(25);
    }
}

/* LED Color (needs rework) */
void shiftLED() {
    for (int i = 0; i <= 36; i++) {
        leds[0][i] = CRGB::Red;
        leds[1][i] = CRGB::Blue;
        leds[2][i] = CRGB::Green;
        leds[3][i] = CRGB::White;
        leds[4][i] = CRGB::Yellow;
        leds[5][i] = CRGB::Orange;
        leds[6][i] = CRGB::Cyan;
        leds[7][i] = CRGB::Purple;
    }
    FastLED.show();
}

/* Auto-Calibrate bottom and upper */


void setup() {
    /* Initialization */
    Serial.begin(57600);
    Wire.setClock(400000);
    Wire.begin();        // I2C begin
    lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);  // BH1750 begin

    //Serial.println("Starting SOUNDBOX v1.0");

    /* WS2811 Setup */
    //Serial.print("Initializing WS2811 LED");
    FastLED.addLeds<WS2811, 3>(leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 4>(leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 5>(leds[2], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 6>(leds[3], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 7>(leds[4], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 8>(leds[5], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 9>(leds[6], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 10>(leds[7], NUM_LEDS_PER_STRIP);

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
}

void loop() {
    //j++;
    //j = j % 3;
    //readLight();
    shiftLED();
    serialNote();
}