#include <Servo.h>
#include "control.h"

#define RPWM_M1 16
#define LPWM_M1 17
#define R_EN_M1 18
#define L_EN_M1 19

#define RPWM_M2 21
#define LPWM_M2 22
#define R_EN_M2 23
#define L_EN_M2 25

#define RPWM_M3 26
#define LPWM_M3 27
#define R_EN_M3 32
#define L_EN_M3 33

#define RPWM_M4 4
#define LPWM_M4 5
#define R_EN_M4 13
#define L_EN_M4 14
#include <Servo.h>

Servo myservo = Servo();
const int servoPin = 15;
void initControl() {
  pinMode(RPWM_M1, OUTPUT);
  pinMode(LPWM_M1, OUTPUT);
  pinMode(R_EN_M1, OUTPUT);
  pinMode(L_EN_M1, OUTPUT);
  pinMode(RPWM_M2, OUTPUT);
  pinMode(LPWM_M2, OUTPUT);
  pinMode(R_EN_M2, OUTPUT);
  pinMode(L_EN_M2, OUTPUT);
  pinMode(RPWM_M3, OUTPUT);
  pinMode(LPWM_M3, OUTPUT);
  pinMode(R_EN_M3, OUTPUT);
  pinMode(L_EN_M3, OUTPUT);
  pinMode(RPWM_M4, OUTPUT);
  pinMode(LPWM_M4, OUTPUT);
  pinMode(R_EN_M4, OUTPUT);
  pinMode(L_EN_M4, OUTPUT);
  digitalWrite(R_EN_M1, HIGH);
  digitalWrite(L_EN_M1, HIGH);
  digitalWrite(R_EN_M2, HIGH);
  digitalWrite(L_EN_M2, HIGH);
  digitalWrite(R_EN_M3, HIGH);
  digitalWrite(L_EN_M3, HIGH);
  digitalWrite(R_EN_M4, HIGH);
  digitalWrite(L_EN_M4, HIGH);
  //myservo.write(servoPin, 45);
}

void moveForward() {
  myservo.write(servoPin, 45);
  delay(100);
  analogWrite(RPWM_M1, 40);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 40);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 40);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 40);
  analogWrite(LPWM_M4, 0);
  delay(250);
  analogWrite(RPWM_M1, 20);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 20);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 20);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 20);
  analogWrite(LPWM_M4, 0);
}

void moveBackward() {
  myservo.write(servoPin, 45);
  delay(100);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 40);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 40);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 40);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 40);
  delay(250);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 25);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 25);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 25);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 25);
}

void offsetRight(){
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 10);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 10);
  analogWrite(RPWM_M2, 50);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M4, 60);
  analogWrite(LPWM_M4, 0);
}

void offsetLeft(){
  analogWrite(RPWM_M1, 50);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M3, 60);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 10);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 10);
}

void turnLeft() {
  myservo.write(servoPin, 130);
  delay(200);
  myservo.write(servoPin, 100);
  delay(100);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 10);
  analogWrite(RPWM_M4, 70);
  analogWrite(LPWM_M4, 0);
}

void turnRight() {
  myservo.write(servoPin, 130);
  delay(200);
  myservo.write(servoPin, 100);
  delay(100);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 10);
  analogWrite(RPWM_M3, 70);
  analogWrite(LPWM_M3, 0);
}

void stop(){
  myservo.write(servoPin, 45);
  delay(100);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 0);
}
