#include <Arduino.h>
#include "pins.h"
#include "servo_angle.h"
#include "stepper_motor.h"
#include "serial_control.h"
#include "oled.h" // 注意包含方式：双引号 + 正确文件路径

// 创建设备实例
ServoAngle servo1(SERVO1_PIN, 0); // 1号舵机
ServoAngle servo2(SERVO2_PIN, 1); // 2号舵机
StepperMotor motor;               // 步进电机

// 创建串口控制器
SerialControl controller(servo1, servo2, motor);

void setup()
{
  controller.begin(); // 初始化串口控制器
  // 初始化设备
  servo1.resetToZero();
  servo2.resetToZero();
  motor.begin();
  motor.setSpeed(3); // 设置步进电机速度

  init_OLEDs(); // 初始化 OLED 屏幕
}

void loop()
{
  controller.update();
}
