// 定义通信引脚，与接线表保持一致
#define CLK_Pin 9   // 时钟引脚，连接模块 CLK
#define DIN_Pin 8   // 数据引脚，连接模块 DIN

// 微笑图案数据 (16字节，对应16列，每字节8位对应8行)
unsigned char smile[] = {
  0x00, 0x00, 0x1C, 0x02, 0x02, 0x02, 0x5C, 0x40, 
  0x40, 0x5C, 0x02, 0x02, 0x02, 0x1C, 0x00, 0x00
};

// 爱心图案数据 (补充用于实现课程介绍中的功能)
unsigned char heart[] = {
    0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x7c,
    0x7c, 0x3e, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  // 设置引脚为输出模式
  pinMode(CLK_Pin, OUTPUT);
  pinMode(DIN_Pin, OUTPUT);
  
  // 初始化默认电平，确保处于空闲状态
  digitalWrite(CLK_Pin, HIGH);
  digitalWrite(DIN_Pin, HIGH);
  
  // 清屏操作，防止上电时显示乱码
  unsigned char clear_data[16] = {0};
  matrix_display(clear_data);
}

void loop() {
  // 1. 显示爱心图案，保持 3 秒
  matrix_display(heart);
  delay(3000);
  
  // 2. 显示微笑图案，保持 2 秒
  matrix_display(smile);
  delay(2000);
  
  // 3. 简单的滚动效果演示 (将数据循环左移)
  for (int shift = 0; shift < 16; shift++) {
    unsigned char scroll_data[16];
    for (int i = 0; i < 16; i++) {
      // 将 smile 数组循环左移，实现滚动效果
      scroll_data[i] = smile[(i + shift) % 16]; 
    }
    matrix_display(scroll_data);
    delay(150); // 滚动速度控制，数值越小滚动越快
  }
}

// 核心函数：将 16 字节的图案数据写入 AiP1640 显存
void matrix_display(unsigned char matrix_value[]) {
  // 第一步：发送数据命令 (0x40)，设置为写数据模式，地址自动加 1
  IIC_start();
  IIC_send(0x40);
  IIC_end();
  
  // 第二步：发送地址命令 (0xC0)，设置显存起始地址为 0
  IIC_start();
  IIC_send(0xC0);
  
  // 第三步：循环发送 16 字节的图案数据
  for (int i = 0; i < 16; i++) {
    IIC_send(matrix_value[i]);
  }
  IIC_end();
  
  // 第四步：发送显示控制命令 (0x8A)，开启显示，脉冲宽度设为 8/16 (最高亮度)
  IIC_start();
  IIC_send(0x8A);
  IIC_end();
}

// 模拟类 I2C 协议的起始条件：CLK 高电平时，DIN 由高变低
void IIC_start() {
  digitalWrite(CLK_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, LOW);
  delayMicroseconds(3);
}

// 模拟类 I2C 协议的数据传输：CLK 低电平时改变数据，高电平时锁存
void IIC_send(unsigned char send_data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(CLK_Pin, LOW);  // 拉低时钟，准备数据
    delayMicroseconds(3);
    
    // 根据字节的最低位设置 DIN 引脚电平
    if (send_data & 0x01) {
      digitalWrite(DIN_Pin, HIGH);
    } else {
      digitalWrite(DIN_Pin, LOW);
    }
    delayMicroseconds(3);
    
    digitalWrite(CLK_Pin, HIGH); // 拉高时钟，锁存数据
    delayMicroseconds(3);
    
    send_data = send_data >> 1;  // 数据右移一位，准备传输下一位
  }
}

// 模拟类 I2C 协议的停止条件：CLK 高电平时，DIN 由低变高
void IIC_end() {
  digitalWrite(CLK_Pin, LOW);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, LOW);
  delayMicroseconds(3);
  digitalWrite(CLK_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, HIGH);
  delayMicroseconds(3);
}