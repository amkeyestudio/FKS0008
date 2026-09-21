#include "AiCam.h"

// Initialize the vision module, specifying serial port pins as 18(RX), 17(TX)
AiCam aiCam(18, 17);

// ==================== Motors ====================

#define MOTOR_AIN 40  // Channel A direction control (left wheel)
#define MOTOR_AEN 41  // Channel A PWM control (left wheel)
#define MOTOR_BIN 38  // Channel B direction control (right wheel)
#define MOTOR_BEN 21  // Channel B PWM control (right wheel)

// ===================================================

// Set motor speed (Note: corrected the error in mapping left/right wheel parameters from the original code)
void setMotor(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward

  analogWrite(MOTOR_AEN, leftSpeed);  // Channel A (left wheel) outputs left wheel speed
  analogWrite(MOTOR_BEN, rightSpeed); // Channel B (right wheel) outputs right wheel speed
}

// Car forward function
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward

  analogWrite(MOTOR_AEN, leftSpeed);  // Channel A (left wheel) outputs left wheel speed
  analogWrite(MOTOR_BEN, rightSpeed); // Channel B (right wheel) outputs right wheel speed
}

// Car turn left function (left wheel backward, right wheel forward)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel backward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward

  analogWrite(MOTOR_AEN, leftSpeed);  // Channel A (left wheel) outputs left wheel speed
  analogWrite(MOTOR_BEN, rightSpeed); // Channel B (right wheel) outputs right wheel speed
}

// Car turn right function (left wheel forward, right wheel backward)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel backward

  analogWrite(MOTOR_AEN, leftSpeed);  // Channel A (left wheel) outputs left wheel speed
  analogWrite(MOTOR_BEN, rightSpeed); // Channel B (right wheel) outputs right wheel speed
}

// Motor stop function
void Motor_Stop() {
  analogWrite(MOTOR_AEN, 0); // Left wheel PWM set to 0
  analogWrite(MOTOR_BEN, 0); // Right wheel PWM set to 0
}

void setup() {
  Serial.begin(115200); // Initialize serial communication, baud rate set to 115200

  // Configure motor control pins as output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
  pinMode(MOTOR_AEN, OUTPUT);
  pinMode(MOTOR_BEN, OUTPUT);

  Motor_Stop(); // Ensure motors are in a stopped state initially
  delay(1000);  // Delay for 1000 milliseconds, waiting for hardware and sensors to stabilize

  // Initialize the Ai Cam vision module
  aiCam.begin();
  // Set the working mode of the vision module to "line" (line-following mode)
  aiCam.setAiCamMode("line");
  delay(500); // Delay for 500 milliseconds, waiting for module configuration to take effect
}


void loop() {
  // Put your main code here to run repeatedly:
  aiCam.readEspSerial();  // Read and process data sent by the TinyVision module through the serial port

  // Get core data for line recognition
  int offsetVal = aiCam.getLineOffset();  // Line left/right offset (negative means left, positive means right)
  int angleVal = aiCam.getLineAngle();    // Line left/right angle (negative means tilted left, positive means tilted right)
  int validVal = aiCam.getLineValid();    // Line status value (5 indicates valid line recognition)

  // Core control logic: perform differential steering control based on data returned by the vision module
  if (angleVal >= -25 && angleVal <= 25 && offsetVal <= 50 && offsetVal >= -50 && validVal == 5) {
    // Condition: angle and offset are both within small normal ranges, and recognition is valid
    // Action: move straight at a uniform speed
    forward(80, 80);
  } else if (angleVal > 25 || offsetVal > 50) {
    // Condition: the line is deflected significantly to the right
    // Action: turn right (left wheel maintains medium speed 80, right wheel high speed 255 to generate rightward torque)
    right(80, 255);
  } else if (angleVal < -25 || offsetVal < -50) {
    // Condition: the line is deflected significantly to the left
    // Action: turn left (left wheel high speed 255, right wheel maintains medium speed 80)
    left(255, 80);
  } else {
    // Condition: valid line not recognized (validVal != 5) or other abnormal state
    // Action: maintain current posture, move forward briefly for 300ms then stop to prevent running off the track
    forward(80, 80);
    delay(300);
    Motor_Stop();
  }
}