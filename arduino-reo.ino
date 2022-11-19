#include <Servo.h>

// Servo 
Servo servoEngine;
const int servo_pin = 12;

// Engine 1 ------------ {IN3, IN4, ENB}
const int EngineENA[3] = {5, 6, 7};

// Engine 2 ------------ {IN3, IN4, ENB}
const int EngineENB[3] = {8, 9, 10};

// Sensor SR04
const int SR04_trigger = 3;
const int SR04_echo = 4;

// Minimum viewing distance
const int min_distance = 15;

void setup() {
  Serial.begin(9600);

  servoEngine.attach(servo_pin);

  pinMode(SR04_trigger, OUTPUT);
  pinMode(SR04_echo, INPUT);
  digitalWrite(SR04_trigger, LOW);

  for (int i = 0; i <= 2; i++){
    pinMode(EngineENA[i], OUTPUT);
    pinMode(EngineENB[i], OUTPUT);
  }
}

void loop() {
  servoEngine.write(90);
  int get_distance = getDistance();

  if (get_distance <= min_distance) {
    stop();
    Scan(0);
    int r = getDistance();
    delay(500);

    Scan(180);
    int l = getDistance();
    delay(500);

    if (r > l) { moveTurnRight(); }

    if (r < l) { moveTurnLeft(); }

    //if (r == l) { moveBackward(); }

    stop();
    servoEngine.write(90);
  }
  moveForward();
  delay(150);
}

int getDistance(){
  long distance;
  long time;
  
  digitalWrite(SR04_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(SR04_trigger, LOW);

  time = pulseIn(SR04_echo, HIGH);
  distance = time / 59;

  return distance;
}

void Scan(int To){
  servoEngine.write(To);
  delay(500);
}

void moveEngineA(int state, int speed) {
  switch(state){
    case 1: // reverse
      digitalWrite(EngineENA[0], LOW);
      digitalWrite(EngineENA[1], HIGH);
      analogWrite(EngineENA[2], speed);
      break;
    case 2: // stop 
      digitalWrite(EngineENA[0], LOW);
      digitalWrite(EngineENA[1], LOW);
      analogWrite(EngineENA[2], 0);
      break;      
    default: //advance
      digitalWrite(EngineENA[0], HIGH);
      digitalWrite(EngineENA[1], LOW);
      analogWrite(EngineENA[2], speed); 
      break;
  }
}

void moveEngineB(int state, int speed) {
  switch(state){
    case 1: // reverse 
      digitalWrite(EngineENB[0], HIGH);
      digitalWrite(EngineENB[1], LOW);
      analogWrite(EngineENB[2], speed);
      break;
    case 2: // stop
      digitalWrite(EngineENB[0], LOW);
      digitalWrite(EngineENB[1], LOW);
      analogWrite(EngineENB[2], 0);
      break;
    default: // advance
      digitalWrite(EngineENB[0], LOW);
      digitalWrite(EngineENB[1], HIGH);
      analogWrite(EngineENB[2], speed);
      break;
  }
}

void moveForward(){
  moveEngineA(0, 250);
  moveEngineB(0, 250);
}

void moveTurnRight(){
  for (int i = 0; i < 3; i++){
    moveEngineA(0, 250);
    moveEngineB(1, 250);
    delay(200);
  }
}

void moveTurnLeft(){
  for (int i = 0; i < 3; i++){
    moveEngineA(1, 250);
    moveEngineB(0, 250);
    delay(200);
  }
}

void stop(){
  moveEngineA(2, 0);
  moveEngineB(2, 0);
}