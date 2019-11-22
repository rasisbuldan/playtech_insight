/* OmniRobot v0.2 */
/* To do:   [x] 1. Add infrared reading and transmit
            [x] 2. Data decomposition to motor driver

 */

#include <SoftwareSerial.h>

/* Macro */
#define PWM_1 3
#define PWM_2 5
#define PWM_3 9
#define DIR_1 4
#define DIR_2 6
#define DIR_3 10

/* Object Declaration */
SoftwareSerial HC12(12, 11);  // HC-12 TX Pin, HC-12 RX Pin

/* Global Variable */
char logmsg[100];
String sinyal;
String str1, str2, str3, str4, str5, str6;
int spd1, dir1, spd2, dir2, spd3, dir3;
byte incomingByte;
int infra;

/* Get data from HC-12 */
void getReceiver() {
    while (HC12.available()) {         // If HC-12 has data
        incomingByte = HC12.read();    // Store each incoming byte from HC-12
        sinyal += char(incomingByte);  // Add each byte to ReadBuffer string variable
    }
    delay(10);
}

/* Turn off motor */
void resetMotor() {
    digitalWrite(PWM_1, LOW);
    digitalWrite(DIR_1, LOW);
    digitalWrite(PWM_2, LOW);
    digitalWrite(DIR_2, LOW);
    digitalWrite(PWM_3, LOW);
    digitalWrite(DIR_3, LOW);
}

/* Drive motor */
void driveMotor() {
    analogWrite(PWM_1, spd1);
    analogWrite(PWM_2, spd2);
    analogWrite(PWM_3, spd3);
    digitalWrite(DIR_1, dir1);
    digitalWrite(DIR_2, dir2);
    digitalWrite(DIR_3, dir3);
}

/* Decompose and convert received data */
void convertData() {
    spd1 = (sinyal.substring(0, 3)).toInt();    // Motor1 speed
    dir1 = (sinyal.substring(3, 4)).toInt();    // Motor1 dir
    spd2 = (sinyal.substring(4, 7)).toInt();    // Motor2 speed
    dir2 = (sinyal.substring(7, 8)).toInt();    // Motor2 dir
    spd3 = (sinyal.substring(8, 11)).toInt();   // Motor3 speed
    dir3 = (sinyal.substring(11, 12)).toInt();  // Motor3 dir
}

/* Print log to serial monitor */
void printLog() {
    sprintf(logmsg, "Motor1: %d(%d) | Motor2: %d(%d) | Motor3: %d(%d) | Infra: %d", spd1, dir1, spd2, dir2, spd3, dir3, infra);
    Serial.println(logmsg);
}

void setup() {
    /* Protocol Initialization */
    Serial.begin(57600);  // Serial port to computer
    HC12.begin(57600);    // Serial port to HC12

    /* Pin initialization */
    for(int i=3; i<=10; i++){
        pinMode(i, OUTPUT);
    }

    digitalWrite(8, HIGH);
    resetMotor();
}

void loop() {
    /* Get Data from HC-12 */
    getReceiver();

    /* Check string length received */
    if (sinyal.length() < 9) {
        /* Terminate loop */
        return;
    } else {
        convertData();
        sinyal = "";
    }
    
    /* Drive motor */
    printLog();
    driveMotor();
}
