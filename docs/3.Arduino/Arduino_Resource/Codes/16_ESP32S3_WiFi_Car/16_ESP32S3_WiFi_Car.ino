#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "webpage.h" // Include external webpage HTML code file

// ================= WiFi Configuration =================
const char* ssid = "KS0567";         // Replace with your WiFi name
const char* password = "88888888";   // Replace with your WiFi password

// ================= Global Objects =================
WebServer server(80); // Create Web server instance, listening on port 80
Servo myServo;        // Create servo object

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Motor A direction control (left wheel)
#define MOTOR_AEN 41  // Motor A PWM control (left wheel speed)
#define MOTOR_BIN 38  // Motor B direction control (right wheel)
#define MOTOR_BEN 21  // Motor B PWM control (right wheel speed)

// ================= Ultrasonic Pin Definitions =================
const int TRIG_PIN = 12; // Ultrasonic trigger pin
const int ECHO_PIN = 13; // Ultrasonic echo pin

// ================= Servo Pin and State =================
const int SERVO_PIN = 42; // Servo signal pin
int servoAngle = 90;      // Initial servo angle

// ================= Motor Control Functions =================

// Set PWM speeds for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write left wheel PWM value
  ledcWrite(MOTOR_BEN, rightSpeed); // Write right wheel PWM value
}

// Forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse
  setMotor(leftSpeed, rightSpeed);
}

// Turn left (left wheel stop, right wheel forward)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel stop
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Turn right (left wheel forward, right wheel stop)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel stop
  setMotor(leftSpeed, rightSpeed);
}

// Stop motors
void stop_Motor() {
  setMotor(0, 0); // Set speed to 0
}

// ================= Sensor and Peripheral Functions =================

// Read ultrasonic distance (unit: cm)
long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // Send 10-microsecond high pulse
  digitalWrite(TRIG_PIN, LOW);
  
  // Read high level duration, timeout set to 30000 microseconds
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  // Distance = (time * speed of sound) / 2, speed of sound is approximately 0.034 cm/us
  return duration * 0.034 / 2; 
}

// ================= Web Server Request Handling =================

// Handle control commands
void handleCmd() {
  // Get the "move" value from URL parameters
  String moveCommand = server.arg("move");

  // Execute corresponding action based on command
  if (moveCommand == "forward") {
    forward(200, 200);
  } 
  else if (moveCommand == "backward") {
    back(200, 200);
  } 
  else if (moveCommand == "left") {
    left(200, 200);
  } 
  else if (moveCommand == "right") {
    right(200, 200);
  } 
  else if (moveCommand == "stop") {
    stop_Motor();
    myServo.write(90); // Center servo when stopped
  } 
  else if (moveCommand == "servo_plus") {
    // Increase servo angle (simplified here to go directly to max angle)
    // servoAngle = min(180, servoAngle + 5);
    // myServo.write(servoAngle);
    myServo.write(180);
  } 
  else if (moveCommand == "servo_minus") {
    // Decrease servo angle (simplified here to go directly to min angle)
    // servoAngle = max(0, servoAngle - 5);
    // myServo.write(servoAngle);
    myServo.write(0);
  } 
  else if (moveCommand == "claw_open") {
    myServo.write(0);   // Open mechanical claw
    servoAngle = 0;
  } 
  else if (moveCommand == "claw_close") {
    myServo.write(180); // Close mechanical claw
    servoAngle = 90;
  }

  // Return success response to client
  server.send(200, "text/plain", "OK");
}

// ================= Initialization Setup =================
void setup() {
  // Initialize serial monitor, baud rate 115200
  Serial.begin(115200);

  // Set motor direction pins as output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Set ultrasonic pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize servo
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle); // Set initial angle

  // Start connecting to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // Wait for WiFi connection to succeed
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print assigned IP address

  // Configure Web server routes
  // When accessing root path "/", send webpage HTML content
  server.on("/", []() {
    server.send_P(200, "text/html; charset=UTF-8", index_html);
  });
  
  // When accessing "/cmd" path, call handleCmd function to process commands
  server.on("/cmd", handleCmd);
  
  // When accessing "/distance" path, return ultrasonic ranging data
  server.on("/distance", []() {
    server.send(200, "text/plain", String(readDistance()));
  });

  // Start Web server
  server.begin();
  Serial.println("Web server started!");
}

// ================= Main Loop =================
void loop() {
  // Continuously handle client requests
  server.handleClient();
}