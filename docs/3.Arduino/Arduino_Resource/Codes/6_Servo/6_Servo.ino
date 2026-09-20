#include <ESP32Servo.h>  // 引入 ESP32 舵机控制库

Servo myServo;           // 创建一个名为 myServo 的舵机对象

// 定义舵机信号线连接的引脚为 GPIO42（请根据实际接线修改）
const int servoPin = 42; 

void setup() {
  // 为 ESP32 分配 PWM 定时器（ESP32 特有操作）
  ESP32PWM::allocateTimer(0); 
  
  // 设置舵机控制信号的标准频率为 50Hz
  myServo.setPeriodHertz(50); 
  
  // 将舵机连接到指定引脚，并设置最小脉宽为 500 微秒，最大脉宽为 2400 微秒
  // 注意：SG90 微型舵机通常使用 500us 和 2400us；
  // 如果是 MG995 等大型舵机，默认使用 1000us 和 2000us，可直接写为 myServo.attach(servoPin);
  myServo.attach(servoPin, 500, 2400);   
  
  // 初始化时，让舵机先回到 90 度中间位置，防止突然剧烈运动
  myServo.write(90);        
  delay(1000);              // 等待 1000 毫秒（1 秒），让舵机稳定下来
}

void loop() {
  // 第一阶段：从 0 度平滑转到 180 度
  for (int angle = 0; angle <= 180; angle += 1) { 
    myServo.write(angle);   // 命令舵机转到当前的 angle 角度
    delay(15);              // 等待 15 毫秒，给舵机留出转动的时间
  }

  delay(1000);              // 到达 180 度后，停顿 1 秒钟

  // 第二阶段：从 180 度平滑转回 0 度
  for (int angle = 180; angle >= 0; angle -= 1) { 
    myServo.write(angle);   // 命令舵机转到当前的 angle 角度
    delay(15);              // 同样等待 15 毫秒
  }

  delay(1000);              // 回到 0 度后，再停顿 1 秒钟
}