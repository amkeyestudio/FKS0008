#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "webpage.h" // Include external webpage HTML code file

// ================= WiFi Configuration =================
const char* ssid = "FKS0008";         // Replace with your WiFi name
const char* password = "88888888";   // Replace with your WiFi password

// ================= Global Objects =================
WebServer server(80); // Create Web server instance, listening on port 80
Servo myServo;        // Create servo object

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Motor A direction control (left wheel)
#define MOTOR_AEN 41  // Motor A PWM control (left wheel speed)
#define MOTOR_BIN 38  // Motor B direction control (right wheel)
#define MOTOR_BEN 21  // Motor B PWM control (right wheel speed)

// ================= Servo Pin and State =================
const int SERVO_PIN = 11; // Servo signal pin
int servoAngle = 140;      // Initial servo angle (open state)

// ================= Motor Control Functions =================

// Set PWM speed for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write left wheel PWM value
  ledcWrite(MOTOR_BEN, rightSpeed); // Write right wheel PWM value
}

// Move forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Move backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse
  setMotor(leftSpeed, rightSpeed);
}

// Turn left (left wheel stops, right wheel moves forward)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel stop
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Turn right (left wheel moves forward, right wheel stops)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel stop
  setMotor(leftSpeed, rightSpeed);
}

// Stop motors
void stop_Motor() {
  setMotor(0, 0); // Set speed to 0
}


// ================= Web Server Request Handling =================

// Handle control commands
void handleCmd() {
  // Get the "move" value from URL parameters
  String moveCommand = server.arg("move");

  // Execute corresponding actions based on commands
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
  } 
  else if (moveCommand == "claw_open") {
    myServo.write(140);   // Mechanical claw opens (prepare to catch ball or reset)
  } 
  else if (moveCommand == "claw_close") {
    myServo.write(90);    // Mechanical claw closes (squeeze soccer ball, trigger shooting action)
  }

  // Return success response to client
  server.send(200, "text/plain", "OK");
}

// ================= Initialization Setup =================
void setup() {
  // Initialize serial monitor with baud rate 115200
  Serial.begin(115200);

  // Set motor direction pins as output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Initialize servo
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle); // Set initial angle to ensure claw is open upon power-on

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Print assigned IP address for webpage access

  // Configure Web server routes
  // When accessing root path "/", send webpage HTML content
  server.on("/", []() {
    server.send_P(200, "text/html; charset=UTF-8", index_html);
  });
  
  // When accessing "/cmd" path, call handleCmd function to process commands
  server.on("/cmd", handleCmd);
  
  // When accessing "/distance" path, return ultrasonic ranging data (reserved interface here, returns 0 by default)
  server.on("/distance", []() {
    server.send(200, "text/plain", String(0));
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