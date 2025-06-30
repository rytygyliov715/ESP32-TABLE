#include <Arduino.h>
#include "oled.h"
#include "sensors.h"

void setup()
{
  Serial.begin(115200);

  // 初始化各个传感器模块
  initDS1302();
  initHCSR04();
  initDHT11();

  // 初始化OLED
  init_OLED1();
  init_OLED2();
}

void loop()
{
  // 更新OLED2上的传感器数据
  updateOLED2WithSensors();

  delay(1000); // 每1秒刷新一次
}
