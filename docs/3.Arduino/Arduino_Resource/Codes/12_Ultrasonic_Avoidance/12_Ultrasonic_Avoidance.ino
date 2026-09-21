#include <ESP32Servo.h>  // Include the ESP32 servo control library

Servo myServo;  // Create a servo object named myServo

// ================= Ultrasonic Pin Definitions =================
const int trigPin = 13;  // Trigger pin connected to IO13
const int echoPin = 12;  // Echo pin connected to IO12

// Define the pin connected to the servo signal line as GPIO42
const int servoPin = 42;

// ================= Distance Threshold Setting =================
// Define the danger distance lower limit as 15 cm. Below this distance, the servo rotates 0-180 degrees to measure left and right distances.
const long closeDistance = 15;

long leftDistance = 0;   // Left distance measurement value
long rightDistance = 0;  // Right distance measurement value

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Motor A direction control (left wheel)
#define MOTOR_AEN 41  // Motor A PWM control (left wheel speed)
#define MOTOR_BIN 38  // Motor B direction control (right wheel)
#define MOTOR_BEN 21  // Motor B PWM control (right wheel speed)

// ================= Initialization Function =================
void setup() {
  // Initialize serial communication with a baud rate of 115200 for faster and smoother data display
  Serial.begin(115200);
  Serial.println("System initialization complete, ultrasonic obstacle avoidance car started!");

  // Set ultrasonic pin modes
  pinMode(trigPin, OUTPUT);  // Trig pin as output to send trigger signals
  pinMode(echoPin, INPUT);   // Echo pin as input to receive echo signals

  // Set motor direction pins as output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Ensure motors are stopped initially
  stopMotor();

  // Allocate a PWM timer for ESP32 (ESP32 specific operation to avoid conflicts with motor PWM)
  ESP32PWM::allocateTimer(0);
  // Set the standard frequency for servo control signals to 50Hz
  myServo.setPeriodHertz(50);
  // Attach the servo to the specified pin and set the minimum pulse width to 500 microseconds and maximum to 2400 microseconds
  // Note: SG90 micro servos typically use 500us and 2400us;
  // For larger servos like MG995 which default to 1000us and 2000us, you can write myServo.attach(servoPin); directly
  myServo.attach(servoPin, 500, 2400);

  // During initialization, return the servo to the center position of 90 degrees to prevent sudden violent movements
  myServo.write(90);
  delay(1000);  // Wait for 1000 milliseconds (1 second) for the servo to stabilize
}

// ================= Main Loop Function =================
void loop() {
  // 1. Get the front distance
  long distance = getDistance();  // Call the ranging function and store the calculated distance in the distance variable

  // 2. Print the distance in the serial monitor for debugging and observation
  Serial.print("Current front distance: ");  // Print prompt text
  Serial.print(distance);          // Print the specific distance number
  Serial.println(" cm");           // Print unit " cm" (centimeters) and newline

  // 3. Control car actions based on distance (core logic decision)
  if (distance <= closeDistance) {
    // If the measured distance is less than or equal to 15 cm, it means there is an obstacle ahead
    stopMotor();  // Stop moving forward if distance is less than 15 cm
    // Servo rotates the ultrasonic sensor to 180 degrees (left side)
    myServo.write(180);
    delay(300);                    // Delay 300ms for the servo to rotate into place
    leftDistance = getDistance();  // Measure left distance
    delay(100);                    // Delay 100ms to eliminate signal jitter

    // Servo rotates the ultrasonic sensor to 0 degrees (right side)
    myServo.write(0);
    delay(300);                     // Delay 300ms for the servo to rotate into place
    rightDistance = getDistance();  // Measure right distance
    delay(100);                     // Delay 100ms to eliminate signal jitter

    // Return the servo to 90 degrees (straight ahead) after ranging
    myServo.write(90);

    // Compare left and right distances to decide steering direction
    if (leftDistance > rightDistance) {
      // Left distance is further, turn left
      left(200, 200);
    } else {
      // Right distance is further (or equal), turn right
      right(200, 200);
    }
    delay(500);

  } else {
    // Front distance is greater than 15 cm, safe, continue moving straight
    forward(150, 150);
  }

  delay(100);  // Brief delay in main loop to prevent executing too fast
}

// ================= Ultrasonic Ranging Function =================
/*
 * Function Purpose: Encapsulate ultrasonic ranging code into a function to simplify the main loop code
 * Function Name: getDistance
 * Function Feature: Returns an integer distance value (unit: centimeters)
*/
int getDistance() {
  // Define variables to store time and distance
  long duration;  // Round-trip time of the sound wave (microseconds)
  int distance;   // Calculated distance (centimeters)

  // Step 1: Ensure Trig pin is low, preparing for transmission
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // Wait 2 microseconds to ensure signal stability

  // Step 2: Give the Trig pin a 10 microsecond high pulse to trigger the sensor to emit ultrasonic waves
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Step 3: Read the high-level duration of the Echo pin
  // The pulseIn function waits for the pin to go high, then times until the pin goes low
  duration = pulseIn(echoPin, HIGH);

  // Step 4: Calculate distance
  // The speed of sound in air is approximately 0.034 cm/microsecond
  // Distance = (Time × Speed) / 2, because the sound makes a round trip
  distance = duration * 0.034 / 2;

  // Limit the ultrasonic ranging range because values beyond 3 meters or below 2 centimeters are inaccurate
  if (distance < 2 || distance > 300) {
    distance = 0;  // Out of range data is treated as invalid, returning 0
  }

  return distance;
}

// ================= Motor Control Functions =================

// Set PWM speed for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write left wheel PWM value
  ledcWrite(MOTOR_BEN, rightSpeed); // Write right wheel PWM value
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