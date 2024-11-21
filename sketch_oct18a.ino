#include <SoftwareSerial.h>
#include <HCSR04.h>
#include <Servo.h>

SoftwareSerial serialdoBluetooth(2, 3);

#define Ex 9
#define Tx 11
#define distmax 200

#define engineFD 6
#define engineTD 5
#define engineTE 7
#define engineFE 8

#define speed 255
#define speed_back 200
#define speed_zero 0

HCSR04 hc(Tx, Ex);
Servo myservo;

int buzzer = 12;
char valueBluetooth;

void setup() {
  serialdoBluetooth.begin(9600);
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(engineFD, OUTPUT);
  pinMode(engineTD, OUTPUT);
  pinMode(engineFE, OUTPUT);
  pinMode(engineTE, OUTPUT);
  stop_engines();
  myservo.attach(4);
  myservo.write(30);
 
}

void loop() {
  if (serialdoBluetooth.available()) {
    valueBluetooth = serialdoBluetooth.read();
    Serial.println(valueBluetooth);
  }
  switch (valueBluetooth) {
    case 'C':
    myservo.write(30);
    delay(100);
      if (valueBluetooth == 'C'){
       sensor_Go();
      }else{
       stop_engines();
      }
      break;

    case 'B':
      myservo.write(30);
      back();
      break;

    case 'E':
    myservo.write(70);
    delay(100);
    if (valueBluetooth == 'E'){
       sensor_left();
      }else{
       stop_engines();
      }
      break;

    case 'D':
      myservo.write(0);
      delay(100);
      if (valueBluetooth == 'D'){
       sensor_right();
      }else{
       stop_engines();
      }
      break;

    case 'c': case 'b': case 'e': case 'd':
      stop_engines();
      break;

    default:
      break;
  }
}

void left() {
  analogWrite(engineFD, speed);
  analogWrite(engineFE, speed_zero);
  analogWrite(engineTE, speed_zero);
  valueBluetooth = "";
}
void right() {
  analogWrite(engineFE, speed);
  analogWrite(engineFD, speed_zero);
  analogWrite(engineTD, speed_zero);
  valueBluetooth = "";
}
void Go() {
  analogWrite(engineFD, speed);
  analogWrite(engineFE, speed);
  analogWrite(engineTD, speed_zero);
  analogWrite(engineTE, speed_zero);
  valueBluetooth = "c";
}
void back() {
  analogWrite(engineTD, speed_back);
  analogWrite(engineTE, speed_back);
  analogWrite(engineFD, speed_zero);
  analogWrite(engineFE, speed_zero);
  tone(buzzer, 1500); delay(500); noTone(buzzer); delay(500);
}
void stop_engines() {
  analogWrite(engineFD, speed_zero);
  analogWrite(engineTD, speed_zero);
  analogWrite(engineFE, speed_zero);
  analogWrite(engineTE, speed_zero);
}

void sensor_Go(){
bool loop_on = true; 
float dist;
while (loop_on){
  dist = hc.dist(); 
    if (dist <= 30.0) { 
      stop_engines();
      Serial.println(dist);
      Serial.println("You can't go.");
      break;
    } else {
      Go();
      Serial.println("Going!!!");
      break;
    }

    if (serialdoBluetooth.available()) {
    valueBluetooth = serialdoBluetooth.read();
    Serial.println(valueBluetooth);

    if (valueBluetooth != 'C'){
      loop_on = false;
      break;
    }
  }
}
}



void sensor_right(){
bool loop_on = true; 
while (loop_on){
  float dist;
  dist = hc.dist(); 
    if (dist <= 30.0) { 
      stop_engines();
      Serial.println("You can't go to right");
      break;
    } else {
      right();
      Serial.println("Going to right");
      break;
    }

    if (serialdoBluetooth.available()) {
    valueBluetooth = serialdoBluetooth.read();
    Serial.println(valueBluetooth);

    if (valueBluetooth != 'D'){
      loop_on = false;
      break;
    }
  }
}
}



void sensor_left(){
bool loop_on = true; 
while (loop_on){
  float dist;
  dist = hc.dist(); 
    if (dist <= 30.0) { 
      stop_engines();
      Serial.println("You can't go to left.");
      break;
    } else {
      left();
      Serial.println("Going to left");
      break;
    }

    if (serialdoBluetooth.available()) {
    valueBluetooth = serialdoBluetooth.read();
    Serial.println(valueBluetooth);

    if (valueBluetooth != 'E'){
      loop_on = false;
      break;
    }
  }
}
}