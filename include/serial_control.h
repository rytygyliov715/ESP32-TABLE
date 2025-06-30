// serial_control.h
#pragma once
#include <Arduino.h>
#include "servo_angle.h"
#include "stepper_motor.h"
#include "oled.h" // 注意包含方式：双引号 + 正确文件路径
#include <Wire.h>
#include "pins.h"

class SerialControl
{
private:
    ServoAngle &servo1;
    ServoAngle &servo2;
    StepperMotor &motor;
    bool motorHomed = false;

public:
    // 舵机控制
    void controlServo(uint8_t cmd)
    {
        switch (cmd)
        {
        case 10:
            servo1.setAngle(0);
            servo2.setAngle(0);
            Serial.println("Servos: 0°");
            updateServoDisplay(0); // 更新OLED显示
            break;
        case 11:
            servo1.setAngle(30);
            servo2.setAngle(30);
            Serial.println("Servos: 30°");
            updateServoDisplay(30); // 更新OLED显示
            break;
        case 12:
            servo1.setAngle(60);
            servo2.setAngle(60);
            Serial.println("Servos: 60°");
            updateServoDisplay(60); // 更新OLED显示
            break;
        case 13:
            servo1.setAngle(90);
            servo2.setAngle(90);
            Serial.println("Servos: 90°");
            updateServoDisplay(90); // 更新OLED显示
            break;
        }
    }

    // 步进电机控制
    void controlStepper(uint8_t cmd)
    {
        switch (cmd)
        {
        case 20: // 归零
            if (!motorHomed)
            {
                Serial.println("Homing motor...");
                motor.rotateAngle(-400); // 多转一些确保归零
                motor.rotateAngle(5);    // 小角度调整
                motorHomed = true;
                Serial.println("Motor homed");
                updateStepperDisplay(0); // 更新OLED显示
            }
            break;

        case 21: // 第3圈 (1080°)
            ensureHomed();
            motor.rotateAngle(1080);
            Serial.println("Motor at 3 turns");
            updateStepperDisplay(3); // 更新OLED显示
            break;

        case 22: // 第6圈 (2160°)
            ensureHomed();
            motor.rotateAngle(2160);
            Serial.println("Motor at 6 turns");
            updateStepperDisplay(6); // 更新OLED显示
            break;

        case 23: // 第9圈 (3240°)
            ensureHomed();
            motor.rotateAngle(3240);
            Serial.println("Motor at 9 turns");
            updateStepperDisplay(9); // 更新OLED显示
            break;
        }
    }

    // 确保步进电机已归零
    // 如果未归零，则自动执行归零操作
    // 归零后设置 motorHomed 为 true
    void ensureHomed()
    {
        if (!motorHomed) // 如果未归零
        {
            Serial.println("Motor not homed! Auto-homing..."); // 提示信息
            controlStepper(20);                                // 自动执行归零
        }
    }

    SerialControl(ServoAngle &s1, ServoAngle &s2, StepperMotor &m)
        : servo1(s1), servo2(s2), motor(m) {}

    // 初始化函数
    // 设置串口波特率并打印欢迎信息
    // 等待串口连接完成
    void begin()
    {
        // 初始化串口0
        Serial.begin(115200);
        // 初始化串口2
        Serial2.begin(115200, SERIAL_8N1, ASRPRO_RX_PIN, ASRPRO_TX_PIN);
        while (!Serial)
            ;
        Serial.println("Integrated Control System Ready");
        printHelp();
    }

    // 打印帮助信息
    // 显示可用命令和功能
    void printHelp()
    {
        Serial.println("=== Servo Commands ===");
        Serial.println("10 - Servos 0°");
        Serial.println("11 - Servos 30°");
        Serial.println("12 - Servos 60°");
        Serial.println("13 - Servos 90°");

        Serial.println("=== Stepper Commands ===");
        Serial.println("20 - Motor Home");
        Serial.println("21 - Motor to 3 turns");
        Serial.println("22 - Motor to 6 turns");
        Serial.println("23 - Motor to 9 turns");
    }

    // 更新函数
    // 检查串口是否有可用数据
    void update()
    {
        if (Serial2.available() > 0)
        {
            int cmd = Serial2.parseInt();
            while (Serial2.available() > 0)
                Serial2.read();

            if (cmd >= 10 && cmd <= 13)
            {
                controlServo(cmd);
            }
            else if (cmd >= 20 && cmd <= 23)
            {
                controlStepper(cmd);
            }
            else if (cmd != 0)
            {
                Serial.println("Invalid command");
            }
        }
        delay(10);
    }
};