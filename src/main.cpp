#include <Arduino.h>
#include "sensors.h"

void setup()
{
  Serial.begin(115200);

  // 初始化各个传感器模块
  initDS1302();
  initHCSR04();
  initDHT11();

  Serial.println("Sensor initialization completed!");
}

void loop()
{
  // 获取并打印时间
  String timeStr = getCurrentTimeString();
  Serial.print("Time: ");
  Serial.println(timeStr);

  // 获取并打印超声波距离
  long distance = getDistanceCM();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 获取并打印温湿度
  float temp = getTemperature();
  float humi = getHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.println(" %");

  Serial.println("-----------------------");
  delay(1000); // 每秒更新一次
}
