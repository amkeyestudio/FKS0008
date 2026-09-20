# 3.12 Ultrasonic Obstacle-Avoiding Car

## 3.12.1 Course Introduction

Imagine you are walking blindfolded. If there is a wall in front of you, what would you do? You might reach out with both hands to feel, or use a blind cane to probe the way. Our "Ultrasonic Obstacle-Avoiding Car" does the exact same thing! Although it has no eyes, it has a magical "bat ear" — an ultrasonic sensor. This sensor can emit sounds we cannot hear. When the sound bounces back after hitting an obstacle, the car knows there is something ahead.

Furthermore, to make the car smarter, we have installed a rotatable "neck" — a servo pan-tilt. When the car finds an obstacle ahead, it turns its head left and right like a human to observe the road conditions on both sides, thereby choosing the safest direction to avoid it.

In real life, this technology is very useful. For example, robot vacuums won't knock over your slippers, autonomous cars can avoid pedestrians suddenly rushing out, and even automated guided vehicles (AGVs) in factories can safely shuttle between shelves. Today, we will combine the motor driving, servo control, and ultrasonic ranging learned previously to turn the car into a smart "explorer."

After completing this lesson, you will master how to make the car make decisions based on distance: go straight if far away, stop, look left and right, and turn if close. This is not just programming; it is teaching machines the ability to "think." Are you ready? Let's start endowing the car with intelligence!

## 3.12.2 Course Objectives

*   Correctly connect the ultrasonic sensor, servo, and motor driver module to the ESP32S3 Pro development board, and understand the role of each pin.
*   Write code to read data from the ultrasonic sensor, and master the method of using a servo to drive the sensor for left and right sweeping angle ranging.
*   Combine motor control logic to implement a complete intelligent obstacle avoidance process: "Detect obstacle -> Stop -> Servo left/right ranging -> Compare distances -> Turn to avoid -> Continue forward."
*   Learn to use `if...else` conditional statements to make the program react differently according to different distances and left/right road conditions.

## 3.12.3 Course Equipment

| Name               | Specification/Model           | Quantity | Remarks                           |
| :----------------- | :---------------------------- | :------- | :-------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board | 1        | Core Brain                        |
| Ultrasonic Module  | HC-SR04                       | 1        | The "Eyes" of the Car             |
| Micro Servo        | SG90                          | 1        | The "Neck" of the Car             |
| Expansion Board    | ESP32S3 Pro Expansion Board   | 1        | Integrated motor driver function. |

## 3.12.4 Lesson Principles

**1. Servo Pan-Tilt: The "Flexible Neck" of the Car**
If the ultrasonic sensor is fixed in place, the car can only see directly ahead. When it encounters an obstacle, it doesn't know whether the left or right side is more open. By introducing a servo, we can mount the ultrasonic sensor on the servo turntable. When the car detects an obstacle in front, it controls the servo to turn to the left (180 degrees) and right (0 degrees) respectively to measure distances, thereby acquiring environmental information on both sides and providing data support for intelligent steering.

**2. Programming Logic: How the Car's "Brain" Thinks**
We need to use `if...else` statements, which are like answering multiple-choice questions:

*   **If** the front distance is greater than the safety threshold (e.g., 15 cm, safe), then both motors rotate forward (move straight).
*   **Otherwise** (front distance is less than or equal to 15 cm, danger), then:
    1. Stop the motors.
    2. Control the servo to turn to 180 degrees to measure the left distance.
    3. Control the servo to turn to 0 degrees to measure the right distance.
    4. Return the servo to the center (90 degrees).
    5. Compare the left and right distances: if the left distance is greater than the right, turn left; otherwise, turn right.
    6. Turn for a certain duration and then stop, waiting for the next detection.

## 3.12.5 Wiring Instructions

Wiring is the most critical step in the car experiment because there are many wires, making miswiring easy. Please disconnect the battery first to ensure safety comes first! We will wire in three parts: the sensor part, the servo part, and the motor part.

⚠️ **Note**: The following wiring pins are completely consistent with the definitions in the sample code. If you change the wiring pins during actual operation, make sure to update the pin definitions in the code accordingly.

