## 接线表    

| 设备  | 引脚 | ESP32引脚 |
| ----- | ---- | --------- |
| OLED1 | SCL  | 8         |
| OLED1 | SDA  | 9         |
| OLED2 | SCL  | 10        |
| OLED2 | SDA  | 11        |


## 功能设计

舵机角度控制
servo_angle.cpp
描述：
模式1，舵机初始状态，舵机角度为0
模式2，舵机角度为30度
模式3，舵机角度为60度
模式4，舵机角度为90度
模式2，3，4相互变化时，需要先将舵机角度设置为0度（设置为模式1），再设置为30度，60度，90度

## 串口接收表    

| 串口接收 | 功能              |
| -------- | ----------------- |
| 10       | 舵机角度0         |
| 11       | 舵机角度30        |
| 12       | 舵机角度60        |
| 13       | 舵机角度90        |
| 20       | 步进电机归0       |
| 21       | 步进电机转至第3圈 |
| 22       | 步进电机转至第6圈 |
| 23       | 步进电机转至第9圈 |

## OLED设计

OLED1
--------------------
TABLE-state
Angle:Low/medium/high
Height:Low/medium/high
---------------------



我在使用vsc的PIO插件开发ESP32-S3,下面是我的项目的的PINS.H的播放内容：
```h
// ===== I2C OLED 显示屏 =====
#define OLED1_SCL_PIN 8  // I2C1 时钟
#define OLED1_SDA_PIN 9  // I2C1 数据
#define OLED2_SCL_PIN 10 // I2C2 时钟
#define OLED2_SDA_PIN 11 // I2C2 数据
```

还有的是我的平台IO的配置文件platformio.ini的内容：
```ini
[platformio]
index-url = http://mirrors.aliyun.com/pypi/simple/
trusted-host = mirrors.aliyun.com

[env:esp32dev]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

monitor_speed = 115200

lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306

```

现在，帮我编写一个OLED.H的文件，实现对OLED显示屏的初始化和基本操作功能。在两个屏幕分别显示不同的内容。
OLED1显示：HELLO WORLD , THIS IS OLED1
OLED2显示：WELCOME TO OLED2


下面是我用来控制电机和舵机的代码片段，我要求你在OLED.H文件中使用OLED1实现对舵机角度和步进电机圈数的显示功能。

```cpp
// serial_control.h
#pragma once
#include <Arduino.h>
#include "servo_angle.h"
#include "stepper_motor.h"

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
            }
            break;

        case 21: // 第3圈 (1080°)
            ensureHomed();
            motor.rotateAngle(1080);
            Serial.println("Motor at 3 turns");
            break;

        case 22: // 第6圈 (2160°)
            ensureHomed();
            motor.rotateAngle(2160);
            Serial.println("Motor at 6 turns");
            break;

        case 23: // 第9圈 (3240°)
            ensureHomed();
            motor.rotateAngle(3240);
            Serial.println("Motor at 9 turns");
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
```
显示情况如下：
OLED1
--------------------
TABLE-state
Angle:Low/medium/high
Height:Low/medium/high
---------------------


以下是PINS.H文件的关于传感器内容：
```cpp
// ===== DS1302 实时时钟 =====
#define DS1302_SCLK_PIN 4 // 时钟线
#define DS1302_IO_PIN 5   // 数据线（双向）
#define DS1302_RST_PIN 6  // 复位线

// ===== HC-SR04 超声波 =====
#define HC_SR04_TRIG_PIN 7  // 触发信号
#define HC_SR04_ECHO_PIN 15 // 回波信号

// ===== DHT11 温湿度传感器 =====
#define DHT11_PIN 16 // 数据线
```

现在，编写根据表格编写需要的功能程序，在一个.h文件中实现：
| 设备名称 | 功能描述                                |
| -------- | --------------------------------------- |
| DS1302   | 获取当前时间（翻译成年/月/日/时/分/秒） |
| HC-SR04  | 测量距离                                |
| DHT11    | 获取温湿度                              |

要求有以下功能：
1. DS1302 实时时钟获取当前时间，并格式化为字符串返回。
2. HC-SR04 超声波传感器测量距离，并返回距离值。
3. DHT11 温湿度传感器获取温度和湿度，并返回

保留get函数