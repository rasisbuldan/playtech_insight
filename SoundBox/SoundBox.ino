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
char quizSequence[7][10] = {
    {'C', 'C', 'D', 'D', 'D', 'E', 'E', 'E', 'F', 'F'},     // 0
    {'A', 'C', 'D', 'A', 'D', 'F', 'E', 'G', 'F', 'A'},     // 1
    {'C', 'C', 'D', 'D', 'D', 'E', 'E', 'E', 'F', 'F'},     // 2
    {'C', 'C', 'D', 'D', 'D', 'E', 'E', 'E', 'F', 'F'},     // 3
    {'C', 'C', 'D', 'D', 'D', 'E', 'E', 'E', 'F', 'F'},     // 4
    {'C', 'C', 'D', 'D', 'D', 'E', 'E', 'E', 'F', 'F'},     // 5
    {'C', 'C', 'D', 'D', 'D', 'E', 'E', 'E', 'F', 'F'}};    // 6
char quizAttempt[10];
char noteRandom[8];
int treshold = 10;  // Lux treshold to trigger note sound
int i, j;
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
                addToArray();
            }
        } else {
            LMx[i] = false;
        }
        delay(25);
    }
}

/* Add to Array */
void addToArray(char not){
    quizAttempt[j] = not;
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
        leds[0][i] = CRGB::White;  // X
        leds[1][i] = CRGB::White;  // S
        leds[2][i] = CRGB::White;  // OR
        leds[3][i] = CRGB::White;  // OL
        leds[4][i] = CRGB::White;  // B
        leds[5][i] = CRGB::White;  // U
        leds[6][i] = CRGB::White;  // D
        leds[7][i] = CRGB::White;  // N
    }
    FastLED.show();
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

/* Check quiz sequence by note */
void checkSequence() {
    for(i = 0; i<10; i++){
        if(quizAttempt[i] != quizSequence[quiz][i]){
            quizTrue = false;
        }
    }
}

/* Change State */
void changeState() {
    state += 1;
    if (state > 3) {
        state = 0;
        randomized = false;
        randomizeQuiz();
    }
    if(state == 3){
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
    randomSeed(analogRead(A0));

    //Serial.println("Starting SOUNDBOX v1.0");

    FastLED.addLeds<WS2811, 3, BRG>(leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 4, BRG>(leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 5, BRG>(leds[2], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 6, BRG>(leds[3], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 7, BRG>(leds[4], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 8, BRG>(leds[5], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 9, BRG>(leds[6], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2811, 10, BRG>(leds[7], NUM_LEDS_PER_STRIP);

    FastLED.setBrightness(255);

    for (i = 0; i < 8; i++) {
        LMx[i] = false;
    }

    /* Variable Initialization */
    randomized = false;
    randomizeQuiz();

    //Serial.println("Ready!");
}

void loop() {
    //readLight();
    if (buttonIsPressed(2)) {
        changeState();
    } else {
        if (state == 0) {
            /* Standby */
            shiftLED();  // Ganti ke dynamic LED
        } else if (state == 1) {
            /* Randomized */
            if (!randomized) {
                randomizeNote();
                randomized = true;
            }
            if (buttonIsPressed(11)) [randomizeNote();
            ] shiftLED();  // Ganti ke dynamic LED
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
            if(j == 9){
                checkSequence();
                if(quiz == false){
                    blinkRedLED();
                }
                else{
                    blinkGreenLED();
                }
                j = 0;
            }
            
        }
    }
}