**Part 1: Ultrasonic Sensor (HC-SR04)**
The ultrasonic module has 4 pins. The HC-SR04 operates at 5V, while the ESP32 GPIO operates at 3.3V logic level. For stability, we directly use the 5V power supply from the development board, and connect the signal lines directly to the GPIO.

| Component Pin | ESP32S3 Pro Development Board Pin | Description                                             |
| :------------ | :-------------------------------- | :------------------------------------------------------ |
| VCC           | 5V                                | Provides 5V power                                       |
| GND           | GND                               | Ground                                                  |
| Trig          | IO13                              | Trigger pin, controls ultrasonic wave emission via code |
| Echo          | IO12                              | Echo pin, receives the returning signal                 |

**Part 2: Servo (SG90)**
The servo has 3 wires, typically colored brown/red/orange or black/red/yellow.

| Component Pin (Wire Color) | ESP32S3 Pro Development Board Pin | Description             |
| :------------------------- | :-------------------------------- | :---------------------- |
| Brown/Black (GND)          | GND                               | Ground                  |
| Red (VCC)                  | 5V (or VIN)                       | Provides 5V power       |
| Orange/Yellow (Signal)     | IO42                              | PWM signal control line |

⚠️ **Important Notes:**

1. **Common Ground Principle**: The battery negative terminal, development board GND, and motor driver GND must be connected together, otherwise the car will behave erratically or fail to work.
2. **Power Separation**: Motors and servos consume significant power. Please use the battery to directly power the motor driver and servo (VIN or external interface), while USB should only supply power to the development board and download programs.
3. **Check Polarity**: If the car moves backward instead of forward, simply swap the two wires of the corresponding motor without changing the code.

## 3.12.6 Sample Code

This code makes the car continuously detect the distance ahead. If the distance is greater than 15 cm, the car moves straight; if it is less than 15 cm, the car stops, turns its head left and right via the servo to measure the distances on both sides, and then turns toward the more open side to avoid the obstacle.

```cpp
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
```

## 3.12.7 Code Explanation

To help everyone better understand how the code controls the smart car, we have divided the code into several core modules for detailed explanation:

**1. Library Import and Object Creation**

```cpp
#include <ESP32Servo.h>
Servo myServo;
```

Imports the ESP32-specific servo control library and creates a servo object named `myServo`. Subsequently, the servo's rotation is controlled by calling the methods of this object.

**2. Pin and Parameter Definition**
The beginning of the code centrally defines the pin numbers for the ultrasonic sensor, servo, and motors, as well as the obstacle avoidance safety distance threshold `closeDistance`. Centrally defining pins and parameters makes it convenient for us to modify hardware wiring by simply changing the numbers here, without having to search everywhere in the code.

**3. `setup()` Initialization Function**
Executed once when the program starts up. It mainly completes the following tasks:

*   Initializes the serial port for outputting debugging information.
*   Sets the input and output modes for the ultrasonic Trig and Echo pins.
*   Sets the motor control pins to output mode, and calls `stopMotor()` to ensure the car is stationary in its initial state.
*   Configures the ESP32 PWM timer, initializes the servo object, and returns the servo to 90 degrees (straight ahead) to prevent erratic movement upon powering on.

**4. `loop()` Main Loop and Obstacle Avoidance Logic**
This is the "brain" of the smart car, continuously looping through the following logic:

*   **Front Ranging**: Calls `getDistance()` to obtain the distance directly ahead.
*   **Safety Judgment**: If the distance is greater than 15 cm, calls `forward()` to move straight.
*   **Obstacle Handling**: If the distance is less than or equal to 15 cm, enters the obstacle avoidance process:
    *   Controls the servo to turn to 180 degrees, measures the left distance, and stores it in `leftDistance`.
    *   Controls the servo to turn to 0 degrees, measures the right distance, and stores it in `rightDistance`.
    *   Returns the servo to the center at 90 degrees.
    *   Compares `leftDistance` and `rightDistance`; whichever side has a greater distance, calls `left()` or `right()` to turn in that direction.
    *   Turns for 500 milliseconds and then stops, waiting for the next loop detection.

**5. `getDistance()` Ranging Function**
Encapsulates the standard timing for ultrasonic ranging: first pulls Trig low, then gives a 10-microsecond high level trigger, and finally uses `pulseIn()` to read the Echo high level duration. The actual distance is calculated using the formula `time * 0.034 / 2`, and an effective range filter of 2~300 cm is added to weed out abnormal data.

