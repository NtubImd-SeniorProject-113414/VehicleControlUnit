#include "HX711.h"

// HX711 接線設定
const int DT_PIN = 14;
const int SCK_PIN = 12;
const int sample_weight = 15;  //基準物品的真實重量(公克)
HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale();  // 開始取得比例參數
  scale.tare();
  Serial.println("Nothing on it.");
  float empty_weight = scale.get_units(10);
  Serial.print("Empty weight reading: ");
  Serial.println(empty_weight);
  Serial.println("Please put sample object on it...");

  delay(5000); // 給足夠的時間放置基準物品

}

void loop() {
  float current_weight = scale.get_units(10);
  Serial.print("Current weight reading: ");
  Serial.println(current_weight);

  float scale_factor = current_weight / sample_weight;
  Serial.print("Scale factor: ");
  Serial.println(scale_factor, 6);  // 顯示比例參數，保留6位小數以提高精度
}
