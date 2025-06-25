#include <Arduino.h>
#include "pins.h"
#include "servo_angle.h"
#include "serial_servo_control.h"

// 创建舵机实例
ServoAngle servo1(SERVO1_PIN, 0); // 使用通道0
ServoAngle servo2(SERVO2_PIN, 1); // 使用通道1

// 创建串口控制器
SerialServoControl controller(servo1, servo2);

void setup()
{
  controller.begin();
  servo1.resetToZero();
  servo2.resetToZero();
  Serial.println("System initialized");
}

void loop()
{
  controller.update();
}