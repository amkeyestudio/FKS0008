// ================= 超声波引脚定义 =================
const int trigPin = 13;  // 触发引脚连接到 IO13
const int echoPin = 12;  // 回声引脚连接到 IO12

// ================= 距离阈值设定 =================
// 设定距离的“警戒线”（单位：厘米）
const long safeDistance = 30;   // 定义安全距离上限为 30 厘米。小于此距离停止或后退。
const long closeDistance = 10;  // 定义危险距离下限为 10 厘米。低于此距离后退。
const long maxDistance = 50;    // 定义最大跟随距离为 50 厘米。超过此距离小车停止，防止跑丢。

// ================= 电机引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮速度)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮速度)

// ================= 初始化函数 =================
void setup() {
  // 初始化串口通信，波特率设为 115200，这样数据显示更快更流畅
  Serial.begin(115200);
  Serial.println("系统初始化完成，超声波跟随小车启动！");

  // 设置超声波引脚模式
  pinMode(trigPin, OUTPUT);  // Trig 引脚作为输出，用来发送触发信号
  pinMode(echoPin, INPUT);   // Echo 引脚作为输入，用来接收回声信号

  // 配置电机方向控制引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_AEN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
  pinMode(MOTOR_BEN, OUTPUT);
  
  // 初始状态确保电机停止
  stopMotor();
}

// ================= 主循环函数 =================
void loop() {
  // 1. 获取前方距离
  long distance = getDistance();  // 调用测距函数，把算出来的距离存到变量 distance 中。

  // 2. 在串口监视器打印距离，方便我们调试和观察。
  Serial.print("当前距离: ");  // 打印提示文字。
  Serial.print(distance);      // 打印具体的距离数字。
  Serial.println(" cm");       // 打印单位 " cm"（厘米），并换行。

  // 3. 根据距离控制小车动作（核心逻辑判断）
  if (distance > safeDistance && distance <= maxDistance) {  
    // 如果测得的距离 大于 30厘米 并且 小于等于 50厘米
    // 距离合适，追上去！
    forward(150, 150);  // 调用“前进”函数，左右轮速度设为 150。
    
  } else if ((distance >= closeDistance && distance <= safeDistance) || distance > maxDistance) {  
    // 否则如果 距离在 10厘米 到 30厘米 之间，或者 距离大于 50厘米
    // 距离刚好保持跟随，或者超出最大范围防止跑丢
    stopMotor();  // 调用“停止”函数。
    
  } else if (distance < closeDistance && distance > 0) {  
    // 否则如果 距离 小于 10厘米 并且 大于 0（排除传感器故障导致的0或负数）
    // 距离太近，快后退！
    back(105, 150);  // 调用“后退”函数，左轮105，右轮150（可微调防止跑偏）。
  }

  // 稍微延迟（暂停）100毫秒（0.1秒）。
  // 让动作更平稳，不要一直疯狂发指令，也给传感器一点“喘息”和接收回声的时间。
  delay(100);
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
    distance = 0; // 超出范围视为无效数据，返回 0
  }
  
  return distance;
}

// ================= 运动控制函数 =================

// 前进：正转
void forward(int leftSpeed, int rightSpeed) {
  Serial.println("状态：前进");
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮方向：正转
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮方向：正转
  analogWrite(MOTOR_AEN, leftSpeed);   // 设置左轮速度 (PWM)
  analogWrite(MOTOR_BEN, rightSpeed);  // 设置右轮速度 (PWM)
}

// 后退：反转
void back(int leftSpeed, int rightSpeed) {
  Serial.println("状态：后退");
  digitalWrite(MOTOR_AIN, LOW);  // 左轮方向：反转
  digitalWrite(MOTOR_BIN, LOW);  // 右轮方向：反转
  analogWrite(MOTOR_AEN, leftSpeed);   // 设置左轮速度 (PWM)
  analogWrite(MOTOR_BEN, rightSpeed);  // 设置右轮速度 (PWM)
}

// 停止：关闭 PWM 输出
void stopMotor() {
  Serial.println("状态：停止");
  analogWrite(MOTOR_AEN, 0);  // 左轮速度设为 0
  analogWrite(MOTOR_BEN, 0);  // 右轮速度设为 0
}