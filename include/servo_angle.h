// servo_angle.h
#pragma once
#include <Arduino.h>
#include "pins.h"

class ServoAngle
{
private:
    uint8_t channel;
    uint8_t pin;
    bool initialized = false;
    int currentAngle; // 当前角度

    void ensureInitialized()
    {
        if (!initialized)
        {
            ledcSetup(channel, 50, 12); // 50Hz, 12-bit resolution
            ledcAttachPin(pin, channel);
            initialized = true;
            delay(10); // 短暂延迟确保稳定
        }
    }

public:
    ServoAngle(uint8_t servoPin, uint8_t pwmChannel)
        : pin(servoPin), channel(pwmChannel) {}

    void setAngle(uint8_t angle)
    {
        ensureInitialized();
        angle = constrain(angle, 0, 180);
        uint32_t duty = map(angle, 0, 180, 163, 819); // 500-2400μs对应值
        ledcWrite(channel, duty);
        currentAngle = angle; // 更新当前角度
    }

    int getAngle() const { return currentAngle; }

    void resetToZero()
    {
        setAngle(0);
        delay(500);
    }
};