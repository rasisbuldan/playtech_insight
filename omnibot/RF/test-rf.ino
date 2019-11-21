#include <SoftwareSerial.h>

#define SET_PIN 10

SoftwareSerial HC12(9,8);  // HC-12 TX Pin (none), HC-12 RX Pin

void setup() {
    Serial.begin(115200);
    HC12.begin(115200);

    pinMode(8, OUTPUT);
    digitalWrite(SET_PIN,HIGH);
    delay(2000);
    digitalWrite(SET_PIN,LOW);
}

void loop() {
    /* .. */
}