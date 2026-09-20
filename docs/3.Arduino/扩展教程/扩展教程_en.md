# Extended Function Tutorials

## 1. Color Sorting Car

### 1.1 Introduction

This project aims to build an intelligent color sorting car by utilizing the powerful color recognition capabilities of the TinyVision module, combined with the football gripper mechanical structure of the car. The system can capture and recognize specific color targets within its field of view in real time, autonomously plan its movement trajectory based on the recognition results, and accurately transport items of different colors to designated locations: red items are sorted to the left area, and yellow items are sorted to the right area.

---

### 1.2 Required Items

Before starting the experiment, please check and prepare the following hardware materials. Reasonable selection is the basis for the success of the project.

| Name | Specification/Model | Quantity | Remarks |
| :--- | :--- | :--- | :--- |
| Car | ESP32S3 4WD Car | 1 | Includes chassis, motors, battery, and main control board |
| TinyVision | AI Vision Module | 1 | The color recognition model needs to be pre-configured in the TinyVision PC software |
| Football Gripper | 4WD Car Football Gripper | 1 | Can be used to launch footballs or grip items, must be installed at the front of the car |
| LEGO (Self-provided) | 5x7 LEGO pieces | 1 | Used to build baffles or markers for the sorting target area |
| Red & Yellow Items (Self-provided) | Any material | 1 | Used for AI vision color recognition; size must ensure the car gripper can securely hold them |

---

### 1.3 Code Explanation

```cpp
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
```

### 1.4 Experimental Verification

1. **Power-on Initialization**: After the car is powered on, the servo rotates to 140 degrees, and the football gripper fully opens. The car remains stationary in place, and the TinyVision module camera indicator light turns on.
2. **Identifying Red Objects**: When a red object is placed directly in front of the camera, the car's football gripper rapidly closes (90 degrees) to clamp the object; then the car rotates in place to the left by about 90 degrees; next, it drives straight forward for a certain distance; upon reaching the designated position, it stops, and the football gripper opens to release the object; subsequently, the car reverses back to near the origin, rotates 90 degrees to the right to straighten the chassis, and finally stops and waits.
3. **Identifying Yellow Objects**: The action sequence is completely identical to that for red objects, but the turning direction is reversed (turns right first, then turns left back to center after releasing).
4. **No Target or Wrong Color**: The car remains stationary, and the football gripper stays in the open state.



## 2. Vision Line-Following Car

### 2.1 Introduction

The vision line-following car is a classic application project combining artificial intelligence and robotics technology.

In this project, the **TinyVision module** acts as the "eyes" of the car, responsible for collecting track images in real time and performing AI vision algorithm processing to extract the center offset and angle information of the line; the **ESP32S3 main control board** serves as the "brain" and "nerve center" of the car. After receiving the data from the vision module, it calculates the target speeds for the left and right wheels through motion control algorithms, thereby driving the **4WD car** to achieve smooth and precise automatic line following.

### 2.2 Required Items

Before starting the experiment, make sure you have prepared the following hardware items:

| Name | Specification/Model | Qty | Remarks |
| :--- | :--- | :---: | :--- |
| Car | ESP32S3 4WD Car | 1 | Includes chassis, 4 DC gear motors, ESP32S3 main control board, and motor driver module |
| TinyVision | AI Vision Module | 1 | Built-in AI computing power, supports multiple vision algorithms, communicates with the main controller via UART |
| Car Track | Included in the ESP32S3 4WD Car kit | 1 | Usually a black background with white lines or white background with black lines track, used to provide the line-following trajectory |

### 2.3 Code Explanation

```c
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
```

### 2.4 Experimental Verification

