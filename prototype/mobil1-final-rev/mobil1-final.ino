#include <RF24.h>  // include RF24 library
#include <SPI.h>   // include SPI library
RF24 radio(7, 8);  // associate radio with RF24 library
byte addresses[][6] = {"12", "14"};
typedef struct {
    int number;
    int button;
    int opsi;
} dataStruct;
dataStruct data;
long durasi, jarak;
int i;
int k = 0;
int sendingdata;
int enA = 5;   //nanti diubah
int in1 = 10;  //nanti diubah
int in2 = 9;   //nanti diubah

int in3 = 6;  //nanti diubah
int in4 = 2;  //nanti diubah
int enB = 3;  //nanti diubah

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(1, addresses[0]);
    radio.openWritingPipe(addresses[1]);

    pinMode(A0, OUTPUT);  // set pin trig menjadi OUTPUT
    pinMode(A1, INPUT);   // set pin echo menjadi INPUT

    pinMode(in1, OUTPUT);  //driver motor DC
    pinMode(in2, OUTPUT);  //driver motor DC
    pinMode(enA, OUTPUT);  //driver motor DC
    pinMode(in3, OUTPUT);  //driver motor DC
    pinMode(in4, OUTPUT);  //driver motor DC
    pinMode(enB, OUTPUT);  //driver motor DC
    i = 0;
}

void sensor() {
    digitalWrite(A0, LOW);
    delayMicroseconds(8);
    digitalWrite(A0, HIGH);
    delayMicroseconds(8);
    digitalWrite(A0, LOW);
    delayMicroseconds(8);

    durasi = pulseIn(A1, HIGH);   // menerima suara ultrasonic
    jarak = (durasi / 2) / 29.1;  // mengubah durasi menjadi jarak (cm)
                                  // menampilkan jarak pada Serial Monitor
    Serial.print("Jarak: ");
    Serial.print(jarak);
    Serial.print(" | ");
}

void maju() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 90);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, 90);
}

void mundur() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, 90);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, 90);
}

void berhenti() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void loop() {
    //i++;
    Serial.print("Loop | ");
    sensor();
    radio.startListening();
    /*while(!radio.available()){
        Serial.print(".");
    }*/
    if (radio.available()) {
        Serial.print("received | ");
        radio.read(&data, sizeof(data));
    }

    Serial.print("data.n : ");
    Serial.print(data.number);
    Serial.print(" | data.b: ");
    Serial.print(data.button);
    Serial.print(" | data.o: ");
    Serial.print(data.opsi);
    Serial.print(" | ");

    k = jarak / 20;

    if (data.button == 1) {
        if (k == data.number) {
            Serial.print("berhenti | ");
            berhenti();
            data.opsi = 2;
        }
        if (k > data.number) {
            Serial.print("mundur | ");
            mundur();
            data.opsi = 0;
        }
        if (k < data.number) {
            Serial.print("maju | ");
            maju();
            data.opsi = 1;
        }
    }
    //if(jarak <= (20*k)+a && jarak >= (20*k)-a)
    if ((data.number == 9) && (170 <= jarak) && (jarak <= 175)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 8) && (150 <= jarak) && (jarak <= 160)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 7) && (130 <= jarak) && (jarak <= 140)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 6) && (110 <= jarak) && (jarak <= 120)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 5) && (90 <= jarak) && (jarak <= 100)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 4) && (70 <= jarak) && (jarak <= 80)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 3) && (50 <= jarak) && (jarak <= 60)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 2) && (30 <= jarak) && (jarak <= 40)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 1) && (jarak == 16)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }
    if ((data.number == 0) && (jarak == 2)) {
        berhenti();
        data.button = 0;
        sendingdata = 1;
        k = data.number;
    }

    if (sendingdata == 1) {
        Serial.print("kirim");
        radio.stopListening();
        radio.write(&data, sizeof(data));
        sendingdata = 0;
    }
    Serial.println("");
}
