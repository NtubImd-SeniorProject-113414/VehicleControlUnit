// 定義連接到Arduino的感測器的腳位
const int sensorPin = 19; // 將數據腳位連接到Arduino的第7腳位

void setup() {
  // 初始化串行通信，以便於調試
  Serial.begin(9600);
  // 將感測器腳位設定為輸入模式
  pinMode(sensorPin, INPUT);
}

void loop() {
  // 讀取感測器腳位的數值
  int sensorValue = digitalRead(sensorPin);

  // 輸出感測器的數值到串行監視器
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // 判斷是否檢測到障礙物
  if (sensorValue == LOW) {
    // 如果讀取到的值為LOW，表示檢測到障礙物
    Serial.println("Obstacle detected!");
  } else {
    // 如果讀取到的值為HIGH，表示未檢測到障礙物
    Serial.println("No obstacle detected.");
  }

  // 稍作延遲，然後再次讀取
  delay(200);
}
