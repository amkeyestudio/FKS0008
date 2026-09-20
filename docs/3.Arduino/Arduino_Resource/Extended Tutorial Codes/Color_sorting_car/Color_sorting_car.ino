#include "AiCam.h"
#include <ESP32Servo.h>

Servo myServo;  // 创建舵机对象

//创建实例
AiCam aiCam(18, 17);

// ================= 电机引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮速度)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮速度)

// ================= 舵机引脚与状态 =================
const int SERVO_PIN = 11;  // 舵机信号引脚
int servoAngle = 140;      // 舵机初始角度

// ================= 电机控制函数 =================

// 设置左右电机的 PWM 速度 (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);   // 写入左轮 PWM 值
  ledcWrite(MOTOR_BEN, rightSpeed);  // 写入右轮 PWM 值
}

// 前进
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮正转
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮正转
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
  digitalWrite(MOTOR_AIN, LOW);   // 左轮停止
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮正转
  setMotor(leftSpeed, rightSpeed);
}

// 右转 (左轮前进，右轮停止)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮正转
  digitalWrite(MOTOR_BIN, LOW);   // 右轮停止
  setMotor(leftSpeed, rightSpeed);
}

// 停止电机
void stop_Motor() {
  setMotor(0, 0);  // 速度设为 0
}

// ================= 初始化设置 =================
void setup() {
  // 初始化串口，波特率 115200
  Serial.begin(115200);

  //初始化Ai Cam模块
  aiCam.begin();
  aiCam.setAiCamMode("color");

  // 设置电机方向引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // 配置 PWM 通道 (ESP32 Arduino Core 3.x 语法)
  // 参数：引脚, 频率(1000Hz), 分辨率(8位，即0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  stop_Motor();

  // 初始化舵机
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle);  // 设置初始角度
}

void loop() {
  aiCam.readEspSerial();  // 读取并处理 ESP 发来的数据

  //获取颜色识别值
  String strColor = aiCam.getColor();

  if (strColor == "RED" || strColor == "YELLOW") {
    myServo.write(90);
    delay(300);
    if (strColor == "RED") {
      left(220, 220);
    } else {
      right(220, 220);
    }
    delay(300);
    forward(220, 220);
    delay(300);
    stop_Motor();
    delay(300);
    myServo.write(140);
    delay(200);
    back(220, 220);
    delay(300);
    if (strColor == "RED") {
      right(220, 220);
    } else {
      left(220, 220);
    }
    delay(300);
    stop_Motor();
  } else {
    stop_Motor();
  }
}