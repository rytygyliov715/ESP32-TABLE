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