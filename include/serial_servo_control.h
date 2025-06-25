// serial_servo_control.h
#pragma once
#include <Arduino.h>
#include "servo_angle.h"

class SerialServoControl
{
private:
    ServoAngle &servo1;
    ServoAngle &servo2;

    void processCommand(uint8_t cmd)
    {
        switch (cmd)
        {
        case 10:
            servo1.setAngle(0);
            servo2.setAngle(0);
            Serial.println("Servos: 0°");
            break;
        case 11:
            servo1.setAngle(30);
            servo2.setAngle(30);
            Serial.println("Servos: 30°");
            break;
        case 12:
            servo1.setAngle(60);
            servo2.setAngle(60);
            Serial.println("Servos: 60°");
            break;
        case 13:
            servo1.setAngle(90);
            servo2.setAngle(90);
            Serial.println("Servos: 90°");
            break;
        default:
            Serial.println("Invalid command");
            break;
        }
    }

public:
    SerialServoControl(ServoAngle &s1, ServoAngle &s2)
        : servo1(s1), servo2(s2) {}

    void begin()
    {
        Serial.begin(115200);
        while (!Serial)
            ; // 等待串口连接
        Serial.println("Servo Control System Ready");
        printHelp();
    }

    void printHelp()
    {
        Serial.println("Commands:");
        Serial.println("10 - 0°");
        Serial.println("11 - 30°");
        Serial.println("12 - 60°");
        Serial.println("13 - 90°");
    }

    void update()
    {
        if (Serial.available() > 0)
        {
            int cmd = Serial.parseInt();
            while (Serial.available() > 0)
                Serial.read(); // 清空缓冲区

            if (cmd != 0)
            { // 确保读取到有效数据
                Serial.print("Executing: ");
                Serial.println(cmd);
                processCommand(cmd);
            }
        }
        delay(10);
    }
};