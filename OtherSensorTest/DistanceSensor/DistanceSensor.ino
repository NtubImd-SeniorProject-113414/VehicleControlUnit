// 定義連接到Arduino的引腳
const int trigPin = 22; // Trigger Pin 橘
const int echoPin = 23; // Echo Pin 藍

void setup() {
  // 初始化串行通信，以便於調試
  Serial.begin(9600);
  // 定義觸發腳位和回聲腳位的模式
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration, distance;
  // 清除觸發腳位
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // 設置觸發腳位為高電位，持續10微秒
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 讀取回聲腳位，返回聲波往返時間(微秒)
  duration = pulseIn(echoPin, HIGH);
  
  // 計算距離
  distance = duration * 0.034 / 2; // 聲波速度340 m/s，換算為0.034 cm/us

  // 顯示距離值
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 延遲一秒後再次測量
  delay(1000);
}
