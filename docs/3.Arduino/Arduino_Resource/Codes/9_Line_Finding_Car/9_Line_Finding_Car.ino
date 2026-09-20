#include <Wire.h>
#include <PCF8574.h>

PCF8574 pcf8574(0x20);

// ==================== 巡线传感器 ====================
#define OUTA P4  // 探头 A (最左侧)
#define OUTB P0  // 探头 B (左侧)
#define OUTC P1  // 探头 C (中间)
#define OUTD P2  // 探头 D (右侧)
#define OUTE P3  // 探头 E (最右侧

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

void forword(int leftSpeed, int rightSpeed) {
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

  pcf8574.pinMode(OUTA, INPUT);
  pcf8574.pinMode(OUTB, INPUT);
  pcf8574.pinMode(OUTC, INPUT);
  pcf8574.pinMode(OUTD, INPUT);
  pcf8574.pinMode(OUTE, INPUT);
  pinMode(10, OUTPUT);
  pcf8574.begin();

  Motor_Stop();

  Serial.println("Line Tracking Start");
}

void loop() {

  int A = !pcf8574.digitalRead(OUTA);
  int B = !pcf8574.digitalRead(OUTB);
  int C = !pcf8574.digitalRead(OUTC);
  int D = !pcf8574.digitalRead(OUTD);
  int E = !pcf8574.digitalRead(OUTE);

  Serial.print("ABCDE: ");
  Serial.print(A);
  Serial.print(B);
  Serial.print(C);
  Serial.print(D);
  Serial.println(E);

  // 生成5位状态码
  uint8_t state = 0;

  state |= A << 4;
  state |= B << 3;
  state |= C << 2;
  state |= D << 1;
  state |= E << 0;

  switch (state) {
    case 0b01110:
      forword(220, 220);
      break;

    case 0b11100:
      left(220, 220);
      break;

    case 0b11000:
      left(220, 220);
      break;

    case 0b10000:
      left(220, 220);
      break;

    case 0b00111:
      right(220, 220);
      break;

    case 0b00011:
      right(220, 220);
      break;

    case 0b00001:
      right(220, 220);
      break;
      
    case 0b00000:
      Motor_Stop();
      break;

    default:
      forword(150, 150);
      break;
  }
  delay(20);
}
