#include <Arduino.h>
#include "pins.h"
#include "oled.h"
#include "sensors.h"
#include "servo_angle.h"
#include "stepper_motor.h"
#include "serial_control.h"
#include "web.h"

// 创建设备实例
ServoAngle servo1(SERVO1_PIN, 0); // 1号舵机
ServoAngle servo2(SERVO2_PIN, 1); // 2号舵机
StepperMotor motor;               // 步进电机

// 创建控制器实例
SerialControl controller(servo1, servo2, motor);

void setup()
{
  Serial.begin(115200);

  controller.begin(); // 初始化串口控制器

  // 初始化设备
  servo1.resetToZero();
  servo2.resetToZero();
  motor.begin();
  motor.setSpeed(3); // 设置步进电机速度

  // 初始化各个传感器模块
  initDS1302();
  initHCSR04();
  initDHT11();

  // 初始化OLED
  init_OLED1();
  init_OLED2();

  initWebServer(); // 初始化Web服务器
}

void loop()
{
  // 更新OLED2上的传感器数据
  updateOLED2WithSensors();
  controller.update();
  handleWebClient(); // 处理客户端请求

  delay(100); // 每0.1秒刷新一次
}
