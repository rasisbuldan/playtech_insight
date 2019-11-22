/* Global Variable */
int i;
int delayTime = 250;

void setup() {
    Serial.begin(57600);
    randomSeed(analogRead(0));
}

void loop() {
    i = random(1, 8);
    switch (i) {
        case 1:
            Serial.println("C");
            delay(delayTime);
            break;
        case 2:
            Serial.println("D");
            delay(delayTime);
            break;
        case 3:
            Serial.println("E");
            delay(delayTime);
            break;
        case 4:
            Serial.println("F");
            delay(delayTime);
            break;
        case 5:
            Serial.println("G");
            delay(delayTime);
            break;
        case 6:
            Serial.println("A");
            delay(delayTime);
            break;
        case 7:
            Serial.println("B");
            delay(delayTime);
            break;
        case 8:
            Serial.println("H");
            delay(delayTime);
            break;
        default:
            Serial.println("Error!");
            delay(delayTime);
            break;
    }
    //Serial.println("");
}