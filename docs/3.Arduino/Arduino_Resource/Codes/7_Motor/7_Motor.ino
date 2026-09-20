// ================= 引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮)

// ================= 运动控制函数 =================

// 前进：双轮正转
void forward(int leftSpeed, int rightSpeed) {
  Serial.println("状态：前进");
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮方向：正转
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮方向：正转
  analogWrite(MOTOR_AEN, leftSpeed);
  analogWrite(MOTOR_BEN, rightSpeed);
}

// 后退：双轮反转
void back(int leftSpeed, int rightSpeed) {
  Serial.println("状态：后退");
  digitalWrite(MOTOR_AIN, LOW);  // 左轮方向：反转
  digitalWrite(MOTOR_BIN, LOW);  // 右轮方向：反转
  analogWrite(MOTOR_AEN, leftSpeed);
  analogWrite(MOTOR_BEN, rightSpeed);
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
  analogWrite(MOTOR_AEN, 0);
  analogWrite(MOTOR_BEN, 0);
}

// ================= 初始化与主循环 =================

void setup() {
  // 初始化串口通信，波特率 115200
  Serial.begin(115200);
  Serial.println("电机驱动系统初始化...");

  // 设置 PWM 分辨率为 8 位 (0-255)
  // analogWriteResolution(8);

  // 配置方向控制引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_AEN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
  pinMode(MOTOR_BEN, OUTPUT);


  // 初始状态：停止电机
  stopMotor();
  Serial.println("初始化完成，开始运行！");
}

void loop() {
  // 1. 全速前进 2 秒
  forward(100, 100);
  delay(2000);

  // 2. 全速后退 2 秒
  back(100, 100);
  delay(2000);

  // 3. 原地左转 2 秒
  left(255, 255);
  delay(2000);

  // 4. 原地右转 2 秒
  right(255, 255);
  delay(2000);

  // 5. 停止 2 秒，准备下一次循环
  stopMotor();
  delay(2000);
}