- **Instant of Power-on**: The car will remain stationary for about 1.5 seconds (the `delay` in the code), during which the serial monitor begins outputting `Offset`, `Angle`, and `Valid` data.
- **Straight Driving**: When `Valid` is 5, and `Offset` and `Angle` are close to 0, the car moves straight forward at a medium speed (PWM=80).
- **Corner Handling**: When the car approaches a curve, `Angle` or `Offset` will exceed the threshold (e.g., >25), and the car will automatically trigger the `right` or `left` function, smoothly negotiating the curve through the speed difference between the left and right wheels.
- **Line Loss**: If the car accidentally deviates from the track causing `Valid` to not equal 5, the car will move forward briefly for 0.3 seconds and then come to a complete stop, preventing it from completely running off the track.

## 3. Soccer Smart Car

### 3.1 Introduction

This project aims to build an intelligent soccer-playing smart car with a goal-shooting function. By using the smart car soccer clamp expansion kit and installing a special launching component onto the soccer clamp, you can control the closing of the servo through code, utilizing the potential energy from mechanical structure compression to instantly eject the soccer ball (launching action), perfectly simulating the effect of scoring a goal in soccer.

This smart car not only possesses basic omnidirectional movement capabilities (moving forward, backward, turning left, and turning right) but also integrates remote Web control functions. Users can access the LAN webpage hosted by the smart car using a mobile phone or computer browser, remotely control the car's movement on the field in real-time, and trigger the "shoot" command at the optimal moment. This project integrates multiple core embedded technologies such as motor drive, servo control, WiFi communication, and Web front-end interaction, making it an excellent practical project for learning robot control and IoT applications.

### 3.2 Required Items

Before starting assembly and programming, please ensure you have prepared the following hardware and materials:

| Name | Specification/Model | Quantity | Remarks |
| :--- | :--- | :--- | :--- |
| Smart Car | ESP32S3 4WD Smart Car | 1 | Core control platform, comes with ESP32-S3 main control |
| Soccer Clamp | 4WD Smart Car Soccer Clamp | 1 | Can be used to launch soccer balls or grip objects, includes servo and linkage mechanism |
| Soccer Ball | Mini Soccer Ball | 1 | Included in the soccer clamp expansion kit, size must fit the clamp opening |

---

### 3.3 Code Explanation

<span style="color:red;font-size:25px">Please note that the "webpage.h" library file is included in the code files we provide. Do NOT just copy and upload this code directly without it, or it will throw an error stating that the "webpage.h" library does not exist!!</span>

```c
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
```

### 3.4 Experimental Verification

1. Compile and upload the code to the ESP32S3 car.
2. Open the Serial Monitor (set the baud rate to 115200).
3. Power on the car and observe the serial output.
4. Use a phone or computer to connect to the Wi-Fi named "FKS0008".
5. Open a browser and enter the IP address printed on the serial monitor (e.g., `192.168.4.1` or the IP assigned by the router).
6. Click the servo open and servo close buttons on the web page to observe the car's movements.



## 4. Video Smart Car

### 4.1 Introduction

This project aims to use TinyVision's Wi-Fi communication capabilities and camera module to build a Wi-Fi video smart car with real-time image transmission. The system adopts a dual-core architecture: TinyVision acts as the "brain" and "eyes", responsible for camera video stream capture, Wi-Fi network establishment, and Web server setup; the ESP32S3 development board acts as the "cerebellum" and "limbs", responsible for underlying motor driving and motion control.

TinyVision connects to the Wi-Fi hotspot it creates via a mobile phone or tablet. Users can view real-time video footage in the browser and send control commands through virtual buttons on the web page. After receiving the commands, TinyVision sends the data for the car's forward, backward, left turn, right turn, and 4 other custom function buttons to the ESP32S3 development board via the UART serial port. The ESP32S3 parses the commands and drives the motors, thereby achieving smooth remote control of the Wi-Fi video smart car.

### 4.2 Required Items

| Name | Specification/Model | Quantity | Remarks |
| :--- | :--- | :--- | :--- |
| Smart Car | ESP32S3 4WD Smart Car | 1 | |
| TinyVision | AI Vision Module | 1 | |
| Phone or Tablet | | 1 | Used for Wi-Fi car control |

