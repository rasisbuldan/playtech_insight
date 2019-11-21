/* OmniRobot v0.1 */
/* To do:   [o] 1. Calculate angle and speed from accelerometer pitch & yaw
            [x] 2. Calculate motor parameter from angle
 */
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <Wire.h>

/* Macro */
#define SET_PIN 9
#define RX_PIN 11
#define TX_PIN 10

/* Object Declaration */
MPU6050 mpu;
SoftwareSerial HC12(TX_PIN, RX_PIN);  // HC-12 TX Pin (none), HC-12 RX Pin

/* Global Variable */
int pitch, roll;
int angle, speed, thrust;
String message;
char* text;
char send[20];
int speed1, speed2, speed3;
int dir1, dir2, dir3;
int treshold = 15;

/* Variable value checking */
/* Check positive value */
boolean IsPositive(int val) {
    return (val >= 0);
}

/* Check positive value */
boolean IsNegative(int val) {
    return (val < 0);
}

/* Get angle of movement for robot */
int getAngle() {
    /* Read normalized values */
    Vector normAccel = mpu.readNormalizeAccel();

    /* Calculate Pitch & Roll */
    pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
    roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;

    if (abs(pitch) < treshold && abs(roll) < treshold) {
        /* Do Nothing */
    } else {
        if ((abs(pitch) > treshold) && (abs(roll) > treshold)) {
            if (IsPositive(pitch) && IsPositive(roll)) {
                /* Q1 */
                angle = 315;
            } else if (IsPositive(pitch) && IsNegative(roll)) {
                /* Q2 */
                angle = 45;
            } else if (IsNegative(pitch) && IsNegative(roll)) {
                /* Q3 */
                angle = 135;
            } else if (IsNegative(pitch) && IsPositive(roll)) {
                /* Q4 */
                angle = 225;
            }
        } else if ((abs(pitch) > treshold) && (abs(roll) <= treshold)) {
            if (IsPositive(pitch)) {
                /* Positive Y Axis */
                angle = 0;
            } else if (IsNegative(pitch)) {
                /* Negative Y Axis */
                angle = 180;
            }
        } else if ((abs(pitch) <= treshold) && (abs(roll) > treshold)) {
            if (IsPositive(roll)) {
                /* Positive X Axis */
                angle = 270;
            } else if (IsNegative(roll)) {
                /* Negative X Axis */
                angle = 90;
            }
        }
    }
    return (angle % 360);
}

/* Calculate parameter of each motor for robot */
void getSpeed(int p, int r) {
    /* Calculate motor direction (dir1, dir2, dir3) */
    /* ... */

    /* Calculate motor speed (speed1, speed2, speed3) */
    /* ... */
}

/* Transmit parameter to robot */
void transmit() {
    /* Concat all parameter */
    //message = String(speed1) + String(dir1) + String(speed2) + String(dir2) + String(speed3) + String(dir3);
    //message = angle + pitch + roll + '\n';
    sprintf(send,"%03d%03d%03d",angle,pitch,roll);

    /* Send to HC12 */
    HC12.print(String(send));
    Serial.print("Message sent! | ");
}

/* Display parameter and calculation result */
void displayLog() {
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(" | Roll: ");
    Serial.print(roll);
    Serial.print(" | Angle: ");
    Serial.println(getAngle());
}

void setup() {
    /* Protocol Initialization */
    Serial.begin(57600);
    HC12.begin(57600);
    Wire.begin();

    /* MPU Initialization */
    Serial.println("Initializing MPU6050");
    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        Serial.print(".");
        delay(50);
    }

    /* HC12 Initialization */
    pinMode(SET_PIN, OUTPUT);
    digitalWrite(SET_PIN, HIGH);
}

void loop() {
    displayLog();
    transmit();
    delay(100);
}