// 定义 LED 连接的引脚号，这里我们用的是 io42
const int ledPin = 42; 

void setup() {
  // 初始化串口通信，波特率设为 9600，方便我们在电脑上看到信息
  Serial.begin(9600);
  
  // 设置 ledPin (io42) 为输出模式，因为它要控制 LED 亮灭
  pinMode(ledPin, OUTPUT);
  
  // 在串口监视器打印一条消息，告诉我们程序开始运行了
  Serial.println("LED Blink Experiment Started!");
}

void loop() {
  // 1. 让 LED 亮起
  digitalWrite(ledPin, HIGH); 
  Serial.println("LED is ON"); // 在电脑屏幕上显示 "LED is ON"
  
  // 2. 等待 1000 毫秒（也就是 1 秒）
  delay(1000); 
  
  // 3. 让 LED 熄灭
  digitalWrite(ledPin, LOW);  
  Serial.println("LED is OFF"); // 在电脑屏幕上显示 "LED is OFF"
  
  // 4. 再等待 1000 毫秒（1 秒）
  delay(1000); 
}