### 4.3 ESP32S3 Development Board Code

The following is the underlying motion control code for the ESP32S3 development board. This code is responsible for initializing motor pins, configuring PWM, and looping to listen for serial data to execute corresponding actions.

```c
// Define the serial pins for communication with TinyVision
#define ESP_RX_PIN 18  // ESP32S3 receive pin, connected to TinyVision's TX
#define ESP_TX_PIN 17  // ESP32S3 transmit pin, connected to TinyVision's RX

// ================= Pin Definitions =================
// Define motor control pins connected to the motor driver module
#define MOTOR_AIN 40  // Channel A direction control (left wheel) - controls forward/reverse of the left wheel
#define MOTOR_AEN 41  // Channel A PWM control (left wheel) - controls the speed of the left wheel
#define MOTOR_BIN 38  // Channel B direction control (right wheel) - controls forward/reverse of the right wheel
#define MOTOR_BEN 21  // Channel B PWM control (right wheel) - controls the speed of the right wheel

// ================= Motor Control Functions =================

// Set PWM speeds for left and right motors (0-255)
// Parameters: leftSpeed for left wheel, rightSpeed for right wheel
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);   // Write left wheel PWM value; duty cycle determines speed
  ledcWrite(MOTOR_BEN, rightSpeed);  // Write right wheel PWM value; duty cycle determines speed
}

// Forward function
// Parameters: leftSpeed for left wheel, rightSpeed for right wheel
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Set left wheel direction pin HIGH for forward rotation
  digitalWrite(MOTOR_BIN, HIGH);  // Set right wheel direction pin HIGH for forward rotation
  setMotor(leftSpeed, rightSpeed); // Set speeds for both sides
}

// Backward function
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Set left wheel direction pin LOW for reverse rotation
  digitalWrite(MOTOR_BIN, LOW);  // Set right wheel direction pin LOW for reverse rotation
  setMotor(leftSpeed, rightSpeed);
}

// Left turn function (left wheel stops, right wheel moves forward, achieving differential steering)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);   // Left wheel stops (or can be changed to reverse for in-place left turn if needed)
  digitalWrite(MOTOR_BIN, HIGH);  // Right wheel rotates forward
  setMotor(leftSpeed, rightSpeed); // Here leftSpeed actually passes 0 or a smaller value
}

// Right turn function (left wheel moves forward, right wheel stops)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Left wheel rotates forward
  digitalWrite(MOTOR_BIN, LOW);   // Right wheel stops
  setMotor(leftSpeed, rightSpeed);
}

// Stop motor function
void stopMotor() {
  setMotor(0, 0);  // Set both left and right wheel PWM speeds to 0 to stop the motors
}

void setup() {
  // Initialize hardware serial port Serial1 for communication with TinyVision
  // Parameters: baud rate 9600, data format 8 data bits, no parity, 1 stop bit, RX pin, TX pin
  Serial1.begin(9600, SERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN);
  
  // Initialize USB virtual serial port for connecting to computer to view debug info
  Serial.begin(115200);  
  Serial.println("UNO ready"); // Print startup prompt

  // Set motor direction control pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (Note: This uses the new syntax of ESP32 Arduino Core 3.x)
  // Parameters: pin number, PWM frequency (1000Hz suitable for motor control), resolution (8-bit, value range 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Initial state: Ensure motors are stopped upon power-up to prevent the car from running away wildly
  stopMotor();
  Serial.println("Initialization complete, starting execution!");
}

void loop() {
  // Check if data has arrived in the Serial1 serial buffer
  if (Serial1.available()) {
    // Read a single byte of data (i.e., the control command character)
    char cmd = Serial1.read();

    // Print the received command to the USB debug serial port for easy troubleshooting
    Serial.print("Receive: ");
    Serial.println(cmd);

    // Use switch-case structure to parse commands and execute corresponding actions
    switch (cmd) {
      case 'F':
        // Received 'F', motors move forward
        {
          Serial.println("MOTOR FORWARD");
          forward(200, 200); // Set left and right wheel speeds to 200 (max 255) to prevent abrupt startup
        }
        break;

      case 'B':
        // Received 'B', motors move backward
        {
          Serial.println("MOTOR BACKWARD");
          back(200, 200);
        }
        break;

      case 'L':
        // Received 'L', turn left
        {
          Serial.println("MOTOR LEFT");
          left(200, 200); 
        }
        break;

      case 'R':
        // Received 'R', turn right
        {
          Serial.println("MOTOR RIGHT");
          right(200, 200);
        }
        break;

      case 'S':
        // Received 'S', stop
        {
          Serial.println("MOTOR STOP");
          stopMotor();
        }
        break;

      case 'O':
        // Received 'O', servo open (reserved function)
        Serial.println("SERVO OPEN");
        break;

      case 'C':
        // Received 'C', servo close (reserved function)
        Serial.println("SERVO CLOSE");
        break;

      case '1':
        // Received '1', extension function 1 (reserved function)
        Serial.println("EXT1");
        break;

      case '2':
        // Received '2', extension function 2 (reserved function)
        Serial.println("EXT2");
        break;
        
      default:
        // If an unknown character is received, handling logic can be added here
        break;
    }
  }
  // Short delay of 20ms to reduce CPU usage and act as software debounce for serial reception
  delay(20);
}
```

