// 定义引脚
const int trigPin = 13;   // 触发引脚连接到 io13
const int echoPin = 12;  // 回声引脚连接到 io12

// 定义变量存储时间和距离
long duration;   // 声波往返的时间（微秒）
int distance;    // 计算出的距离（厘米）

void setup() {
  // 初始化串口通信，波特率设为 115200，这样数据显示更快更流畅
  Serial.begin(115200);
  
  // 设置引脚模式
  pinMode(trigPin, OUTPUT); // Trig 引脚作为输出，用来发送信号
  pinMode(echoPin, INPUT);  // Echo 引脚作为输入，用来接收信号
}

void loop() {
  // 第一步：确保 Trig 引脚是低电平，准备发射
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // 等待 2 微秒，确保信号稳定

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

  //限制超声波测距范围，因为超过3m或者低于2cm就不准了
  if(distance < 2 || distance > 300){
    distance = 0;
  }

  // 第五步：通过串口打印结果
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 等待 1 秒再进行下一次测量，避免数据刷新太快看不清
  delay(500);
}