**6. Motion Control Functions**
`forward()`, `back()`, `left()`, `right()`, and `stopMotor()` respectively encapsulate the five basic motion states of the smart car. Direction is determined by combining high and low levels on the direction pins (`AIN/BIN`), and car speed is controlled by outputting PWM waves via `analogWrite()` to the speed pins (`AEN/BEN`).

## 3.12.8 Experimental Phenomenon

Upload the code to the ESP32S3 Pro development board, connect the motor power supply, and place the smart car on a flat surface.

1.  **Straight Moving Stage**: If there are no obstacles within 15 cm ahead, you should see the smart car moving forward in a straight line.
2.  **Data Monitoring**: Open the Arduino IDE Serial Monitor (set the baud rate to 115200), and you will see lines of numbers beating, such as `Current Front Distance: 150 cm`, which means the path ahead is clear.
3.  **Obstacle Avoidance Stage**: When you block the front of the car with your hand or a book within 15 cm:
    *   The smart car will immediately stop moving forward.
    *   You will see the ultrasonic module mounted on the servo first turn to the left (180 degrees) and pause; then turn to the right (0 degrees) and pause; and finally return to the front (90 degrees).
    *   Based on the results of the left and right distance measurements, the smart car will turn left or right (left and right wheels rotating in opposite directions).
    *   The turn lasts for about 0.5 seconds and then stops, followed by continuing to detect the distance ahead.
    *   If you remove your hand and the path ahead is clear, the smart car will attempt to move straight again.

Congratulations! You have just used code to control real hardware, giving it "vision", a "flexible rotating neck", and "intelligent decision-making capabilities"!

## 3.12.9 FAQ

**Issue: The Serial Monitor displays Distance: 0 cm, but there is clearly something in front.**

*   **Cause**: The wiring may be loose, or Trig and Echo are swapped. Alternatively, there might be environmental noise interference, or the obstacle surface is sound-absorbing (such as soft sponge).
*   **Solution**: Check whether IO13 and IO12 are connected correctly. Ensure that the ultrasonic module is facing a hard, flat obstacle. Try testing at a different angle or against a different background (such as cardboard).

**Issue: The smart car keeps spinning in circles and won't go straight.**

*   **Cause**: The wiring logic of the left and right motors is inconsistent, or one of the motor wires is reversed.
*   **Solution**: Observe the `forward` function in the code to ensure that the `HIGH/LOW` combination for both wheels makes them spin forward. If you are not sure, you can test each motor individually and adjust the jumper wire positions until both wheels spin forward.

**Issue: The smart car does not stop when encountering an obstacle, but crashes directly into it.**

*   **Cause**: The `closeDistance` threshold is set too small, or the `delay` time is too short, causing it to react too slowly.
*   **Solution**: Change `closeDistance` in the code to 20 or 30 to increase reaction space. Check the battery level; insufficient battery power will lead to decreased motor torque and delayed braking.

**Issue: The servo jitters instead of rotating, or the rotation angle is incorrect.**

*   **Cause**: Insufficient power supply for the servo, or mismatched pulse width parameter settings.
*   **Solution**: Ensure that the red wire of the servo is connected to 5V or VIN, not the 3.3V pin. If you are using a non-SG90 model servo, please modify the pulse width parameters in `myServo.attach(servoPin, 500, 2400);` (e.g., change them to 1000, 2000).

**Issue: Error "Connection Error" when uploading code.**

*   **Cause**: The ESP32S3 Pro may need to enter download mode, or the USB cable can only charge and cannot transfer data.
*   **Solution**: Press and hold the `BOOT` button on the board, click upload, and release it after seeing the prompt. Make sure you are using a high-quality data cable.

### ⚠️ Safety Tips

* **Strictly Prohibit Short Circuits**: Never let the positive and negative terminals of the battery touch each other directly, nor let the output ends of the motor driver short-circuit. This will instantly burn out components or cause the battery to overheat.

* **Lift Testing**: When debugging motor directions and servo angles, it is recommended to prop up the smart car so the wheels are suspended in the air, preventing it from running around randomly and damaging items or injuring people.