#include <ESP32Servo.h>  // 引入 ESP32 舵机控制库

Servo myServo;  // 创建一个名为 myServo 的舵机对象

// ================= 超声波引脚定义 =================
const int trigPin = 13;  // 触发引脚连接到 IO13
const int echoPin = 12;  // 回声引脚连接到 IO12

// 定义舵机信号线连接的引脚为 GPIO42
const int servoPin = 42;

// ================= 距离阈值设定 =================
// 定义危险距离下限为 15 厘米。低于此距离舵机选择 0-180 度测左右两边距离。
const long closeDistance = 15;

long leftDistance = 0;   // 左边测距值
long rightDistance = 0;  // 右边测距值

// ================= 电机引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮速度)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮速度)

// ================= 初始化函数 =================
void setup() {
  // 初始化串口通信，波特率设为 115200，这样数据显示更快更流畅
  Serial.begin(115200);
  Serial.println("系统初始化完成，超声波避障小车启动！");

  // 设置超声波引脚模式
  pinMode(trigPin, OUTPUT);  // Trig 引脚作为输出，用来发送触发信号
  pinMode(echoPin, INPUT);   // Echo 引脚作为输入，用来接收回声信号

  // 配置电机方向与速度控制引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_AEN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
  pinMode(MOTOR_BEN, OUTPUT);

  // 初始状态确保电机停止
  stopMotor();

  // 为 ESP32 分配 PWM 定时器（ESP32 特有操作，避免与电机 PWM 冲突）
  ESP32PWM::allocateTimer(0);
  // 设置舵机控制信号的标准频率为 50Hz
  myServo.setPeriodHertz(50);
  // 将舵机连接到指定引脚，并设置最小脉宽为 500 微秒，最大脉宽为 2400 微秒
  // 注意：SG90 微型舵机通常使用 500us 和 2400us；
  // 如果是 MG995 等大型舵机，默认使用 1000us 和 2000us，可直接写为 myServo.attach(servoPin);
  myServo.attach(servoPin, 500, 2400);

  // 初始化时，让舵机先回到 90 度中间位置，防止突然剧烈运动
  myServo.write(90);
  delay(1000);  // 等待 1000 毫秒（1 秒），让舵机稳定下来
}

// ================= 主循环函数 =================
void loop() {
  // 1. 获取前方距离
  long distance = getDistance();  // 调用测距函数，把算出来的距离存到变量 distance 中

  // 2. 在串口监视器打印距离，方便我们调试和观察
  Serial.print("当前前方距离: ");  // 打印提示文字
  Serial.print(distance);          // 打印具体的距离数字
  Serial.println(" cm");           // 打印单位 " cm"（厘米），并换行

  // 3. 根据距离控制小车动作（核心逻辑判断）
  if (distance <= closeDistance) {
    // 如果测得的距离小于等于 15 厘米，说明前方有障碍
    stopMotor();  // 距离小于15CM停止前进
    // 舵机带动超声波旋转到 180 度（左侧）
    myServo.write(180);
    delay(300);                    // 延时 300ms 稳定舵机旋转到位
    leftDistance = getDistance();  // 测量左侧距离
    delay(100);                    // 延时 100ms 消除信号抖动

    // 舵机带动超声波旋转到 0 度（右侧）
    myServo.write(0);
    delay(300);                     // 延时 300ms 稳定舵机旋转到位
    rightDistance = getDistance();  // 测量右侧距离
    delay(100);                     // 延时 100ms 消除信号抖动

    // 测距结束后舵机归位到 90 度（正前方）
    myServo.write(90);

    // 比较左右距离，决定转向方向
    if (leftDistance > rightDistance) {
      // 左侧距离更远，向左转
      left(200, 200);
    } else {
      // 右侧距离更远（或相等），向右转
      right(200, 200);
    }
    delay(500);

  } else {
    // 前方距离大于 15 厘米，安全，继续直行
    forward(150, 150);
  }

  delay(100);  // 主循环短暂延时，避免执行过快
}

// ================= 超声波测距函数 =================
/*
 * 函数目的：将超声波测距的代码封装到函数中，简化主循环代码
 * 函数名：getDistance
 * 函数功能：返回整数的距离值（单位：厘米）
*/
int getDistance() {
  // 定义变量存储时间和距离
  long duration;  // 声波往返的时间（微秒）
  int distance;   // 计算出的距离（厘米）

  // 第一步：确保 Trig 引脚是低电平，准备发射
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // 等待 2 微秒，确保信号稳定

  // 第二步：给 Trig 引脚一个 10 微秒的高电平脉冲，触发传感器发射超声波
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 第三步：读取 Echo 引脚的高电平持续时间
  // pulseIn 函数会等待引脚变高，然后计时，直到引脚变低
  duration = pulseIn(echoPin, HIGH);

  // 第四步：计算距离
  // 声音在空气中的速度大约是 0.034 厘米/微秒
  // 距离 = (时间 × 速度) / 2，因为声音走了来回
  distance = duration * 0.034 / 2;

  // 限制超声波测距范围，因为超过 3米 或者低于 2厘米 就不准了
  if (distance < 2 || distance > 300) {
    distance = 0;  // 超出范围视为无效数据，返回 0
  }

  return distance;
}

// ================= 运动控制函数 =================

// 前进：正转
void forward(int leftSpeed, int rightSpeed) {
  Serial.println("状态：前进");
  digitalWrite(MOTOR_AIN, HIGH);       // 左轮方向：正转
  digitalWrite(MOTOR_BIN, HIGH);       // 右轮方向：正转
  analogWrite(MOTOR_AEN, leftSpeed);   // 设置左轮速度 (PWM)
  analogWrite(MOTOR_BEN, rightSpeed);  // 设置右轮速度 (PWM)
}

// 后退：反转
void back(int leftSpeed, int rightSpeed) {
  Serial.println("状态：后退");
  digitalWrite(MOTOR_AIN, LOW);        // 左轮方向：反转
  digitalWrite(MOTOR_BIN, LOW);        // 右轮方向：反转
  analogWrite(MOTOR_AEN, leftSpeed);   // 设置左轮速度 (PWM)
  analogWrite(MOTOR_BEN, rightSpeed);  // 设置右轮速度 (PWM)
}

// 左转：左轮反转，右轮正转（差速转向）
void left(int leftSpeed, int rightSpeed) {
  Serial.println("状态：左转");
  digitalWrite(MOTOR_AIN, LOW);   // 左轮方向：反转
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮方向：正转
  analogWrite(MOTOR_AEN, leftSpeed);
  analogWrite(MOTOR_BEN, rightSpeed);
}

// 右转：左轮正转，右轮反转（差速转向）
void right(int leftSpeed, int rightSpeed) {
  Serial.println("状态：右转");
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮方向：正转
  digitalWrite(MOTOR_BIN, LOW);   // 右轮方向：反转
  analogWrite(MOTOR_AEN, leftSpeed);
  analogWrite(MOTOR_BEN, rightSpeed);
}

// 停止：关闭 PWM 输出
void stopMotor() {
  Serial.println("状态：停止");
  analogWrite(MOTOR_AEN, 0);  // 左轮速度设为 0
  analogWrite(MOTOR_BEN, 0);  // 右轮速度设为 0
}