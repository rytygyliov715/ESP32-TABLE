#pragma once
#include <Arduino.h>

// ===== DS1302 实时时钟 =====
#define DS1302_SCLK_PIN 4 // 时钟线
#define DS1302_IO_PIN 5   // 数据线（双向）
#define DS1302_RST_PIN 6  // 复位线

// ===== HC-SR04 超声波 =====
#define HC_SR04_TRIG_PIN 7  // 触发信号
#define HC_SR04_ECHO_PIN 15 // 回波信号

// ===== DHT11 温湿度传感器 =====
#define DHT11_PIN 16 // 数据线

// ===== ASRPRO-4M 语音模块 =====
#define ASRPRO_RX_PIN 17 // 接收（连接ASRPRO的TX）
#define ASRPRO_TX_PIN 18 // 发送（连接ASRPRO的RX）

// ===== I2C OLED 显示屏 =====
#define OLED1_SCL_PIN 8  // I2C1 时钟
#define OLED1_SDA_PIN 9  // I2C1 数据
#define OLED2_SCL_PIN 10 // I2C2 时钟
#define OLED2_SDA_PIN 11 // I2C2 数据

// ===== ULN2003 步进电机驱动 =====
#define MOTOR_IN1_PIN 12
#define MOTOR_IN2_PIN 13
#define MOTOR_IN3_PIN 14
#define MOTOR_IN4_PIN 21

// ===== SG90 舵机 =====
#define SERVO1_PIN 35 // PWM输出
#define SERVO2_PIN 36 // PWM输出