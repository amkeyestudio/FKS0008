#include <IRremote.hpp>  // 引入 IRremote 红外遥控库

// 定义红外接收模块信号引脚连接的 GPIO48
#define IR_RECEIVE_PIN 48

// ================= 电机引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮速度)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮速度)

void setup() {
  // 初始化串口通信，设置波特率为 115200
  Serial.begin(115200);
  delay(1000);  // 延时 1 秒，等待串口稳定

  Serial.println("ESP32-S3 红外接收器启动");
  // 初始化红外接收器，指定引脚并禁用板载 LED 反馈闪烁
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  Serial.println("等待红外信号...");

  // 设置电机方向引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT); 
  pinMode(MOTOR_BIN, OUTPUT);

  // 配置 PWM 通道 (ESP32 Arduino Core 3.x 语法)
  // 参数：引脚, 频率(1000Hz), 分辨率(8位，即0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // 初始状态确保电机停止
  stopMotor();
}

void loop() {
  // 检查是否成功解码到红外信号
  if (IrReceiver.decode()) {
    Serial.println("------ 红外数据 ------");
    
    // 获取解码后的 8 位命令码
    int irCommand = IrReceiver.decodedIRData.command;
    
    Serial.print("IR Value:");
    Serial.println(irCommand); 
    Serial.println("---------------------");
    
    // 根据 8 位命令码执行对应动作
    // 注意：不同遥控器的命令码可能不同，请根据串口打印的实际值进行修改
    switch (irCommand) {
      case 70: // 对应遥控器上的 '上' 键 (十进制 70)
        forward(200, 200); 
        break;
      case 21: // 对应遥控器上的 '下' 键 (十进制 21)
        back(200, 200); 
        break;
      case 68: // 对应遥控器上的 '左' 键 (十进制 68)
        left(200, 200); 
        break;
      case 67: // 对应遥控器上的 '右' 键 (十进制 67)
        right(200, 200); 
        break;
      case 64: // 对应遥控器上的 'OK' 键 (十进制 64)
        stopMotor(); 
        break;
      default:
        // 如果按下未定义的按键，不执行任何动作
        break;
    }

    // 恢复接收状态，清空缓存，准备接收下一个红外信号
    // 这一步非常关键，如果没有它，接收器在处理完一个信号后会停止工作
    IrReceiver.resume();
  }
}

// ================= 电机控制函数 =================

// 设置左右电机的 PWM 速度 (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // 写入左轮 PWM 值
  ledcWrite(MOTOR_BEN, rightSpeed); // 写入右轮 PWM 值
}

// 前进
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // 左轮正转
  digitalWrite(MOTOR_BIN, HIGH); // 右轮正转
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
  digitalWrite(MOTOR_AIN, LOW);  // 左轮停止
  digitalWrite(MOTOR_BIN, HIGH); // 右轮正转
  setMotor(leftSpeed, rightSpeed);
}

// 右转 (左轮前进，右轮停止)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // 左轮正转
  digitalWrite(MOTOR_BIN, LOW);  // 右轮停止
  setMotor(leftSpeed, rightSpeed);
}

// 停止电机
void stopMotor() {
  setMotor(0, 0); // 速度设为 0
}