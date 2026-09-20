/*
  项目名称: 打印 Wi-Fi IP 地址
  作者: Keyestudio
  描述: 介绍如何使用 ESP32S3 连接 Wi-Fi 并打印 ESP32S3 的 IP 地址
*/

// 导入 Wi-Fi 库文件
#include <WiFi.h>

// 请将 "your_SSID" 修改成您的 Wi-Fi 名称
const char* ssid = "your_SSID";
// 请将 "your_PASSWORD" 修改成您的 Wi-Fi 密码
const char* password = "your_PASSWORD";

void setup() {
  // 初始化串口通信，设置波特率为 115200
  Serial.begin(115200);
  
  // 初始化 Wi-Fi 连接，传入 SSID 和密码
  WiFi.begin(ssid, password);
  
  // 循环检查 Wi-Fi 连接状态
  // 如果未连接成功 (状态不是 WL_CONNECTED)，则一直处于连接中状态
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // 延时 1 秒
    Serial.println("Connecting to WiFi..."); // 打印连接提示信息
  }
  
  // 连接成功，退出 while 循环
  Serial.println("Connected to WiFi"); // 打印连接成功信息
  // 打印获取到的本地 IP 地址
  Serial.println(WiFi.localIP()); 
}

void loop() {
  // 主循环为空，因为连接操作只需在 setup 中执行一次
}