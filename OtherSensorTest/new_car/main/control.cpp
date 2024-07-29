#include <Servo.h>
#include "control.h"

Servo myservo;

void initControl() {
  myservo.attach(15);
  myservo.write(45);

  // 初始化控制引腳，與之前代碼相同
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
}

void moveForward() {
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
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 30);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 30);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 30);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 30);
}

void turnLeft() {
  myservo.write(100);
  delay(500);
  moveForward();
  delay(1300);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 30);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 20);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 30);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 20);
  delay(1000);
  myservo.write(45);
  delay(500);
}

void turnRight() {
  myservo.write(100);
  delay(500);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 0);
  delay(200);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 0);
  delay(200);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 0);
  delay(200);
  analogWrite(RPWM_M4, 55);
  analogWrite(LPWM_M4, 0);
  delay(200);
  delay(3000);
  myservo.write(45);
  delay(500);
}
