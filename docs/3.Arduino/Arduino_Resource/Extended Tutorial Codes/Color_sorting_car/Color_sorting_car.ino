#include "AiCam.h"
#include <ESP32Servo.h>

Servo myServo;  // Create a servo object to control the opening and closing of the football gripper

// Create a TinyVision vision module instance, parameters 18 and 17 correspond to the RX and TX pins of the ESP32 respectively
AiCam aiCam(18, 17);

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Channel A direction control pin (controls left wheel forward/reverse)
#define MOTOR_AEN 41  // Channel A PWM control pin (controls left wheel speed)
#define MOTOR_BIN 38  // Channel B direction control pin (controls right wheel forward/reverse)
#define MOTOR_BEN 21  // Channel B PWM control pin (controls right wheel speed)

// ================= Servo Pin and Status =================
const int SERVO_PIN = 11;  // Servo signal pin, connected to GPIO 11 of the ESP32
int servoAngle = 140;      // Initial servo angle, at 140 degrees the football gripper is open

// ================= Motor Control Functions =================

// Set the PWM speed of the left and right motors, parameter range 0-255
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);   // Write PWM duty cycle value for the left wheel
  ledcWrite(MOTOR_BEN, rightSpeed);  // Write PWM duty cycle value for the right wheel
}

// Forward function: both left and right wheels rotate forward simultaneously
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Left wheel direction pin set to HIGH, forward
  digitalWrite(MOTOR_BIN, HIGH);  // Right wheel direction pin set to HIGH, forward
  setMotor(leftSpeed, rightSpeed); // Set speed
}

// Backward function: both left and right wheels rotate in reverse simultaneously
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel direction pin set to LOW, reverse
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel direction pin set to LOW, reverse
  setMotor(leftSpeed, rightSpeed);
}

// Turn left function: using differential speed principles, left wheel stops, right wheel moves forward, turning left in place
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);   // Left wheel stops
  digitalWrite(MOTOR_BIN, HIGH);  // Right wheel moves forward
  setMotor(leftSpeed, rightSpeed);
}

// Turn right function: left wheel moves forward, right wheel stops, turning right in place
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Left wheel moves forward
  digitalWrite(MOTOR_BIN, LOW);   // Right wheel stops
  setMotor(leftSpeed, rightSpeed);
}

// Stop motor function: set PWM duty cycle to 0
void stop_Motor() {
  setMotor(0, 0);  // Both left and right wheel speeds set to 0
}

// ================= Initialization Setup =================
void setup() {
  // Initialize serial communication, baud rate set to 115200 for debugging printouts
  Serial.begin(115200);

  // Initialize AiCam vision module, establish serial communication
  aiCam.begin();
  // Set the working mode of the vision module to "color" (color recognition mode)
  aiCam.setAiCamMode("color");

  // Set motor direction control pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channel (Note: This uses the new syntax of ESP32 Arduino Core 3.x)
  // Parameter description: pin number, PWM frequency (1000Hz is suitable for DC motors), resolution (8-bit, meaning duty cycle range 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Ensure motors are stopped in the initial state
  stop_Motor();

  // Initialize servo, bind signal pin
  myServo.attach(SERVO_PIN);
  // Set the initial angle of the servo to 140 degrees to keep the football gripper open and ready for gripping
  myServo.write(servoAngle);  
}

// ================= Main Loop =================
void loop() {
  // Continuously read and process data sent via serial from the TinyVision module
  aiCam.readEspSerial();  

  // Get the recognized color category string
  String strColor = aiCam.getColor();

  // Check if a red or yellow target is recognized
  if (strColor == "RED" || strColor == "YELLOW") {
    // 1. Close the football gripper to grip the item (90 degrees is the clamped state)
    myServo.write(90);
    delay(300); // Wait for the servo action to complete
    
    // 2. Turn according to the color, preparing to head towards the target area
    if (strColor == "RED") {
      left(220, 220);  // Red item, turn left
    } else {
      right(220, 220); // Yellow item, turn right
    }
    delay(300); // Turn duration
    
    // 3. Move forward to transport the item to the target area
    forward(220, 220);
    delay(300); // Forward duration
    
    // 4. Arrive at the target area, stop motors
    stop_Motor();
    delay(300); 
    
    // 5. Open the football gripper to release the item
    myServo.write(140); // Restore to 140 degrees open state
    delay(200); 
    
    // 6. Move backward to leave the target area
    back(220, 220);
    delay(300); 
    
    // 7. Reverse turn to align the car body straight again
    if (strColor == "RED") {
      right(220, 220); // Previously turned left, now turn right to straighten
    } else {
      left(220, 220);  // Previously turned right, now turn left to straighten
    }
    delay(300); // Straightening duration
    
    // 8. Action completed, stop motors, wait for the next recognition
    stop_Motor();
  } else {
    // Target color not recognized, keep motors stopped
    stop_Motor();
  }
}

/* 
 * Note: The following is a repeatedly appearing code segment from the original text, retained for content completeness.
 * During actual project compilation, please delete the duplicate code below to avoid "redefinition" compilation errors.
 */
#include "AiCam.h"
#include <ESP32Servo.h>

Servo myServo;  // Create servo object

// Create instance
AiCam aiCam(18, 17);

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Channel A direction control (left wheel)
#define MOTOR_AEN 41  // Channel A PWM control (left wheel speed)
#define MOTOR_BIN 38  // Channel B direction control (right wheel)
#define MOTOR_BEN 21  // Channel B PWM control (right wheel speed)

// ================= Servo Pin and Status =================
const int SERVO_PIN = 11;  // Servo signal pin
int servoAngle = 140;      // Initial servo angle

// ================= Motor Control Functions =================

// Set left and right motor PWM speeds (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);   // Write left wheel PWM value
  ledcWrite(MOTOR_BEN, rightSpeed);  // Write right wheel PWM value
}

// Forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH);  // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse
  setMotor(leftSpeed, rightSpeed);
}

// Turn left (left wheel stops, right wheel moves forward)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);   // Left wheel stops
  digitalWrite(MOTOR_BIN, HIGH);  // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Turn right (left wheel moves forward, right wheel stops)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Left wheel forward
  digitalWrite(MOTOR_BIN, LOW);   // Right wheel stops
  setMotor(leftSpeed, rightSpeed);
}

// Stop motor
void stop_Motor() {
  setMotor(0, 0);  // Set speed to 0
}

// ================= Initialization Setup =================
void setup() {
  // Initialize serial, baud rate 115200
  Serial.begin(115200);

  // Initialize Ai Cam module
  aiCam.begin();
  aiCam.setAiCamMode("color");

  // Set motor direction pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channel (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  stop_Motor();

  // Initialize servo
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle);  // Set initial angle
}

void loop() {
  aiCam.readEspSerial();  // Read and process data sent from ESP

  // Get color recognition value
  String strColor = aiCam.getColor();

  if (strColor == "RED" || strColor == "YELLOW") {
    myServo.write(90);
    delay(300);
    if (strColor == "RED") {
      left(220, 220);
    } else {
      right(220, 220);
    }
    delay(300);
    forward(220, 220);
    delay(300);
    stop_Motor();
    delay(300);
    myServo.write(140);
    delay(200);
    back(220, 220);
    delay(300);
    if (strColor == "RED") {
      right(220, 220);
    } else {
      left(220, 220);
    }
    delay(300);
    stop_Motor();
  } else {
    stop_Motor();
  }
}