/* OmniRobot v0.2 */
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
char mot[40];
char logmsg[40];
int spd1, spd2, spd3;
int dir1, dir2, dir3;
float f1, f2, f3;
float a, a2, a3;
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
        angle = 0;
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
                angle = 271;
            } else if (IsNegative(roll)) {
                /* Negative X Axis */
                angle = 89;
            }
        }
    }
    return (angle % 360);
}

/*  */
float Radian(float deg) {
    return ((deg * 2 * PI) / 360);
}

/* Calculate parameter of each motor for robot */
void getSpeed(float ang, float p, float r) {
    /* Calculate motor speed (speed1, speed2, speed3) */
    int v = 255;
    
    f1 = v * sin(Radian(float(90 - ang)));
    f2 = v * sin(Radian(float(210 - ang)));
    f3 = v * sin(Radian(float(330 - ang)));

    /* a = (float(ang) * 2 * PI) / 360;
    a2 = float((120 * 2 * PI) / 360);
    a3 = float((120 * 2 * PI) / 360);
    Serial.print("p: ");
    Serial.print(a + a2);
    Serial.print(" | r: ");
    Serial.println(a + a3);
    f1 = ((-sin(a) * cos(a) * p) + (cos(a) * cos(a) * r) + 0.12 * 7) / 0.5;
    f2 = ((-sin(a + a2) * cos(a) * p) + (cos(a + a2) * cos(a) * r) + 0.12 * 7) / 0.5;
    f3 = ((-sin(a + a3) * cos(a) * p) + (cos(a + a3) * cos(a) * r) + 0.12 * 7) / 0.5; */
    /* sprintf(mot, "f1: %f | f2: %f | f3: %f", f1, f2, f3);
    Serial.print(mot); */
    /* Serial.print("f1: ");
    Serial.print(int(f1));
    Serial.print(" | f2: ");
    Serial.print(int(f2));
    Serial.print(" | f3: ");
    Serial.println(int(f3)); */
}

/* Processing float to speed and dir */
void processData(float f, int* spd, int* dir) {
    /* Serial.print("F: ");
    Serial.println(f); */
    (*spd) = abs(int(f));
    (*spd) = map((*spd), 0, 128, 0, 200);
    if (IsPositive(int(f))) {
        (*dir) = 1;
        (*spd) = 255 - (*spd);
    } else {
        (*dir) = 0;
    }
}

/* Transmit parameter to robot */
void transmit() {
    /* Concat all parameter */
    sprintf(send, "%03d%d%03d%d%03d%d", spd1, dir1, spd2, dir2, spd3, dir3);

    /* Send to HC12 */
    HC12.print(String(send));
    Serial.print("Message sent! | ");
    Serial.print(send);
    Serial.print(" | ");
}

/* Display parameter and calculation result */
void displayLog() {
    /* Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(" | Roll: ");
    Serial.print(roll);
    Serial.print(" | Angle: ");
    Serial.println(getAngle()); */

    sprintf(logmsg, "Angle: %d | Motor1: %d(%d) | Motor2: %d(%d) | Motor3: %d(%d)", angle, spd1, dir1, spd2, dir2, spd3, dir3);
    Serial.println(logmsg);
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
    getSpeed(float(getAngle()), float(pitch), float(roll));
    processData(f1, &spd1, &dir1);
    processData(f2, &spd2, &dir2);
    processData(f3, &spd3, &dir3);
    transmit();
    delay(100);
}