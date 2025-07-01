#ifndef WEB_H
#define WEB_H

#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include "servo_angle.h"
#include "stepper_motor.h"
#include "sensors.h"

// 声明外部变量
extern ServoAngle servo1;
extern ServoAngle servo2;
extern StepperMotor motor;

// 配置热点名称和密码
const char *AP_SSID = "ESP32-S3-AP";
const char *AP_PASSWORD = "password123";

WebServer server(80); // 创建Web服务器对象，监听80端口

// 根目录请求处理函数
void handleRoot()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>智能课桌控制系统</title>
    <style>
         @import url('https://fonts.googleapis.com/css2?family=HarmonyOS+Sans&display=swap');

        :root {
            --harmony-primary: #007DFF;
            --harmony-success: #00C292;
            --harmony-warning: #FFA300;
            --harmony-danger: #FF4B5C;
            --harmony-bg: #F4F6F9;
            --harmony-dark: #1C1C1E;
            --harmony-light: #ffffff;
            --radius: 16px;
            --shadow: 0 4px 12px rgba(0, 0, 0, 0.06);
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            font-family: 'HarmonyOS Sans', sans-serif;
        }

        body {
            background-color: var(--harmony-bg);
            color: var(--harmony-dark);
            padding: 20px;
        }

        .container {
            max-width: 1000px;
            margin: auto;
        }

        header {
            background-color: var(--harmony-primary);
            color: white;
            padding: 24px;
            border-radius: var(--radius);
            box-shadow: var(--shadow);
            text-align: center;
            transition: all 0.3s ease;
        }

        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-top: 20px;
        }

        .card {
            background-color: var(--harmony-light);
            padding: 20px;
            border-radius: var(--radius);
            box-shadow: var(--shadow);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }

        .card:hover {
            transform: scale(1.015);
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.08);
        }

        .card h2 {
            margin-bottom: 15px;
            font-size: 1.2rem;
            color: var(--harmony-dark);
            display: flex;
            align-items: center;
        }

        .card h2 i {
            margin-right: 10px;
        }

        .sensor-data {
            display: flex;
            justify-content: space-between;
            margin-bottom: 12px;
            font-size: 1rem;
        }

        .sensor-label {
            color: #888;
        }

        .sensor-value {
            font-weight: bold;
            color: var(--harmony-primary);
        }

        .sensor-value.warning {
            color: var(--harmony-warning);
        }

        .sensor-value.danger {
            color: var(--harmony-danger);
        }

        .btn-group {
            display: flex;
            gap: 10px;
            flex-wrap: wrap;
        }

        .btn {
            flex: 1;
            min-width: 80px;
            padding: 12px;
            text-align: center;
            background-color: var(--harmony-primary);
            color: white;
            border: none;
            border-radius: var(--radius);
            font-weight: bold;
            transition: all 0.25s ease;
        }

        .btn:hover:not(:disabled) {
            transform: scale(1.05);
            opacity: 0.9;
        }

        .btn:disabled {
            background-color: #ccc;
            cursor: not-allowed;
        }

        .btn.active {
            box-shadow: 0 0 0 3px rgba(0, 125, 255, 0.2);
        }

        .btn-warning {
            background-color: var(--harmony-warning);
        }

        .btn-danger {
            background-color: var(--harmony-danger);
        }

        .btn-secondary {
            background-color: var(--harmony-success);
        }

        .tomato-clock {
            text-align: center;
            margin: 20px 0;
        }

        .clock-display {
            font-size: 2.8rem;
            font-weight: bold;
            margin: 16px 0;
            color: var(--harmony-dark);
        }

        .clock-progress {
            height: 12px;
            background-color: #e0e0e0;
            border-radius: 8px;
            overflow: hidden;
        }

        .progress-bar {
            height: 100%;
            background-color: var(--harmony-primary);
            width: 0%;
            transition: width 0.3s ease;
        }

        .notification {
            position: fixed;
            bottom: 20px;
            right: 20px;
            background-color: var(--harmony-success);
            color: white;
            padding: 16px 24px;
            border-radius: var(--radius);
            box-shadow: var(--shadow);
            transform: translateX(150%);
            opacity: 0;
            transition: all 0.4s ease;
            z-index: 1000;
        }

        .notification.show {
            transform: translateX(0);
            opacity: 1;
        }

        .notification.error {
            background-color: var(--harmony-danger);
        }

        @media (max-width: 768px) {
            .dashboard {
                grid-template-columns: 1fr;
            }

            .btn-group {
                flex-direction: column;
            }
        }
    </style>
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
</head>
<body>
    <div class="container">
        <header>
            <h1><i class="material-icons">desktop_windows</i> 智能课桌控制系统</h1>
            <p>ESP32 物联网控制平台</p>
        </header>

        <div class="dashboard">
            <div class="card">
                <h2><i class="material-icons">thermostat</i> 环境监测</h2>
                <div class="sensor-data">
                    <span class="sensor-label">温度:</span>
                    <span class="sensor-value" id="temperature">-- °C</span>
                </div>
                <div class="sensor-data">
                    <span class="sensor-label">湿度:</span>
                    <span class="sensor-value" id="humidity">-- %</span>
                </div>
                <div class="sensor-data">
                    <span class="sensor-label">距离:</span>
                    <span class="sensor-value" id="distance">-- cm</span>
                </div>
                <div class="sensor-data">
                    <span class="sensor-label">时间:</span>
                    <span class="sensor-value" id="time">--</span>
                </div>
            </div>

            <div class="card">
                <h2><i class="material-icons">settings</i> 桌面控制</h2>
                <div class="control-group">
                    <h3>桌面高度</h3>
                    <div class="btn-group">
                        <button id="height-zero" class="btn btn-primary">零位</button>
                        <button id="height-low" class="btn btn-secondary">低位</button>
                        <button id="height-mid" class="btn btn-warning">中位</button>
                        <button id="height-high" class="btn btn-danger">高位</button>
                    </div>
                </div>

                <div class="control-group">
                    <h3>桌面角度</h3>
                    <div class="btn-group">
                        <button id="angle-zero" class="btn btn-primary">0°</button>
                        <button id="angle-30" class="btn btn-secondary">30°</button>
                        <button id="angle-60" class="btn btn-warning">60°</button>
                        <button id="angle-90" class="btn btn-danger">90°</button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="notification" id="notification"></div>

    <script>
        // 更新传感器数据
        function updateSensorData() {
            fetch('/sensors')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temperature').textContent = data.temperature + ' °C';
                    document.getElementById('humidity').textContent = data.humidity + ' %';
                    document.getElementById('distance').textContent = data.distance + ' cm';
                    document.getElementById('time').textContent = data.time;
                })
                .catch(error => console.error('Error:', error));
            
            setTimeout(updateSensorData, 2000); // 每2秒更新一次
        }

        // 控制高度
        document.querySelectorAll('[id^="height-"]').forEach(btn => {
            btn.addEventListener('click', function() {
                const height = this.id.split('-')[1];
                fetch('/height?height=' + height)
                    .then(showNotification)
                    .catch(showError);
            });
        });

        // 控制角度
        document.querySelectorAll('[id^="angle-"]').forEach(btn => {
            btn.addEventListener('click', function() {
                const angle = this.id.split('-')[1];
                fetch('/angle?angle=' + angle)
                    .then(showNotification)
                    .catch(showError);
            });
        });

        // 显示通知
        function showNotification(response) {
            return response.text().then(text => {
                const notification = document.getElementById('notification');
                notification.textContent = text;
                notification.className = 'notification show';
                setTimeout(() => notification.className = 'notification', 3000);
            });
        }

        // 显示错误
        function showError(error) {
            const notification = document.getElementById('notification');
            notification.textContent = '操作失败: ' + error;
            notification.className = 'notification show error';
            setTimeout(() => notification.className = 'notification', 3000);
        }

        // 初始化
        document.addEventListener('DOMContentLoaded', function() {
            updateSensorData();
        });
    </script>
