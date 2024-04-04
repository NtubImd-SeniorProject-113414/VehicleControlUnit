// 定義馬達控制腳位
const int ENA = 4; // PWM 控制速度的輸入腳位
const int IN1 = 5; // 馬達 1 正轉輸入腳位
const int IN2 = 6; // 馬達 1 反轉輸入腳位
const int IN3 = 7; // 馬達 2 正轉輸入腳位
const int IN4 = 8; // 馬達 2 反轉輸入腳位
const int ENB = 9; // PWM 控制速度的輸入腳位


void setup() {
  // 設置腳位模式
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  // 馬達 1 正轉
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // 馬達 2 正轉
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  // 設置速度
  analogWrite(ENA, 255); // 設置速度為 150 (0-255 範圍內的 PWM 值)
  analogWrite(ENB, 255); // 設置速度為 150 (0-255 範圍內的 PWM 值)
  // delay(5000); // 等待 5 秒
  
  // // 停止馬達
  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, LOW);
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, LOW);
  
  // delay(1000); // 等待 1 秒
}
