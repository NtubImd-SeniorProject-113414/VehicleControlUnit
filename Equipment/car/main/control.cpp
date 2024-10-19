#include "control.h"
#include <Arduino.h>

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

// 定義全域速度變數
int motorSpeed = 32; // 預設速度

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
  
  // 啟用馬達
  digitalWrite(R_EN_M1, HIGH);
  digitalWrite(L_EN_M1, HIGH);
  digitalWrite(R_EN_M2, HIGH);
  digitalWrite(L_EN_M2, HIGH);
  digitalWrite(R_EN_M3, HIGH);
  digitalWrite(L_EN_M3, HIGH);
  digitalWrite(R_EN_M4, HIGH);
  digitalWrite(L_EN_M4, HIGH);
}

// 設置馬達速度
void setSpeed(int newSpeed) {
  motorSpeed = newSpeed;
}

int getSpeed() {
  return motorSpeed;
}

// 前進
void moveForward() {
  analogWrite(RPWM_M1, motorSpeed);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, motorSpeed);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, motorSpeed);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, motorSpeed);
  analogWrite(LPWM_M4, 0);
}
// 後退
void moveBackward() {
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, motorSpeed);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, motorSpeed);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, motorSpeed);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, motorSpeed);
}

// 水平偏移 - 向右
void shiftRight() {
  analogWrite(RPWM_M1, motorSpeed);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, motorSpeed);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, motorSpeed);
  analogWrite(RPWM_M4, motorSpeed);
  analogWrite(LPWM_M4, 0);
}

// 水平偏移 - 向左
void shiftLeft() {
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, motorSpeed);
  analogWrite(RPWM_M3, motorSpeed);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M2, motorSpeed);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, motorSpeed);
}

// 原地左轉
void turnLeft() {
  analogWrite(RPWM_M1, 0);  // M1 反轉
  analogWrite(LPWM_M1, motorSpeed);
  analogWrite(RPWM_M2, motorSpeed); // M2 正轉
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 0);  // M3 反轉
  analogWrite(LPWM_M3, motorSpeed);
  analogWrite(RPWM_M4, motorSpeed); // M4 正轉
  analogWrite(LPWM_M4, 0);
}

// 原地右轉
void turnRight() {
  analogWrite(RPWM_M1, motorSpeed); // M1 正轉
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 0);  // M2 反轉
  analogWrite(LPWM_M2, motorSpeed);
  analogWrite(RPWM_M3, motorSpeed); // M3 正轉
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 0);  // M4 反轉
  analogWrite(LPWM_M4, motorSpeed);
}

// 左修正 - 微調，調整 M2、M4 的速度
void correctLeft() {
  analogWrite(RPWM_M2, min((int)(motorSpeed * 1.3), 50));
  analogWrite(RPWM_M4, min((int)(motorSpeed * 1.3), 50));
  analogWrite(RPWM_M1, motorSpeed / 4);
  analogWrite(RPWM_M3, motorSpeed / 4);
}

// 右修正 - 微調，調整 M2、M4 的速度
void correctRight() {
  analogWrite(RPWM_M1, min((int)(motorSpeed * 1.3), 50));
  analogWrite(RPWM_M3, min((int)(motorSpeed * 1.3), 50));
  analogWrite(RPWM_M2, motorSpeed / 4);
  analogWrite(RPWM_M4, motorSpeed / 4);
}

// 停止所有馬達
void stop() {
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 0);
}
