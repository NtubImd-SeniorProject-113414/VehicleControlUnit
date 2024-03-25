// 定義馬達控制腳位
const int IN1 = 4; // 馬達 1 正轉輸入腳位
const int IN2 = 7; // 馬達 1 反轉輸入腳位
const int IN3 = 10; // 馬達 2 正轉輸入腳位
const int IN4 = 11; // 馬達 2 反轉輸入腳位

void setup() {
  // 設置腳位模式
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // 馬達 1 正轉
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // 馬達 2 正轉
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  }
