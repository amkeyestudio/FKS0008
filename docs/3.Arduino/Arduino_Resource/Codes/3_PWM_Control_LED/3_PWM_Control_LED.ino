// 定义 LED 连接的引脚为 io42
const int ledPin = 42; 

// 定义亮度变量，初始值为 0（最暗）
int brightness = 0;    

// 定义每次变化的步长，数值越小变化越平滑，但速度越慢
int fadeAmount = 5;    

void setup() {
  // 设置 ledPin 为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 1. 设置当前引脚的亮度
  // analogWrite 可以输出 PWM 信号，value 范围 0-255
  analogWrite(ledPin, brightness);

  // 2. 改变下一次的亮度值
  brightness = brightness + fadeAmount;

  // 3. 判断是否达到了最亮或最暗的边界
  // 如果亮度到了 0 或者 255，就反转变化方向
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount; 
  }

  // 4. 等待 30 毫秒，让人眼能看清变化过程
  // 如果这个时间太短，灯会闪得太快；太长，呼吸感不连贯
  delay(30);          
}