// 定義 BTS7960 的控制引腳
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

void setup() {
  // 設置控制引腳為輸出
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

  // 使能所有方向
  digitalWrite(R_EN_M1, HIGH);
  digitalWrite(L_EN_M1, HIGH);
  
  digitalWrite(R_EN_M2, HIGH);
  digitalWrite(L_EN_M2, HIGH);
  
  digitalWrite(R_EN_M3, HIGH);
  digitalWrite(L_EN_M3, HIGH);
  
  digitalWrite(R_EN_M4, HIGH);
  digitalWrite(L_EN_M4, HIGH);

  myservo.write(servoPin,45);
}

void moveForward() {
  // 所有馬達向前
  analogWrite(RPWM_M1, 20);   // 左前馬達
  analogWrite(LPWM_M1, 0);    

  analogWrite(RPWM_M2, 20);   // 右前馬達
  analogWrite(LPWM_M2, 0);    

  analogWrite(RPWM_M3, 20);   // 左後馬達
  analogWrite(LPWM_M3, 0);    

  analogWrite(RPWM_M4, 20);   // 右後馬達
  analogWrite(LPWM_M4, 0);    
}

void moveBackward() {
  // 所有馬達向後
  analogWrite(RPWM_M1, 0);   
  analogWrite(LPWM_M1, 30);  // 左前馬達

  analogWrite(RPWM_M2, 0);   
  analogWrite(LPWM_M2, 30);  // 右前馬達

  analogWrite(RPWM_M3, 0);   
  analogWrite(LPWM_M3, 30);  // 左後馬達

  analogWrite(RPWM_M4, 0);   
  analogWrite(LPWM_M4, 30);  // 右後馬達
}

void turnLeft() {
  // 向前行駛一段時間
  moveForward();  
  delay(1300);    // 前進1秒
  
  // 向後行駛一段時間，但右側馬達速度稍慢
  analogWrite(RPWM_M1, 0);   
  analogWrite(LPWM_M1, 30);  // 左前馬達向後

  analogWrite(RPWM_M2, 0);   
  analogWrite(LPWM_M2, 20);  // 右前馬達向後，速度稍慢

  analogWrite(RPWM_M3, 0);   
  analogWrite(LPWM_M3, 30);  // 左後馬達向後

  analogWrite(RPWM_M4, 0);   
  analogWrite(LPWM_M4, 20);  // 右後馬達向後，速度稍慢

  delay(1000);    // 後退1秒
}

void turnRight() {
  myservo.write(servoPin,100);
  analogWrite(RPWM_M1, 0);   
  analogWrite(LPWM_M1, 0);  // 左前馬達向後，速度稍慢
  delay(200);

  analogWrite(RPWM_M2, 0);   
  analogWrite(LPWM_M2, 0);  // 右前馬達向後
  delay(200);

  analogWrite(RPWM_M3, 0);
  analogWrite(LPWM_M3, 0);  // 左後馬達向後，速度稍慢
  delay(200);

  analogWrite(RPWM_M4, 55);
  analogWrite(LPWM_M4, 0);  // 右後馬達向後
  delay(200);

  delay(3000);    // 後退1秒
  myservo.write(servoPin,45);
}

int pos = 0;    // 設定 Servo 位置的變數

void loop() {
  moveForward();
  delay(2000);
  turnRight();
  delay(2000);
}