</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

// LED控制处理函数
void handleLED()
{
    String state = server.arg("state");
    if (state == "on")
    {
        digitalWrite(LED_BUILTIN, LOW);
        server.send(200, "text/plain", "LED已开启");
    }
    else if (state == "off")
    {
        digitalWrite(LED_BUILTIN, HIGH);
        server.send(200, "text/plain", "LED已关闭");
    }
    else
    {
        server.send(400, "text/plain", "无效请求");
    }
}

// 重启设备处理函数
void handleRestart()
{
    server.send(200, "text/plain", "设备将在2秒后重启...");
    delay(2000);
    ESP.restart();
}

// 获取所有传感器数据
void handleSensors()
{
    String json = "{";
    json += "\"temperature\":" + String(getTemperature()) + ",";
    json += "\"humidity\":" + String(getHumidity()) + ",";
    json += "\"distance\":" + String(getDistanceCM()) + ",";
    json += "\"time\":\"" + getCurrentTimeString() + "\"";
    json += "}";

    server.send(200, "application/json", json);
}

// 控制桌面高度
void handleHeight()
{
    String height = server.arg("height");
    if (height == "zero")
    {
        motor.rotateAngle(0);
        server.send(200, "text/plain", "高度已设置为零位");
    }
    else if (height == "low")
    {
        motor.rotateAngle(30);
        server.send(200, "text/plain", "高度已设置为低位");
    }
    else if (height == "mid")
    {
        motor.rotateAngle(60);
        server.send(200, "text/plain", "高度已设置为中位");
    }
    else if (height == "high")
    {
        motor.rotateAngle(90);
        server.send(200, "text/plain", "高度已设置为高位");
    }
    else
    {
        server.send(400, "text/plain", "无效的高度设置");
    }
}

// 控制桌面角度
void handleAngle()
{
    String angle = server.arg("angle");
    if (angle == "0")
    {
        servo1.setAngle(0);
        servo2.setAngle(0);
        server.send(200, "text/plain", "角度已设置为0度");
    }
    else if (angle == "30")
    {
        servo1.setAngle(30);
        servo2.setAngle(30);
        server.send(200, "text/plain", "角度已设置为30度");
    }
    else if (angle == "60")
    {
        servo1.setAngle(60);
        servo2.setAngle(60);
        server.send(200, "text/plain", "角度已设置为60度");
    }
    else if (angle == "90")
    {
        servo1.setAngle(90);
        servo2.setAngle(90);
        server.send(200, "text/plain", "角度已设置为90度");
    }
    else
    {
        server.send(400, "text/plain", "无效的角度设置");
    }
}

// 初始化Web服务器
void initWebServer()
{
    // // 设置内置LED控制
    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, HIGH);

    // 启动AP模式
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    Serial.println("AP模式已启动");
    Serial.print("SSID: ");
    Serial.println(AP_SSID);
    Serial.print("密码: ");
    Serial.println(AP_PASSWORD);
    Serial.print("IP地址: ");
    Serial.println(WiFi.softAPIP());

    // 设置路由处理函数
    server.on("/", handleRoot);
    server.on("/led", handleLED);
    server.on("/restart", handleRestart);
    server.on("/sensors", handleSensors);
    server.on("/height", handleHeight);
    server.on("/angle", handleAngle);

    // 启动服务器
    server.begin();
    Serial.println("HTTP服务器已启动");
}

// 在loop()中持续调用以处理客户端请求
void handleWebClient()
{
    server.handleClient();
}

#endif
