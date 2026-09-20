
#define ESP_RX_PIN 18
#define ESP_TX_PIN 17

// ================= 引脚定义 =================
#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮)

// ================= 电机控制函数 =================

// 设置左右电机的 PWM 速度 (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);   // 写入左轮 PWM 值
  ledcWrite(MOTOR_BEN, rightSpeed);  // 写入右轮 PWM 值
}

// 前进
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮正转
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮正转
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
  digitalWrite(MOTOR_AIN, LOW);   // 左轮停止
  digitalWrite(MOTOR_BIN, HIGH);  // 右轮正转
  setMotor(leftSpeed, rightSpeed);
}

// 右转 (左轮前进，右轮停止)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // 左轮正转
  digitalWrite(MOTOR_BIN, LOW);   // 右轮停止
  setMotor(leftSpeed, rightSpeed);
}

// 停止电机
void stopMotor() {
  setMotor(0, 0);  // 速度设为 0
}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600, SERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN);
  Serial.begin(115200);  // USB 调试
  Serial.println("UNO ready");

  // 设置电机方向引脚为输出模式
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // 配置 PWM 通道 (ESP32 Arduino Core 3.x 语法)
  // 参数：引脚, 频率(1000Hz), 分辨率(8位，即0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // 初始状态：停止电机
  stopMotor();
  Serial.println("初始化完成，开始运行！");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    char cmd = Serial1.read();

    Serial.print("Receive: ");
    Serial.println(cmd);

    switch (cmd) {
      case 'F':
        // 电机前进
        {
          Serial.println("MOTOR FORWARD");
          forward(200, 200);
        }
        break;

      case 'B':
        // 电机后退
        {
          Serial.println("MOTOR BACKWARD");
          back(200, 200);
        }
        break;

      case 'L':
        // 左转
        {
          Serial.println("MOTOR LEFT");
          left(200, 200);
        }
        break;

      case 'R':
        // 右转
        {
          Serial.println("MOTOR RIGHT");
          right(200, 200);
        }
        break;

      case 'S':
        // 停止
        {
          Serial.println("MOTOR STOP");
          stopMotor();
        }
        break;

      case 'O':
        // 舵机打开
        Serial.println("SERVO OPEN");
        break;

      case 'C':
        // 舵机关闭
        Serial.println("SERVO CLOSE");
        break;

      case '1':
        Serial.println("EXT1");
        break;

      case '2':
        Serial.println("EXT2");
        break;
    }
  }
  delay(20);
}
