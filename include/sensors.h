#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "pins.h"

// ========== DS1302 实时时钟 ==========
#include <ThreeWire.h>
#include <RtcDS1302.h>

// 初始化 DS1302 实例
ThreeWire myWire(DS1302_IO_PIN, DS1302_SCLK_PIN, DS1302_RST_PIN); // IO, SCLK, RST
RtcDS1302<ThreeWire> rtc(myWire);

// 初始化 RTC
void initDS1302()
{
    rtc.Begin();

    if (!rtc.IsDateTimeValid())
    {
        Serial.println("RTC无效，重置为默认时间");
        rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__)); // 设置为编译时间
    }

    if (rtc.GetIsWriteProtected())
    {
        rtc.SetIsWriteProtected(false);
    }

    if (!rtc.GetIsRunning())
    {
        rtc.SetIsRunning(true);
    }
}

// 获取当前时间，格式化为字符串 "YYYY-MM-DD HH:MM:SS"
String getCurrentTimeString()
{
    RtcDateTime now = rtc.GetDateTime();
    char buf[20];
    snprintf(buf, sizeof(buf), "%04u-%02u-%02u %02u:%02u:%02u",
             now.Year(), now.Month(), now.Day(),
             now.Hour(), now.Minute(), now.Second());
    return String(buf);
}

// ========== HC-SR04 超声波 ==========
long getDistanceCM()
{
    digitalWrite(HC_SR04_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(HC_SR04_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(HC_SR04_TRIG_PIN, LOW);

    long duration = pulseIn(HC_SR04_ECHO_PIN, HIGH, 30000); // 最长等待30ms
    long distance = duration * 0.034 / 2;                   // 计算距离（单位：厘米）
    return distance;
}

void initHCSR04()
{
    pinMode(HC_SR04_TRIG_PIN, OUTPUT);
    pinMode(HC_SR04_ECHO_PIN, INPUT);
}

// ========== DHT11 温湿度 ==========
#include <DHT.h>

#define DHTTYPE DHT11
DHT dht(DHT11_PIN, DHTTYPE);

void initDHT11()
{
    dht.begin();
}

float getTemperature()
{
    return dht.readTemperature(); // 摄氏度
}

float getHumidity()
{
    return dht.readHumidity(); // 百分比
}

#endif // SENSORS_H
