// 定义蜂鸣器控制引脚为 IO10
const int buzzerPin = 10; 

// 自定义播放音符函数，封装发声、延时、停声逻辑，使代码更简洁
void playNote(int pin, int frequency, int duration) {
  tone(pin, frequency);       // 以指定频率发声
  delay(duration);            // 持续指定时间
  noTone(pin);                // 停止发声
  delay(50);                  // 音符间的短暂停顿，使声音更清晰
}

void setup() {
  // 初始化串口，波特率设置为 115200，用于调试输出
  Serial.begin(115200);
  // 设置蜂鸣器引脚为输出模式
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  Serial.println("Playing Alarm...");
  // 1. 警报阶段：发出短促、尖锐的“滴-滴-滴”声
  for (int i = 0; i < 3; i++) {
    playNote(buzzerPin, 1000, 200); // 1000Hz 尖锐声音，持续 200ms
    delay(200);                     // 停顿 200ms
  }
  
  // 2. 休息阶段
  Serial.println("Resting...");
  delay(1000); // 安静 1 秒钟

  Serial.println("Playing Scale...");
  // 3. 音乐阶段：演奏 Do-Re-Mi-Fa-Sol-La-Si 音阶
  playNote(buzzerPin, 262, 500); // 低 Do
  playNote(buzzerPin, 294, 500); // 低 Re
  playNote(buzzerPin, 330, 500); // 低 Mi
  playNote(buzzerPin, 349, 500); // 低 Fa
  playNote(buzzerPin, 392, 500); // 低 Sol
  playNote(buzzerPin, 440, 500); // 低 La
  playNote(buzzerPin, 494, 500); // 低 Si
  
  // 4. 循环前的长休息
  delay(2000); // 安静 2 秒后重新开始
}