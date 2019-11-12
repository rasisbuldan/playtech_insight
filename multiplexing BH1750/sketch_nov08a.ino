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

void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

// standard Arduino setup()
void setup() {
    Serial.begin(9600);

    // Initialize the I2C bus (BH1750 library doesn't do this automatically)
    // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
    Wire.begin();

    lightMeter.begin();
    Serial.println(F("BH1750 Test"));
    //Serial.println("\nTCAScanner ready!");
}

void loop() {
    tcaselect(4);
    uint16_t lux = lightMeter.readLightLevel();
    Serial.print("Light1: ");
    Serial.print(lux);
    Serial.println(" lx");
    delay(1000);
    tcaselect(7);
    uint16_t yee = lightMeter.readLightLevel();
    Serial.print("Light2: ");
    Serial.print(yee);
    Serial.println(" lx");
    delay(1000);
}
