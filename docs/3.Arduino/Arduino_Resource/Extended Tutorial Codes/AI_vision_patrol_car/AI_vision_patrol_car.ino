#include "AiCam.h"

AiCam aiCam(18, 17);
// ==================== 电机 ====================

#define MOTOR_AIN 40  // A路方向控制 (左轮)
#define MOTOR_AEN 41  // A路 PWM 控制 (左轮)
#define MOTOR_BIN 38  // B路方向控制 (右轮)
#define MOTOR_BEN 21  // B路 PWM 控制 (右轮)

// ===================================================

void setMotor(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);
  digitalWrite(MOTOR_BIN, HIGH);

  analogWrite(MOTOR_AEN, rightSpeed);
  analogWrite(MOTOR_BEN, leftSpeed);
}

void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);
  digitalWrite(MOTOR_BIN, HIGH);

  analogWrite(MOTOR_AEN, rightSpeed);
  analogWrite(MOTOR_BEN, leftSpeed);
}

void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);
  digitalWrite(MOTOR_BIN, HIGH);

  analogWrite(MOTOR_AEN, rightSpeed);
  analogWrite(MOTOR_BEN, leftSpeed);
}

void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);
  digitalWrite(MOTOR_BIN, LOW);

  analogWrite(MOTOR_AEN, rightSpeed);
  analogWrite(MOTOR_BEN, leftSpeed);
}

void Motor_Stop() {
  analogWrite(MOTOR_AEN, 0);
  analogWrite(MOTOR_BEN, 0);
}

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
  pinMode(MOTOR_AEN, OUTPUT);
  pinMode(MOTOR_BEN, OUTPUT);

  Motor_Stop();
  delay(1000);

  //初始化Ai Cam模块
  aiCam.begin();
  aiCam.setAiCamMode("line");
  delay(500);
}


void loop() {
  // put your main code here, to run repeatedly:
  aiCam.readEspSerial();  // 读取并处理 ESP 发来的数据

  //获取线条识别的数据
  int offsetVal = aiCam.getLineOffset();  //线条左右偏移量
  int angleVal = aiCam.getLineAngle();    //线条左右转角
  int validVal = aiCam.getLineValid();    //线条状态值

  if (angleVal >= -25 && angleVal <= 25 && offsetVal <= 50 && offsetVal >= -50 && validVal == 5) {
    forward(80, 80);
  } else if (angleVal > 25 || offsetVal > 50) {
    right(80,255);
  } else if (angleVal < -25 || offsetVal < -50) {
    left(255,80);
  }else {
    forward(80, 80);
    delay(300);
    Motor_Stop();
  }


}
