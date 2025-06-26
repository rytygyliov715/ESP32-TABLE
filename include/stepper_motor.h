// stepper_motor.h
#pragma once
#include <Arduino.h>
#include "pins.h"

class StepperMotor
{
private:
    uint8_t pins[4];
    int stepDelay = 3;   // 默认速度(ms)
    int currentStep = 0; // 当前步进状态(0-7)
    long totalSteps = 0; // 记录总步数

    // 步进序列
    // 28BYJ-48步进电机的四相驱动序列
    const uint8_t stepSequence[8][4] = {
        {1, 0, 0, 0},  // 步进状态0
        {1, 1, 0, 0},  // 步进状态1
        {0, 1, 0, 0},  // 步进状态2
        {0, 1, 1, 0},  // 步进状态3
        {0, 0, 1, 0},  // 步进状态4
        {0, 0, 1, 1},  // 步进状态5
        {0, 0, 0, 1},  // 步进状态6
        {1, 0, 0, 1}}; // 步进状态7

    // 设置步进电机的步进状态
    // 通过设置对应的引脚状态来控制步进电机的转动
    // 参数 step: 当前步进状态(0-7)
    // 通过 stepSequence 数组获取对应的引脚状态
    void setStep(int step)
    {
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(pins[i], stepSequence[step][i]);
        }
    }

public:
    // 构造函数
    // 初始化步进电机的引脚
    StepperMotor()
    {
        pins[0] = MOTOR_IN1_PIN; // 定义步进电机的引脚
        pins[1] = MOTOR_IN2_PIN; // 定义步进电机的引脚
        pins[2] = MOTOR_IN3_PIN; // 定义步进电机的引脚
        pins[3] = MOTOR_IN4_PIN; // 定义步进电机的引脚
    }

    void begin()
    {
        for (int i = 0; i < 4; i++)
        {
            pinMode(pins[i], OUTPUT);
            digitalWrite(pins[i], LOW);
        }
    }

    void setSpeed(int speed)
    {
        // stepDelay = constrain(speed, 1, 20); // 限制速度范围在1到20毫秒
        stepDelay = constrain(speed, 1, 20) / 3; // 除以3，加快3倍
        if (stepDelay < 1)
            stepDelay = 1; // 防止为0
    }

    // 更精确的步进控制
    void step(int steps)
    {
        int direction = (steps > 0) ? 1 : -1;
        steps = abs(steps);

        for (int i = 0; i < steps; i++)
        {
            currentStep = (currentStep + direction + 8) % 8;
            setStep(currentStep);
            totalSteps += direction;
            delay(stepDelay);
        }
    }

    // 精确角度控制
    void rotateAngle(float angle)
    {
        // 28BYJ-48: 64步/转 × 32减速比 = 2048步/转
        int steps = round(angle * 2048.0 / 360.0);
        step(steps);
    }

    // 获取当前位置(圈数)
    float getPosition()
    {
        return totalSteps / 2048.0;
    }
};