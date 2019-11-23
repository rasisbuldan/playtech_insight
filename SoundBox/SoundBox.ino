/****** SOUNDBOX v1.0 ******/
/* To do:   [v] 0. State (Standby,Finding,Play)
            [x] 1. Autocalibration (with button input)
            [x] 2. LED color breathing / shift
            [v] 3. Randomize note location (sensor)
            [v] 4. Test BH1750 mode
            [v] 5. Check each box led pin
            [x] 6. Test baud rate to 115200 or 250000
            [x] 7. WS2811 data rate
            [v] 8. Button interrupt (pin 2,3) => JANGAN PAKE INTERRUPT
            [x] 9. Blink when change state
            [x] 10. Check quiz answers
            [x] 11. Reset attempt
*/

#include <BH1750.h>
#include <FastLED.h>
#include <Wire.h>

/* Macro */
#define NUM_LEDS_PER_STRIP 36
#define NUM_LEDS_PER_STRIP_AMBIENT 36
#define DATA_PIN 3
#define NUM_STRIP 8
#define TCAADDR 0x70

/* Object Declaration */
BH1750 lightMeter;
CRGB leds[NUM_STRIP][NUM_LEDS_PER_STRIP];
// CRGB ledsAmbient[2][NUM_LEDS_PER_STRIP_AMBIENT];

/* Global Variable */
uint16_t LM[8];  // Lux value [1-8]
boolean LMx[8];
char note[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'H'};
char quizSequence[4][6] = {
    {'C', 'C', 'D', 'D', 'D', 'E'},  // 0
    {'A', 'C', 'D', 'A', 'D', 'F'},  // 1
    {'C', 'C', 'D', 'D', 'D', 'E'},  // 2
    {'C', 'C', 'D', 'D', 'D', 'E'}   // 3
};
int ledColor[6][3] = {
    {255, 0, 0},     // Red
    {200, 0, 200},   // Green
    {0, 0, 255},     // Blue
    {0, 200, 200},   //
    {0, 255, 0},     //
    {200, 200, 0}
};
char quizAttempt[6];
char noteRandom[8];
int treshold = 10;  // Lux treshold to trigger note sound
int i, j, k, l, m, n, s;
int state;
boolean randomized;
int quiz;
boolean quizTrue;

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
                            addToArray(note[3]);
                            break;  // N
                        case 1:
                            Serial.println(note[4]);
                            addToArray(note[4]);
                            break;  // D
                        case 2:
                            Serial.println(note[2]);
                            addToArray(note[2]);
                            break;  // U
                        case 3:
                            Serial.println(note[5]);
                            addToArray(note[5]);
                            break;  // B
                        case 4:
                            Serial.println(note[1]);
                            addToArray(note[1]);
                            break;  // OL
                        case 5:
                            Serial.println(note[6]);
                            addToArray(note[6]);
                            break;  // OR
                        case 6:
                            Serial.println(note[0]);
                            addToArray(note[0]);
                            break;  // S
                        case 7:
                            Serial.println(note[7]);
                            addToArray(note[7]);
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

