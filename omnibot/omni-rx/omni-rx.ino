#include <SoftwareSerial.h>

SoftwareSerial HC12(12, 11);  // HC-12 TX Pin, HC-12 RX Pin

String sinyal;
String str1, str2, str3;
byte incomingByte;

void setup() {
    Serial.begin(57600);  // Serial port to computer
    HC12.begin(57600);    // Serial port to HC12

    pinMode(3, OUTPUT);   // IN1
    pinMode(4, OUTPUT);   // IN2
    pinMode(5, OUTPUT);   // IN3
    pinMode(6, OUTPUT);   // IN4
    pinMode(8, OUTPUT);   // SET_PIN
    pinMode(9, OUTPUT);   // IN5
    pinMode(10, OUTPUT);  // IN6

    digitalWrite(8, HIGH);
}

void loop() {
    while (HC12.available()) {         // If HC-12 has data
        incomingByte = HC12.read();    // Store each incoming byte from HC-12
        sinyal += char(incomingByte);  // Add each byte to ReadBuffer string variable
    }

    delay(10);  // Refresh rate sync (kalo ga pas datanya jd ngaco)

    if (sinyal.length() < 9) {  // Total panjang string yang ditransmit
        return;                 // Ulang terus sampe panjangnya pas
    } else {
        str1 = sinyal.substring(0, 3);
        str2 = sinyal.substring(3, 6);
        str3 = sinyal.substring(6, 9);
        Serial.print("str1: ");
        Serial.print(str1);
        Serial.print(" | str2: ");
        Serial.print(str2);
        Serial.print(" | str3: ");
        Serial.println(str3);
        sinyal = "";
    }

    if (str1 == "045") {
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
    } else if (str1 == "090") {
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
    } else if (str1 == "135") {
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
    }
    else{
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
    }
}
