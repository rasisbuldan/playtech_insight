# 1 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
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

*/
# 14 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
# 15 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino" 2
# 16 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino" 2
# 17 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino" 2

/* Macro */





/* Object Declaration */
BH1750 lightMeter;
CRGB leds[8][36];

/* Global Variable */
uint16_t LM[8]; // Lux value [1-8]
boolean LMx[8];
char note[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'H'};
char noteRandom[8];
int treshold = 10; // Lux treshold to trigger note sound
int i, j;
int state;
boolean randomized;

/* I2C Multiplexer Selector */
void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(0x70);
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
                            break; // N
                        case 1:
                            Serial.println(noteRandom[4]);
                            break; // D
                        case 2:
                            Serial.println(noteRandom[2]);
                            break; // U
                        case 3:
                            Serial.println(noteRandom[5]);
                            break; // B
                        case 4:
                            Serial.println(noteRandom[1]);
                            break; // OL
                        case 5:
                            Serial.println(noteRandom[6]);
                            break; // OR
                        case 6:
                            Serial.println(noteRandom[0]);
                            break; // S
                        case 7:
                            Serial.println(noteRandom[7]);
                            break; // X
                    }
                } else {
                    switch (i) {
                        case 0:
                            Serial.println(note[3]);
                            break; // N
                        case 1:
                            Serial.println(note[4]);
                            break; // D
                        case 2:
                            Serial.println(note[2]);
                            break; // U
                        case 3:
                            Serial.println(note[5]);
                            break; // B
                        case 4:
                            Serial.println(note[1]);
                            break; // OL
                        case 5:
                            Serial.println(note[6]);
                            break; // OR
                        case 6:
                            Serial.println(note[0]);
                            break; // S
                        case 7:
                            Serial.println(note[7]);
                            break; // X
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

/* Check button press (2) */
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
        leds[0][i] = CRGB::White; // X
        leds[1][i] = CRGB::White; // S
        leds[2][i] = CRGB::White; // OR
        leds[3][i] = CRGB::White; // OL
        leds[4][i] = CRGB::White; //  B
        leds[5][i] = CRGB::White; // U
        leds[6][i] = CRGB::White; // D
        leds[7][i] = CRGB::White; // N
    }
    FastLED.show();
}

/* Change State */
void changeState(){
    state += 1;
    if(state > 2){
        state = 0;
        randomized = false;
    }
}

void setup() {
    /* Initialization */
    Serial.begin(57600);
    Wire.setClock(400000);
    Wire.begin(); // I2C begin
    lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE); // BH1750 begin
    randomSeed(analogRead(A0));

    //Serial.println("Starting SOUNDBOX v1.0");

    /* WS2811 Setup */
    //Serial.print("Initializing WS2811 LED");
    FastLED.addLeds<WS2811, 3, BRG>(leds[0], 36);
    FastLED.addLeds<WS2811, 4, BRG>(leds[1], 36);
    FastLED.addLeds<WS2811, 5, BRG>(leds[2], 36);
    FastLED.addLeds<WS2811, 6, BRG>(leds[3], 36);
    FastLED.addLeds<WS2811, 7, BRG>(leds[4], 36);
    FastLED.addLeds<WS2811, 8, BRG>(leds[5], 36);
    FastLED.addLeds<WS2811, 9, BRG>(leds[6], 36);
    FastLED.addLeds<WS2811, 10, BRG>(leds[7], 36);

    FastLED.setBrightness(255);
    //Serial.println("Ready!");

    /* BH1750 Setup */
    // Serial.print("Initializing BH1750");
    /* while (!lightMeter.begin()) {

        Serial.print(".");

    } */
# 218 "c:\\Users\\Stoorm\\Documents\\GitHub\\insight_playtech\\SoundBox\\SoundBox.ino"
    //Serial.println("Ready!");
    for (i = 0; i < 8; i++) {
        LMx[i] = false;
    }

    /* Variable Initialization */
    randomized = false;
}

void loop() {
    //readLight();
    if (buttonIsPressed(2)) {
        changeState();
    } else {
        if (state == 0) {
            /* Standby */
            shiftLED(); // Ganti ke dynamic LED
        } else if (state == 1) {
            /* Randomized */
            if (!randomized) {
                randomizeNote();
                randomized = true;
            }
            shiftLED(); // Ganti ke dynamic LED
            serialNote(1);
        } else if (state == 2) {
            /* Playing */
            shiftLED(); // Ganti ke static LED
            serialNote(0);
        }
    }
}
