# 1 "c:\\Users\\rss75\\Documents\\GitHub\\playtech_insight\\serial-test\\serial-test.ino"
/* Global Variable */
int i;

void setup() {
    Serial.begin(57600);
    randomSeed(analogRead(0));
}

void loop() {
    i = random(1, 8);
    switch (i) {
        case 1:
            Serial.println("C");
            delay(100);
            break;
        case 2:
            Serial.println("D");
            delay(100);
            break;
        case 3:
            Serial.println("E");
            delay(100);
            break;
        case 4:
            Serial.println("F");
            delay(100);
            break;
        case 5:
            Serial.println("G");
            delay(100);
            break;
        case 6:
            Serial.println("A");
            delay(100);
            break;
        case 7:
            Serial.println("B");
            delay(100);
            break;
        case 8:
            Serial.println("H");
            delay(100);
            break;
        default:
            Serial.println("Error!");
            delay(100);
            break;
    }
    //Serial.println("");
}
