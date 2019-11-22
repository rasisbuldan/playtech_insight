/**
 * TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

#define TCAADDR 0x70

int i;

void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

// standard Arduino setup()
void setup() {
    Serial.begin(57600);

    // Initialize the I2C bus (BH1750 library doesn't do this automatically)
    // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
    Wire.begin();

    lightMeter.begin();
    Serial.println(F("BH1750 Test"));
    //Serial.println("\nTCAScanner ready!");
}

void loop() {
    for(i=0;i<8;i++){
        tcaselect(i);
        uint16_t lux = lightMeter.readLightLevel();
        Serial.print("L[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(lux);
        Serial.print(" | ");
        delay(25);
    }
    Serial.println("");
}