### 4.4 TinyVision Code

<span style="color:red;font-size:25px">Please note that the "index_html.h" library file is included in the code files we provide. Do not simply copy and paste this code without it, or it will throw an error stating that the "index_html.h" library does not exist!</span>

Below is the core code for the TinyVision module. This code is responsible for camera initialization, Wi-Fi connection, web server setup, video streaming, and TFT screen status display.

```c
// generated by KidsBlock
#include <Arduino.h>
#include "esp_camera.h"       // ESP32 camera driver library
#include "img_converters.h"   // Image format conversion library (used for RGB565 to JPEG)
#include <WiFi.h>             // Wi-Fi function library
#include <AsyncTCP.h>         // Asynchronous TCP library (underlying dependency for AsyncWebServer)
#include <ESPAsyncWebServer.h>// Asynchronous Web Server library, handles high concurrency without blocking
#include "index_html.h"       // Web frontend code (HTML/CSS/JS), included from an external file
#include <SPI.h>              // SPI communication library
#include <Adafruit_GFX.h>     // TFT screen graphics base library
#include <Adafruit_ST7789.h>  // ST7789 screen driver library

// ================= Camera Pin Definitions (for TinyVision Hardware) =================
#define PWDN_GPIO_NUM -1      // Power down pin, -1 means not used
#define RESET_GPIO_NUM -1     // Reset pin, -1 means not used
#define XCLK_GPIO_NUM 15      // External clock pin
#define SIOD_GPIO_NUM 4       // SCCB data line (I2C SDA)
#define SIOC_GPIO_NUM 5       // SCCB clock line (I2C SCL)
#define Y2_GPIO_NUM 11        // Data pins D0-D7
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16
#define VSYNC_GPIO_NUM 6      // Vertical synchronization pin
#define HREF_GPIO_NUM 7       // Horizontal reference pin
#define PCLK_GPIO_NUM 13      // Pixel clock pin

// Camera initialization function
void setupCamera() {
  camera_config_t config = {}; // Create configuration struct and zero it out
  
  // Configure LEDC channel and timer to generate XCLK clock
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  
  // Map data pins D0-D7
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  
  // Map control pins
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  
  // Set XCLK frequency to 20MHz
  config.xclk_freq_hz = 20000000;
  
  // Set pixel format to RGB565 (will be converted to JPEG later)
  config.pixel_format = PIXFORMAT_RGB565;
  
  // Set resolution: QVGA (320x240), suitable for web transmission, balancing clarity and bandwidth
  config.frame_size = FRAMESIZE_QVGA;
  
  // Set frame buffer count to 1 to save memory
  config.fb_count = 1;
  // Frame buffer allocated in internal DRAM
  config.fb_location = CAMERA_FB_IN_DRAM;
  // Grab image when buffer is empty
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

  // Execute camera initialization
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    // If initialization fails, print error code and enter infinite loop
    Serial.printf("Camera init failed: 0x%x\n", err);
    while (true) delay(1000);
  }

  // Get camera sensor object for subsequent configuration
  sensor_t *s = esp_camera_sensor_get();
  if (s) {
    Serial.printf("Sensor PID: 0x%04X\n", s->id.PID); // Print sensor ID
    s->set_vflip(s, 0); // Set vertical flip, 0 for no flip (adjust based on camera mounting orientation)
  }
  Serial.println("Camera init OK");
}

// ================= Serial Communication Definitions =================
#define UNO_TX_PIN 20       // TinyVision transmit pin, connects to ESP32S3's RX
#define UNO_RX_PIN 19       // TinyVision receive pin, connects to ESP32S3's TX
#define UNO_BAUD 9600       // Serial baud rate

// Send control commands to ESP32S3 (UNO)
void sendToUNO(const String &cmd) {
  Serial.printf("UNO TX -> %s\n", cmd.c_str()); // Debug print
  Serial1.println(cmd);                         // Send via hardware serial
}

// ================= Wi-Fi Configuration =================
const char *ssid = "FKS0008";         // Wi-Fi hotspot name
const char *password = "88888888";    // Wi-Fi password

// Create asynchronous Web Server instance, listening on port 80
AsyncWebServer server(80);
// Define MJPEG video stream boundary string to separate each JPEG frame
static const char *BOUNDARY = "123456789000000000000987654321";

// Video stream state management struct
struct StreamState {
  uint8_t *jpg = nullptr;   // JPEG image data pointer
  size_t jpgLen = 0;        // JPEG data length
  size_t sent = 0;          // Number of bytes sent
  char header[128];         // HTTP response header buffer
  size_t headerLen = 0;     // Response header length
  size_t headerSent = 0;    // Number of response header bytes sent

  // Destructor to ensure memory is released
  ~StreamState() { release(); }

  // Release memory for the current frame
  void release() {
    if (jpg) free(jpg);
    jpg = nullptr;
    jpgLen = sent = 0;
    headerLen = headerSent = 0;
  }

  // Grab a frame of image and convert to JPEG
  bool capture() {
    camera_fb_t *fb = esp_camera_fb_get(); // Get a frame of raw data from camera
    if (!fb) return false;

    if (fb->format == PIXFORMAT_JPEG) {
      // If camera outputs JPEG directly (current config is RGB565, won't enter this branch)
      jpg = (uint8_t *)malloc(fb->len);
      if (!jpg) {
        esp_camera_fb_return(fb);
        return false;
      }
      memcpy(jpg, fb->buf, fb->len);
      jpgLen = fb->len;
    } else {
      // Convert RGB565 to JPEG, parameter 80 indicates JPEG compression quality (1-100)
      if (!frame2jpg(fb, 80, &jpg, &jpgLen)) {
        esp_camera_fb_return(fb);
        return false;
      }
    }
    esp_camera_fb_return(fb); // Return frame buffer to driver

    // Construct HTTP boundary header for MJPEG stream
    headerLen = snprintf(header, sizeof(header),
                         "\r\n--%s\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
                         BOUNDARY, (unsigned)jpgLen);
    return true;
  }

  // Fill data chunks into network buffer (core flow control logic)
  size_t fill(uint8_t *buffer, size_t maxLen) {
    // If there is currently no image data, try to capture a frame
    if (jpg == nullptr && !capture()) return RESPONSE_TRY_AGAIN;

    size_t written = 0;
    // 1. Send HTTP response header first
    if (headerSent < headerLen) {
      size_t n = min(maxLen, headerLen - headerSent);
      memcpy(buffer, header + headerSent, n);
      headerSent += n;
      written += n;
    }
    // 2. Send JPEG image data
    if (written < maxLen && sent < jpgLen) {
      size_t n = min(maxLen - written, jpgLen - sent);
      memcpy(buffer + written, jpg + sent, n);
      sent += n;
      written += n;
    }
    // 3. If current frame is completely sent, release memory to prepare for the next frame
    if (sent >= jpgLen && headerSent >= headerLen) release();
    return written;
  }
};

// Route callback for handling video stream requests
void handleStream(AsyncWebServerRequest *request) {
  Serial.println("Stream client connected");
  StreamState *st = new StreamState(); // Create state instance for each connected client

  char contentType[80];
  // Set Content-Type to multipart/x-mixed-replace, which is the standard format for MJPEG streams
  snprintf(contentType, sizeof(contentType),
           "multipart/x-mixed-replace;boundary=%s", BOUNDARY);

  // Use Chunked Response to avoid loading the entire image into memory at once
  AsyncWebServerResponse *response = request->beginChunkedResponse(
    contentType,
    [st](uint8_t *buffer, size_t maxLen, size_t) {
      return st->fill(buffer, maxLen); // Call fill method to populate data on demand
    });

  // Add HTTP headers to disable browser caching and allow CORS
  response->addHeader("Cache-Control", "no-cache");
  response->addHeader("Access-Control-Allow-Origin", "*");
  
  // Register disconnect callback to release memory
  request->onDisconnect([st]() {
    Serial.println("Stream client disconnected");
    delete st;
  });
  request->send(response);
}

// Define whitelist of valid control commands
static const char *VALID_CMDS[] = {"F", "B", "L", "R", "S", "O", "C", "1", "2"};

// Route callback for handling control command requests
void handleControl(AsyncWebServerRequest *request) {
  // Check if "cmd" parameter is included in the request
  if (!request->hasParam("cmd")) {
    request->send(400, "text/plain", "Missing command");
    return;
  }

  // Get parameter value
  String cmd = request->getParam("cmd")->value();
  
  // Verify if command is in the whitelist
  for (auto valid : VALID_CMDS) {
    if (cmd == valid) {
      sendToUNO(cmd); // Valid command, forward to ESP32S3
      request->send(200, "text/plain", "OK");
      return;
    }
  }

  // Invalid command handling
  Serial.printf("Unknown command: %s\n", cmd.c_str());
  request->send(400, "text/plain", "Unknown command");
}

// ================= TFT Screen Driver Definitions =================
// 1.54-inch ST7789 TFT screen pin definitions
static const int TFT_SCK_PIN = 45;  // SPI Clock
static const int TFT_MOSI_PIN = 48; // SPI Master Out Slave In
static const int TFT_DC_PIN = 47;   // Data/Command selection
static const int TFT_CS_PIN = 21;   // Chip Select

// Initialize SPI bus and screen object
SPIClass tinyVisionTftSPI(FSPI);
Adafruit_ST7789 tinyVisionTft(&tinyVisionTftSPI, TFT_CS_PIN, TFT_DC_PIN, -1);
bool tinyVisionTftReady = false;

// Screen status display function
void tinyVisionTftStatus(const String &title, const String &line1,
                         const String &line2, const String &line3) {
  if (!tinyVisionTftReady) return;
  tinyVisionTft.fillScreen(ST77XX_BLACK);       // Clear screen
  tinyVisionTft.setTextWrap(false);             // Disable auto-wrapping
  tinyVisionTft.setTextSize(2);                 // Set font size
  tinyVisionTft.setTextColor(ST77XX_CYAN);      // Title color: Cyan
  tinyVisionTft.setCursor(12, 18);
  tinyVisionTft.println(title);
  tinyVisionTft.drawFastHLine(12, 44, 216, ST77XX_BLUE); // Draw a blue dividing line
  tinyVisionTft.setTextColor(ST77XX_WHITE);     // Body text color: White
  tinyVisionTft.setCursor(12, 72);
  tinyVisionTft.println(line1);
  tinyVisionTft.setCursor(12, 110);
  tinyVisionTft.println(line2);
  tinyVisionTft.setTextColor(ST77XX_GREEN);     // Status color: Green
  tinyVisionTft.setCursor(12, 148);
  tinyVisionTft.println(line3);
}

// TFT screen initialization
void tinyVisionTftInit() {
  tinyVisionTftSPI.begin(TFT_SCK_PIN, -1, TFT_MOSI_PIN, TFT_CS_PIN);
  tinyVisionTft.init(240, 240);       // Initialize 240x240 resolution
  tinyVisionTft.setRotation(2);       // Set screen rotation orientation
  tinyVisionTftReady = true;
  tinyVisionTftStatus("TinyVision", "TFT: ST7789", "Camera: Starting", "WiFi: Waiting");
  Serial.println("TFT ST7789 status screen ready");
}

// ================= Main Program Entry =================
void setup() {
  Serial.begin(115200); // Initialize USB debug serial port
  delay(500);
  Serial.println("\n================================");
  Serial.println("ESP32-S3 Smart Car");
  Serial.println("================================");
  Serial.printf("PSRAM: %u, Free Heap: %u\n", ESP.getPsramSize(), ESP.getFreeHeap());
  
  tinyVisionTftInit(); // Initialize screen
  
  // Initialize serial port for communication with ESP32S3 car control
  Serial1.begin(UNO_BAUD, SERIAL_8N1, UNO_RX_PIN, UNO_TX_PIN);
  Serial.printf("UNO Serial1: TX=GPIO%d, RX=GPIO%d\n", UNO_TX_PIN, UNO_RX_PIN);
  
  setupCamera(); // Initialize camera
  tinyVisionTftStatus("TinyVision", "Camera: OK", "WiFi: Connecting", "");
  
  // Start Wi-Fi connection (AP mode or STA mode; code here looks like STA, but cars usually act as AP. If AP, change to WiFi.softAP)
  // Note: Original code uses WiFi.begin, indicating it connects to a router as STA. If direct phone connection is needed, change to WiFi.softAP(ssid, password)
  WiFi.begin(ssid, password); 
  WiFi.setSleep(false); // Disable Wi-Fi sleep to ensure low-latency video streaming
  Serial.print("WiFi connecting");
  tinyVisionTftStatus("TinyVision", "Camera: OK", "WiFi: Connecting", "");
  
  // Wait for Wi-Fi connection to succeed
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  tinyVisionTftStatus("TinyVision", "Camera: OK", "WiFi: Connected", "IP: " + WiFi.localIP().toString());
  
  // Configure Web Server routes
  // Root path returns frontend HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });
  // Control command route
  server.on("/control", HTTP_GET, handleControl);
  // Video stream route
  server.on("/stream", HTTP_GET, handleStream);
  
  server.begin(); // Start server
  Serial.printf("HTTP server started\nOpen: http://%s\n", WiFi.localIP().toString().c_str());
  tinyVisionTftStatus("TinyVision Ready", "Camera: OK", "WiFi: Connected", "IP: " + WiFi.localIP().toString());
}

void loop() {
  // Asynchronous server handles interrupts in the background, main loop just needs to keep running
  delay(1 * 1000);
}
```


