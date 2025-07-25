#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pins.h"
#include "sensors.h" // 确保包含传感器头文件

// OLED 显示屏宽度和高度
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 创建两个 I2C 实例
TwoWire I2C_OLED1 = TwoWire(0);
TwoWire I2C_OLED2 = TwoWire(1);

// 创建两个 OLED 显示对象
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED1, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED2, -1);

// 当前状态记录
String currentAngleLevel = "Unknown";
String currentHeightLevel = "Unknown";

// 角度等级转换
String getAngleLevel(int angle)
{
    if (angle == 0)
        return "Low";
    else if (angle == 30)
        return "Medium";
    else if (angle == 60 || angle == 90)
        return "High";
    else
        return "Unknown";
}

// 圈数等级转换
String getHeightLevel(int turns)
{
    if (turns == 0)
        return "Low";
    else if (turns == 3)
        return "Medium";
    else if (turns == 6 || turns == 9)
        return "High";
    else
        return "Unknown";
}

// 初始化 OLED1 显示屏
void init_OLED1()
{
    I2C_OLED1.begin(OLED1_SDA_PIN, OLED1_SCL_PIN, 400000);
    if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("OLED1 Init failed"));
    }
    display1.clearDisplay();
    display1.setTextSize(1);
    display1.setTextColor(SSD1306_WHITE);
    display1.setCursor(0, 0);
    display1.println("TABLE-state");
    display1.println("Angle: Unknown");
    display1.println("Height: Unknown");
    display1.display();
}

// 初始化 OLED2 显示屏
void init_OLED2()
{
    I2C_OLED2.begin(OLED2_SDA_PIN, OLED2_SCL_PIN, 400000);
    if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("OLED2 Init failed"));
    }
    display2.clearDisplay();
    display2.setTextSize(1);
    display2.setTextColor(SSD1306_WHITE);
    display2.setCursor(0, 0);
    display2.println("WELCOME TO");
    display2.println("OLED2");
    display2.display();
}

// 更新角度显示
void updateServoDisplay(int angle)
{
    currentAngleLevel = getAngleLevel(angle);
    display1.clearDisplay();
    display1.setTextSize(1);
    display1.setTextColor(SSD1306_WHITE);
    display1.setCursor(0, 0);
    display1.println("TABLE-state");
    display1.print("Angle: ");
    display1.println(currentAngleLevel);
    display1.print("Height: ");
    display1.println(currentHeightLevel);
    display1.display();
}

// 更新步进电机高度显示
void updateStepperDisplay(int turns)
{
    currentHeightLevel = getHeightLevel(turns);
    display1.clearDisplay();
    display1.setTextSize(1);
    display1.setTextColor(SSD1306_WHITE);
    display1.setCursor(0, 0);
    display1.println("TABLE-state");
    display1.print("Angle: ");
    display1.println(currentAngleLevel);
    display1.print("Height: ");
    display1.println(currentHeightLevel);
    display1.display();
}

// 在OLED2上显示传感器数据
void updateOLED2WithSensors()
{
    String timeStr = getCurrentTimeString();
    long distance = getDistanceCM();
    float temp = getTemperature();
    float hum = getHumidity();

    display2.clearDisplay();
    display2.setTextSize(1);
    display2.setTextColor(SSD1306_WHITE);
    display2.setCursor(0, 0);
    display2.println("OLED2");
    display2.println("--------------------");
    display2.println("Sensors-DATA");
    display2.print("DATA: ");
    display2.println(timeStr);
    display2.print("Distance: ");
    display2.print(distance);
    display2.println(" cm");
    display2.print("Temp: ");
    display2.print(temp, 1);
    display2.println(" *C");
    display2.print("Hum: ");
    display2.print(hum, 0);
    display2.println(" %");
    display2.println("--------------------");
    display2.display();
}

#endif // OLED_H
