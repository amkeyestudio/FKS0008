#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "webpage.h" // 引入外部网页 HTML 代码文件

// ================= WiFi 配置 =================
const char* ssid = "FKS0008";         // 替换为你的 WiFi 名称
const char* password = "88888888";   // 替换为你的 WiFi 密码

// ================= 全局对象 =================
WebServer server(80); // 创建 Web 服务器实例，监听 80 端口
Servo myServo;        // 创建舵机对象

// ================= 电机引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮速度)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮速度)

// ================= 舵机引脚与状态 =================
const int SERVO_PIN = 11; // 舵机信号引脚
int servoAngle = 140;      // 舵机初始角度

// ================= 电机控制函数 =================

// 设置左右电机的 PWM 速度 (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // 写入左轮 PWM 值
  ledcWrite(MOTOR_BEN, rightSpeed); // 写入右轮 PWM 值
}

// 前进
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // 左轮正转
  digitalWrite(MOTOR_BIN, HIGH); // 右轮正转
  setMotor(leftSpeed, rightSpeed);
}

// 后退
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // 左轮反转
  digitalWrite(MOTOR_BIN, LOW);  // 右轮反转
  setMotor(leftSpeed, rightSpeed);
}

// 左转 (左轮停止，右轮前进)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // 左轮停止
  digitalWrite(MOTOR_BIN, HIGH); // 右轮正转
  setMotor(leftSpeed, rightSpeed);
}

// 右转 (左轮前进，右轮停止)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // 左轮正转
  digitalWrite(MOTOR_BIN, LOW);  // 右轮停止
  setMotor(leftSpeed, rightSpeed);
}

// 停止电机
void stop_Motor() {
  setMotor(0, 0); // 速度设为 0
}


// ================= Web 服务器请求处理 =================

// 处理控制指令
void handleCmd() {
  // 获取 URL 参数中的 "move" 值
  String moveCommand = server.arg("move");

  // 根据指令执行对应动作
  if (moveCommand == "forward") {
    forward(200, 200);
  } 
  else if (moveCommand == "backward") {
    back(200, 200);
  } 
  else if (moveCommand == "left") {
    left(200, 200);
  } 
  else if (moveCommand == "right") {
    right(200, 200);
  } 
  else if (moveCommand == "stop") {
    stop_Motor();
  } 
  else if (moveCommand == "claw_open") {
    myServo.write(140);   // 机械爪张开
  } 
  else if (moveCommand == "claw_close") {
    myServo.write(90); // 机械爪闭合
  }

  // 向客户端返回成功响应
  server.send(200, "text/plain", "OK");
}

// ================= 初始化设置 =================
void setup() {
  // 初始化串口，波特率 115200
  Serial.begin(115200);

  // 设置电机方向引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // 配置 PWM 通道 (ESP32 Arduino Core 3.x 语法)
  // 参数：引脚, 频率(1000Hz), 分辨率(8位，即0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // 初始化舵机
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle); // 设置初始角度

  // 开始连接 WiFi
  Serial.print("正在连接 WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // 等待 WiFi 连接成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi 连接成功！");
  Serial.print("IP 地址：");
  Serial.println(WiFi.localIP()); // 打印分配到的 IP 地址

  // 配置 Web 服务器路由
  // 访问根路径 "/" 时，发送网页 HTML 内容
  server.on("/", []() {
    server.send_P(200, "text/html; charset=UTF-8", index_html);
  });
  
  // 访问 "/cmd" 路径时，调用 handleCmd 函数处理指令
  server.on("/cmd", handleCmd);
  
  // 访问 "/distance" 路径时，返回超声波测距数据
  server.on("/distance", []() {
    server.send(200, "text/plain", String(0));
  });

  // 启动 Web 服务器
  server.begin();
  Serial.println("Web 服务器已启动！");
}

// ================= 主循环 =================
void loop() {
  // 持续处理客户端请求
  server.handleClient();
}