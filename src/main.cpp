#include <Arduino.h>
#include "pins.h"
#include "servo_angle.h"
#include "stepper_motor.h"
#include "serial_control.h"

// 创建设备实例
ServoAngle servo1(SERVO1_PIN, 0);
ServoAngle servo2(SERVO2_PIN, 1);
StepperMotor motor;

// 创建串口控制器
SerialControl controller(servo1, servo2, motor);

void setup()
{
  controller.begin();

  // 初始化设备
  servo1.resetToZero();
  servo2.resetToZero();
  motor.begin();
  motor.setSpeed(3);
}

void loop()
{
  controller.update();
}