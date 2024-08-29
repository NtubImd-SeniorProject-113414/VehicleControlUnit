// 定義每個紅外線傳感器連接的 GPIO 腳位
const int sensor1Pin = 33;
const int sensor2Pin = 25;
const int sensor3Pin = 26;
const int sensor4Pin = 27;

void setup() {
  // 初始化串口通訊，波特率設置為115200
  Serial.begin(115200);

  // 設置每個腳位為輸入模式
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
}

void loop() {
  // 讀取每個傳感器的數字輸出
  int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);
  int sensor3State = digitalRead(sensor3Pin);
  int sensor4State = digitalRead(sensor4Pin);

  // 打印出每個傳感器的狀態（HIGH表示有反射，LOW表示無反射）
  Serial.print("Sensor 1: ");
  Serial.print(sensor1State);
  Serial.print(" | Sensor 2: ");
  Serial.print(sensor2State);
  Serial.print(" | Sensor 3: ");
  Serial.print(sensor3State);
  Serial.print(" | Sensor 4: ");
  Serial.println(sensor4State);

  // 根據需要增加其他控制邏輯
  // ...

  // 延遲50ms，讓輸出易於閱讀
  delay(1000);
}
