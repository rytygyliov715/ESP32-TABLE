#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pins.h"

// OLED 显示屏宽度和高度（单位：像素）
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 使用两个 OLED 屏幕，分别挂在不同的 I2C 端口上
// 创建两个 Wire 对象
TwoWire I2C_OLED1 = TwoWire(0);
TwoWire I2C_OLED2 = TwoWire(1);

// 创建两个 OLED 显示对象，使用上述 Wire 对象
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED1, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED2, -1);

// 初始化 OLED 显示屏
void init_OLEDs()
{
    // 初始化 I2C 通信
    I2C_OLED1.begin(OLED1_SDA_PIN, OLED1_SCL_PIN, 400000); // 400kHz I2C
    I2C_OLED2.begin(OLED2_SDA_PIN, OLED2_SCL_PIN, 400000);

    // 初始化 OLED1
    if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // 默认I2C地址为0x3C
        Serial.println(F("OLED1 初始化失败"));
    }
    else
    {
        display1.clearDisplay();
        display1.setTextSize(1);
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.println(F("HELLO WORLD ,"));
        display1.println(F("THIS IS OLED1"));
        display1.display();
    }

    // 初始化 OLED2
    if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("OLED2 初始化失败"));
    }
    else
    {
        display2.clearDisplay();
        display2.setTextSize(1);
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.println(F("WELCOME TO"));
        display2.println(F("OLED2"));
        display2.display();
    }
}

#endif // OLED_H
