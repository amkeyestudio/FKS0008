#include <Wire.h>
#include <PCF8574.h>

// Initialize the I2C expansion board, address is typically 0x20
PCF8574 pcf8574(0x20);

// ==================== Line Tracking Sensor Pin Definitions ====================
// Note: The sensor outputs a low level (0) when detecting a black line, and a high level (1) when detecting a white line
#define OUTA P4  // Probe A (far left)
#define OUTB P0  // Probe B (left)
#define OUTC P1  // Probe C (middle)
#define OUTD P2  // Probe D (right)
#define OUTE P3  // Probe E (far right)

// ==================== Motor Pin Definitions ====================
#define MOTOR_AIN 40  // Channel A direction control (left wheel)
#define MOTOR_AEN 41  // Channel A PWM control (left wheel)
#define MOTOR_BIN 38  // Channel B direction control (right wheel)
#define MOTOR_BEN 21  // Channel B PWM control (right wheel)

// ==================== Speed Parameter Definitions ====================
#define BASE_SPEED 220  // Base driving speed
#define TURN_SPEED 220  // Speed when turning

// ===================================================

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

// Stop motors
void stopMotor() {
  setMotor(0, 0); // Set speed to 0
}

void setup() {
  // Initialize serial communication, set baud rate to 115200
  Serial.begin(115200);

  // Set motor control pins as output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
  pinMode(MOTOR_AEN, OUTPUT);
  pinMode(MOTOR_BEN, OUTPUT);
  
  // Set onboard LED pin as output (optional)
  pinMode(10, OUTPUT);

  // Initialize I2C expansion board
  pcf8574.begin();
  
  // Set line tracking sensor pins as input mode
  pcf8574.pinMode(OUTA, INPUT);
  pcf8574.pinMode(OUTB, INPUT);
  pcf8574.pinMode(OUTC, INPUT);
  pcf8574.pinMode(OUTD, INPUT);
  pcf8574.pinMode(OUTE, INPUT);

  // Stop motors in initial state
  stopMotor();

  Serial.println("Line Tracking Start");
}

void loop() {
  // Read sensor states and invert them
  // Because the sensor outputs 0 when detecting a black line, inverting makes 1 represent a black line and 0 represent a white line, which is more intuitive
  int A = !pcf8574.digitalRead(OUTA);
  int B = !pcf8574.digitalRead(OUTB);
  int C = !pcf8574.digitalRead(OUTC);
  int D = !pcf8574.digitalRead(OUTD);
  int E = !pcf8574.digitalRead(OUTE);

  // Print sensor states to the Serial Monitor
  Serial.print("ABCDE: ");
  Serial.print(A);
  Serial.print(B);
  Serial.print(C);
  Serial.print(D);
  Serial.println(E);

  // Generate a 5-bit status code, packing the states of the 5 sensors into a single byte
  uint8_t state = 0;
  state |= (A << 4); // Shift A to the highest bit
  state |= (B << 3);
  state |= (C << 2);
  state |= (D << 1);
  state |= (E << 0); // Shift E to the lowest bit

  // Execute corresponding actions based on the status code
  switch (state) {
    case 0b01110: // Middle 3 sensors detect the black line, drive straight normally
      forward(BASE_SPEED, BASE_SPEED);
      break;

    case 0b11100: // Left sensors detect the black line, meaning the car is drifting left and needs to turn right
    case 0b11000:
    case 0b10000:
      right(BASE_SPEED, TURN_SPEED);
      break;

    case 0b00111: // Right sensors detect the black line, meaning the car is drifting right and needs to turn left
    case 0b00011:
    case 0b00001:
      left(BASE_SPEED, TURN_SPEED);
      break;
      
    case 0b00000: // All sensors fail to detect the black line, meaning the car has strayed off the track, stop
      stopMotor();
      break;

    default: // Other complex states, drive straight at low speed to attempt correction
      forward(150, 150);
      break;
  }
  
  // Delay 20 milliseconds to control the loop frequency and prevent oscillation caused by overly rapid reactions
  delay(20);
}