// ================= Pin Definitions =================
#define MOTOR_AIN 40  // Motor A direction control (left wheel)
#define MOTOR_AEN 41  // Motor A PWM control (left wheel)
#define MOTOR_BIN 38  // Motor B direction control (right wheel)
#define MOTOR_BEN 21  // Motor B PWM control (right wheel)

// ================= Motor Control Functions =================

// Set PWM speeds for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write PWM value for left wheel
  ledcWrite(MOTOR_BEN, rightSpeed); // Write PWM value for right wheel
}

// Move Forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Move Backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse
  setMotor(leftSpeed, rightSpeed);
}

// Turn Left (Left wheel stops, right wheel moves forward)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel stops
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Turn Right (Left wheel moves forward, right wheel stops)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel stops
  setMotor(leftSpeed, rightSpeed);
}

// Stop Motors
void stopMotor() {
  setMotor(0, 0); // Set speed to 0
}
// ================= Initialization and Main Loop =================

void setup() {
  // Initialize serial communication with a baud rate of 115200
  Serial.begin(115200);
  Serial.println("Motor driver system initializing...");

  // Set PWM resolution to 8-bit (0-255)
  // analogWriteResolution(8);

  // Set motor direction pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);
  
  // Initial state: Stop motors
  stopMotor();
  Serial.println("Initialization complete, starting execution!");
}

void loop() {
  // 1. Move forward at full speed for 2 seconds
  forward(255, 255);
  delay(2000);

  // 2. Move backward at full speed for 2 seconds
  back(255, 255);
  delay(2000);
  
  // 3. Spin left in place for 2 seconds
  left(255, 255);
  delay(2000);
  
  // 4. Spin right in place for 2 seconds
  right(255, 255);
  delay(2000);
  
  // 5. Stop for 2 seconds, preparing for the next loop
  stopMotor();
  delay(2000);
}