/* Add to Array */
void addToArray(char noteQuiz) {
    quizAttempt[j] = noteQuiz;
    j++;
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

/* Check button press */
boolean buttonIsPressed(int pin) {
    static boolean b1_old = 0;
    boolean b1 = digitalRead(pin);
    boolean s1 = (b1 && !b1_old);
    b1_old = b1;
    return s1;
}

/* LED Color (needs rework) */
void shiftLED() {
    for (int i = 0; i <= 36; i++) {
        leds[0][i] = CRGB::Red;  // X
        leds[1][i] = CRGB::Red;  // S
        leds[2][i] = CRGB::Red;  // OR
        leds[3][i] = CRGB::Red;  // OL
        leds[4][i] = CRGB::Red;  // B
        leds[5][i] = CRGB::Red;  // U
        leds[6][i] = CRGB::Red;  // D
        leds[7][i] = CRGB::Red;  // N
    }
    FastLED.show();
}

/* Shift LED matrix */
void shiftLEDMatrix() {
    for (int s = 0; s <= 7; s++) {
        ledColor[(s + 1) % 7][1] = ledColor[s][1];
        ledColor[(s + 1) % 7][2] = ledColor[s][2];
        ledColor[(s + 1) % 7][3] = ledColor[s][3];
    }
}
/* LED Dynamics */
void shiftDynamicLED() {
    shiftLEDMatrix();
    n = (n + 1) % 2;
    if (n == 0) {
        for (int i = 0; i <= 18; i++) {
            leds[0][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[1][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
            leds[2][i].setRGB(ledColor[2][0], ledColor[2][1], ledColor[2][2]);
            leds[3][i].setRGB(ledColor[3][0], ledColor[3][1], ledColor[3][2]);
            leds[4][i].setRGB(ledColor[4][0], ledColor[4][1], ledColor[4][2]);
            leds[5][i].setRGB(ledColor[5][0], ledColor[5][1], ledColor[5][2]);
            leds[6][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[7][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
        }
        for (int i = 19; i <= 36; i++) {
            leds[0][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[1][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
            leds[2][i].setRGB(ledColor[2][0], ledColor[2][1], ledColor[2][2]);
            leds[3][i].setRGB(ledColor[3][0], ledColor[3][1], ledColor[3][2]);
            leds[4][i].setRGB(ledColor[4][0], ledColor[4][1], ledColor[4][2]);
            leds[5][i].setRGB(ledColor[5][0], ledColor[5][1], ledColor[5][2]);
            leds[6][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[7][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
        }
    } else if (n == 1) {
        for (int i = 0; i <= 18; i++) {
            leds[0][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[1][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
            leds[2][i].setRGB(ledColor[2][0], ledColor[2][1], ledColor[2][2]);
            leds[3][i].setRGB(ledColor[3][0], ledColor[3][1], ledColor[3][2]);
            leds[4][i].setRGB(ledColor[4][0], ledColor[4][1], ledColor[4][2]);
            leds[5][i].setRGB(ledColor[5][0], ledColor[5][1], ledColor[5][2]);
            leds[6][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[7][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
        }
        for (int i = 19; i <= 36; i++) {
            leds[0][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
            leds[1][i].setRGB(ledColor[2][0], ledColor[2][1], ledColor[2][2]);
            leds[2][i].setRGB(ledColor[3][0], ledColor[3][1], ledColor[3][2]);
            leds[3][i].setRGB(ledColor[4][0], ledColor[4][1], ledColor[4][2]);
            leds[4][i].setRGB(ledColor[5][0], ledColor[5][1], ledColor[5][2]);
            leds[5][i].setRGB(ledColor[0][0], ledColor[0][1], ledColor[0][2]);
            leds[6][i].setRGB(ledColor[1][0], ledColor[1][1], ledColor[1][2]);
            leds[7][i].setRGB(ledColor[2][0], ledColor[2][1], ledColor[2][2]);
        }
    }
    FastLED.show();
}

/* Turn Red LED */
void turnRedLED() {
    for (int i = 0; i <= 36; i++) {
        leds[0][i] = CRGB::Red;  // X
        leds[1][i] = CRGB::Red;  // S
        leds[2][i] = CRGB::Red;  // OR
        leds[3][i] = CRGB::Red;  // OL
        leds[4][i] = CRGB::Red;  // B
        leds[5][i] = CRGB::Red;  // U
        leds[6][i] = CRGB::Red;  // D
        leds[7][i] = CRGB::Red;  // N
    }
    FastLED.show();
}

/* Turn Green LED */
void turnGreenLED() {
    for (int i = 0; i <= 36; i++) {
        leds[0][i] = CRGB::Green;  // X
        leds[1][i] = CRGB::Green;  // S
        leds[2][i] = CRGB::Green;  // OR
        leds[3][i] = CRGB::Green;  // OL
        leds[4][i] = CRGB::Green;  // B
        leds[5][i] = CRGB::Green;  // U
        leds[6][i] = CRGB::Green;  // D
        leds[7][i] = CRGB::Green;  // N
    }
    FastLED.show();
}

/* Turn off LED */
void turnOffLED() {
    for (int i = 0; i <= 36; i++) {
        leds[0][i] = CRGB::Black;  // X
        leds[1][i] = CRGB::Black;  // S
        leds[2][i] = CRGB::Black;  // OR
        leds[3][i] = CRGB::Black;  // OL
        leds[4][i] = CRGB::Black;  // B
        leds[5][i] = CRGB::Black;  // U
        leds[6][i] = CRGB::Black;  // D
        leds[7][i] = CRGB::Black;  // N
    }
    FastLED.show();
}

/* Blink Red LED */
void blinkRedLED() {
    for (int l = 0; l <= 5; l++) {
        turnRedLED();
        delay(800);
        turnOffLED();
        delay(800);
    }
}

/* Blink Green LED */
void blinkGreenLED() {
    for (int l = 0; l <= 5; l++) {
        turnGreenLED();
        delay(800);
        turnOffLED();
        delay(800);
    }
}

/* Randomize Quiz Song */
void randomizeQuiz() {
    quiz = random(0, 6);
}

/* Play Quiz */
void playQuiz() {
    switch (quiz) {
        case 0:
            Serial.println("P");
            break;
        case 1:
            Serial.println("Q");
            break;
        case 2:
            Serial.println("R");
            break;
        case 3:
            Serial.println("S");
            break;
        case 4:
            Serial.println("T");
            break;
        case 5:
            Serial.println("U");
            break;
        case 6:
            Serial.println("V");
            break;
    }
}

/* Reset attempt */
void resetAttempt() {
    j = 0;
    quizTrue = true;
}

/* Check quiz sequence by note */
void checkSequence() {
    for (i = 0; i < 6; i++) {
        if (quizAttempt[i] != quizSequence[quiz][i]) {
            quizTrue = false;
        }
    }
}

/* Change State */
void changeState() {
    Serial.println("Change State!");
    state += 1;
    if (state > 3) {
        state = 0;
        randomized = false;
        randomizeQuiz();
    }
    if (state == 3) {
        j = 0;
        quizTrue = true;
    }
}

void setup() {
    /* Initialization */
    Serial.begin(57600);
    Wire.setClock(400000);
    Wire.begin();
    lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
    //randomSeed(analogRead(A0));

    Serial.println("Starting SOUNDBOX v1.0");

    FastLED.addLeds<WS2811, 3, BRG>(leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 4, BRG>(leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 5, BRG>(leds[2], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 6, BRG>(leds[3], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 7, BRG>(leds[4], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 8, BRG>(leds[5], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 9, BRG>(leds[6], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 10, BRG>(leds[7], NUM_LEDS_PER_STRIP);
    //FastLED.addLeds<WS2811, 12, BRG>(ledsAmbient[0], NUM_LEDS_PER_STRIP_AMBIENT);
    //FastLED.addLeds<WS2811, 13, BRG>(ledsAmbient[1], NUM_LEDS_PER_STRIP_AMBIENT);

    FastLED.setBrightness(255);

    for (i = 0; i < 8; i++) {
        LMx[i] = false;
    }

    /* Variable Initialization */
    randomized = false;
    randomizeQuiz();
    n = 0;

    Serial.println("Ready!");
}

void loop() {
    //readLight();
    Serial.print("state: ");
    Serial.println(state);
    if (buttonIsPressed(2)) {
        changeState();
    } else {
        if (state == 0) {
            /* Standby */
            shiftDynamicLED;  // Ganti ke dynamic LED
        } else if (state == 1) {
            /* Randomized */
            if (!randomized) {
                randomizeNote();
                randomized = true;
            }
            if (buttonIsPressed(11)) {
                randomizeNote();
            }
            shiftDynamicLED();  // Ganti ke dynamic LED
            serialNote(1);
        } else if (state == 2) {
            /* Quiz (not Randomized) */
            if (buttonIsPressed(11)) {
                playQuiz();
            }
            shiftLED();  // Ganti ke static LED
            serialNote(0);
        } else if (state == 3) {
            /* Check note */
            if (buttonIsPressed(11)) {
                resetAttempt();
            }
            if (j == 9) {
                checkSequence();
                if (quiz == false) {
                    blinkRedLED();
                } else {
                    blinkGreenLED();
                }
                j = 0;
            }
        }
    }
}