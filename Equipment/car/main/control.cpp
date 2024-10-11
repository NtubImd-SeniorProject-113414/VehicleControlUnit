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
}
// 前進
void moveForward() {
  stop();
  delay(350);
  analogWrite(RPWM_M1, 40);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 40);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 40);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 40);
  analogWrite(LPWM_M4, 0);
}

// 後退
void moveBackward() {
  stop();
  delay(300);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 40);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 40);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 40);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 40);
}

// 水平偏移 - 向右
void shiftRight() {
  stop();
  delay(300);
  analogWrite(RPWM_M1, 40);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 40);
  analogWrite(RPWM_M2, 0);
  analogWrite(LPWM_M2, 40);
  analogWrite(RPWM_M4, 40);
  analogWrite(LPWM_M4, 0);

}

// 水平偏移 - 向左
void shiftLeft() {
  stop();
  delay(300);
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 40);
  analogWrite(RPWM_M3, 40);
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M2, 40);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M4, 0);
  analogWrite(LPWM_M4, 40);
}

// 原地左轉
void turnLeft() {
  stop();
  delay(300);
  analogWrite(RPWM_M1, 0);  // M1 反轉
  analogWrite(LPWM_M1, 40);
  analogWrite(RPWM_M2, 40); // M2 正轉
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M3, 0);  // M3 反轉
  analogWrite(LPWM_M3, 40);
  analogWrite(RPWM_M4, 40); // M4 正轉
  analogWrite(LPWM_M4, 0);
}

// 原地右轉
void turnRight() {
  stop();
  delay(300);
  analogWrite(RPWM_M1, 40); // M1 正轉
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M2, 0);  // M2 反轉
  analogWrite(LPWM_M2, 40);
  analogWrite(RPWM_M3, 40); // M3 正轉
  analogWrite(LPWM_M3, 0);
  analogWrite(RPWM_M4, 0);  // M4 反轉
  analogWrite(LPWM_M4, 40);
}

// 左修正 - 微調，調整 M2、M4 的速度
void correctLeft() {
  analogWrite(RPWM_M1, 0);
  analogWrite(LPWM_M1, 10);
  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 10);
  delay(100); // 短暫修正
  analogWrite(RPWM_M1, 40);
  analogWrite(LPWM_M1, 0);
  analogWrite(RPWM_M3, 40);
  analogWrite(LPWM_M3, 0);
}

// 右修正 - 微調，調整 M2、M4 的速度
void correctRight() {
  analogWrite(RPWM_M2, 10);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M4, 10);
  analogWrite(LPWM_M4, 0);
  delay(100); // 短暫修正
  analogWrite(RPWM_M2, 40);
  analogWrite(LPWM_M2, 0);
  analogWrite(RPWM_M4, 40);
  analogWrite(LPWM_M4, 0);
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