### 4.5 Experimental Verification and Expected Phenomena

#### 1. Hardware Connection Check
1. Ensure the TinyVision module is securely mounted on top of the car.
2. Check whether the serial ribbon cable between TinyVision and ESP32S3 is firmly plugged in. Confirm that **TX connects to RX, RX connects to TX**, and **GND shares a common ground**.
3. Ensure the car motor wires are correctly connected to the driver board, and the battery has sufficient charge.

#### 2. Flashing and Running Steps
1. Use a Type-C data cable to connect the ESP32S3 development board to the computer, select the correct COM port, and flash the ESP32S3 code from **Section 1.4**.
2. Connect the TinyVision module to the computer, **making sure that the `index_html.h` file is in the same directory as the code**, and flash the TinyVision code from **Section 1.5**.
3. After flashing is complete, power on the car.

#### 3. Expected Phenomena Description
- **Screen Display**: The TinyVision TFT screen will sequentially display the startup status and finally stay on the `TinyVision Ready` interface, showing the assigned IP address (e.g., `192.168.1.100`).
- **Wi-Fi Connection**: Open the Wi-Fi settings on your phone or tablet, find the Wi-Fi hotspot named `FKS0008`, and connect to it (password `88888888`).
- **Web Access**: Open the browser on your phone and enter the IP address displayed on the screen into the address bar.
- **Video and Control**:
  - After the web page loads, you should see the real-time video feed from the camera (there may be a 1-2 second delay).
  - Direction control buttons (Forward, Backward, Left Turn, Right Turn, Stop) and other function buttons should appear on the webpage.
  - Click the "Forward" button, and the car should move forward smoothly; click "Stop", and the car should stop immediately. When turning, the car should execute differential steering.

