#include <Arduino.h>
#include "pins.h"
#include "stepper_motor.h"

StepperMotor motor;

void setup()
{
  Serial.begin(115200);
  motor.begin();
  motor.setSpeed(3); // 设置速度(ms)

  Serial.println("Stepper Motor Test - 9 Rotation Cycle");

  // 电机归零（假设初始位置为0）
  Serial.println("Homing motor...");
  motor.rotateAngle(-360); // 先逆时针转1圈确保归位
  delay(500);
  motor.rotateAngle(5); // 小角度调整到准确零点
  delay(500);

  Serial.println("Starting 9-rotation cycle...");
}

void loop()
{
  // 顺时针旋转9圈
  Serial.println("Rotating 9 turns CW");
  for (int i = 0; i < 9; i++)
  {
    motor.rotateAngle(360);
    Serial.print("Completed ");
    Serial.print(i + 1);
    Serial.println("/9 CW rotations");
    delay(200); // 圈间短暂停顿
  }

  delay(1000); // 方向改变前的停顿

  // 逆时针旋转9圈
  Serial.println("Rotating 9 turns CCW");
  for (int i = 0; i < 9; i++)
  {
    motor.rotateAngle(-360);
    Serial.print("Completed ");
    Serial.print(i + 1);
    Serial.println("/9 CCW rotations");
    delay(200); // 圈间短暂停顿
  }

  delay(1000); // 循环前的停顿
  Serial.println("Cycle completed, restarting...");
}