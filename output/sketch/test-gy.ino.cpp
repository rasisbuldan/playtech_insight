#include <Arduino.h>
#line 1 "c:\\Users\\rss75\\Documents\\GitHub\\playtech_insight\\omnibot\\test-gy.ino"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

#line 9 "c:\\Users\\rss75\\Documents\\GitHub\\playtech_insight\\omnibot\\test-gy.ino"
void setup();
#line 15 "c:\\Users\\rss75\\Documents\\GitHub\\playtech_insight\\omnibot\\test-gy.ino"
void loop();
#line 9 "c:\\Users\\rss75\\Documents\\GitHub\\playtech_insight\\omnibot\\test-gy.ino"
void setup(){
    Serial.begin(115200);
    Wire.begin();
    mpu.initialize();
}

void loop(){
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.print("ax: ");
    Serial.print(ax);
    Serial.print("\t | ay: ");
    Serial.print(ay);
    Serial.print("\t | az: ");
    Serial.print(az);
    Serial.print("\t | gx: ");
    Serial.print(gx);
    Serial.print("\t | gy: ");
    Serial.print(gy);
    Serial.print("\t | gz: ");
    Serial.println(gz);
    delay(100);
}