#### 4. Result Analysis
This experiment successfully verifies the feasibility of the dual-core architecture. Utilizing its powerful image processing capabilities and asynchronous web server, TinyVision achieves low-latency MJPEG video streaming push; meanwhile, the ESP32S3 precisely executes motion control commands through high-precision PWM and stable serial communication. Together, they complete a comprehensive IoT video car project.

### 4.6 Common Issues and Troubleshooting (FAQ)

**Q1: Compiling the TinyVision code throws an error `index_html.h: No such file or directory`?**
**A**: This is because the front-end web header file is missing. Please check your downloaded code materials package and ensure that the `index_html.h` file is placed in the same folder directory as the `.ino` or `.c` code file. If you are using graphical software like KidsBlock, make sure to check "Export Web Files" or manually rename the generated web file to `index_html.h` and place it into the project directory.

**Q2: The video stream is extremely lagging, has high latency, or frequently turns black?**
**A**:

1. **Wi-Fi Interference**: Ensure that your phone and the car are connected to the same Wi-Fi network, keep the distance reasonable, and stay away from 2.4G interference sources such as microwave ovens.
2. **Image Quality Too High**: In the TinyVision code, find `frame2jpg(fb, 80, &jpg, &jpgLen)`, and lower `80` (JPEG quality) to `50` or `40` to significantly reduce the data volume and increase the frame rate.
3. **Resolution Too High**: Change `FRAMESIZE_QVGA` (320x240) to `FRAMESIZE_QQVGA` (160x120) to further reduce bandwidth pressure.

**Q3: Clicking the web buttons does nothing, or the direction is reversed?**
**A**:

1. **No Response**: Open the ESP32S3 USB serial monitor (baud rate 115200) to check if print information like `Receive: F` appears. If not, it indicates serial communication has failed. Please check whether the TX/RX connections between TinyVision and ESP32S3 are crossed, and ensure they share a common ground.
2. **Direction Reversed**: If moving forward results in moving backward, you can swap `HIGH` and `LOW` in the `forward` function within the ESP32S3 code; or directly swap the two wires of the corresponding motor in hardware.

**Q4: The TinyVision screen keeps displaying "WiFi: Connecting" and fails to obtain an IP address?**
**A**: Check whether the `ssid` and `password` in the code match your router's Wi-Fi name and password exactly (case-sensitive). If you are using it in a router-free environment, modify `WiFi.begin(ssid, password);` in the code to `WiFi.softAP(ssid, password);` to enable AP hotspot mode.
```