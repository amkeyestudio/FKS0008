# 3.11 Ultrasonic Following Car

## 3.11.1 Course Introduction

In this lesson, we will combine the **motor drive** (the module that controls the rotation of the wheels) learned previously with the ultrasonic sensor to build a smart "following car". It can not only avoid obstacles, but also closely follow an object moving in your hand. Imagine the car following you wherever you go—isn't that super cool?

After completing this lesson, you will master how to make the car automatically adjust its movement state based on distance: chase after the target if it is far away, stop if it is close, and back up to avoid it if it is too close. Congratulations, you are about to give the car real "intelligence"!

## 3.11.2 Course Objectives

*   Correctly wire the ultrasonic sensor to the ESP32S3 Pro development board (the car's "brain").
*   Write code to read distance data from the ultrasonic sensor (allowing the car to "see" distance).
*   Control the car to move forward, stop, or back up based on distance values (allowing the car to "move").
*   Understand the application of **logical judgment** using "If... Then..." statements (just like multiple-choice questions in real life: if condition A is met, do action A; if condition B is met, do action B) in robot control.

## 3.11.3 Course Equipment

Before we start, let's take inventory of today's "gear". Don't worry, every component has its unique purpose:

| Component Name     | Specification/Model           | Qty  | Notes (Layman's Explanation)                                 |
| :----------------- | :---------------------------- | :--- | :----------------------------------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board | 1    | Core brain: Responsible for receiving signals, calculating distances, and issuing commands. |
| Ultrasonic Sensor  | HC-SR04                       | 1    | The car's "eyes": Responsible for emitting ultrasonic waves and receiving echoes to calculate distance. |
| Expansion Board    | ESP32S3 Pro Expansion Board   | 1    | Bridge: Because the interfaces (pins) on the development board are too small and dense, the expansion board enlarges and categorizes the interfaces, making it easier for us to plug in wires. |

## 3.11.4 Programming Logic:

To make the car smart, it doesn't need to actually understand human language; we just need to teach it how to answer "multiple-choice questions". In programming, this is called a **decision tree** or **logical judgment**, which is the rule of "If... Then...":

*   **If** distance > 30 cm and ≤ 50 cm (target is ahead and far away), **then** the car moves forward (chases after it).
*   **If** distance is between 10 and 30 cm (distance is just right), **or** distance > 50 cm (exceeds the maximum following range), **then** the car stops (maintains following distance without crashing into it, or prevents running away).
*   **If** distance < 10 cm and > 0 cm (too close, about to collide), **then** the car reverses (safely avoids collision).

## 3.11.5 Hardware Wiring Instructions

Wiring is the most critical step in the experiment. Please carefully compare with the table below. **Why is wiring so important?** Electricity is like water flow; if the path isn't built correctly, the water won't flow through, and it might even "flood" and damage the components. To ensure safety, please unplug the USB cable of the ESP32S3 Pro development board (disconnect the power) before doing any wiring.

**Overall Concept**: The ultrasonic sensor is responsible for "seeing", the ESP32S3 Pro for "thinking", and the motor driver for "moving".

| Ultrasonic Sensor Pin | ESP32S3 Pro Development Board Pin | Description                                                  |
| :-------------------- | :-------------------------------- | :----------------------------------------------------------- |
| VCC                   | 5V                                | Provides 5V power supply. Be careful not to connect it to 3.3V, otherwise, it may work unstably. |
| Trig                  | IO13                              | Trigger signal, controlled by the development board to emit ultrasonic waves. |
| Echo                  | IO12                              | Echo signal, the development board reads the duration of the high level here. |
| GND                   | GND                               | Ground, forming a circuit.                                   |

**⚠️ Wiring Precautions**:

1. **Power-off Operation**: When plugging and unplugging jumper wires, make sure the development board is powered off to prevent short circuits.
2. **Direction Confirmation**: The two metal cylinders on the front of the ultrasonic sensor (like eyes) must face forward and must not be blocked by the car chassis or wires.
3. **Firm Connections**: Ensure the jumper wires are plugged in tightly to avoid poor contact leading to fluctuating data.

## 3.11.6 Sample Code

This code makes the smart car continuously detect the distance of objects ahead. If the object is between 30 and 50 cm, the car moves forward; if it is between 10-30 cm or greater than 50 cm, the car stops; if it is less than 10 cm, the car reverses.

Don't be intimidated by the length of the code; we have added detailed "translations" to every single line so you can understand it at a glance!

```cpp
// ================= Ultrasonic Pin Definitions =================
const int trigPin = 13;  // Trigger pin connected to IO13
const int echoPin = 12;  // Echo pin connected to IO12

// ================= Distance Threshold Settings =================
// Set the "boundary lines" for distances (Unit: cm)
const long safeDistance = 30;   // Define the upper limit of safe distance as 30 cm. Stop or reverse below this distance.
const long closeDistance = 10;  // Define the lower limit of danger distance as 10 cm. Reverse below this distance.
const long maxDistance = 50;    // Define the maximum following distance as 50 cm. Stop the car if it exceeds this distance to prevent it from running away.

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Path A direction control (Left wheel)
#define MOTOR_AEN 41  // Path A PWM control (Left wheel speed)
#define MOTOR_BIN 38  // Path B direction control (Right wheel)
#define MOTOR_BEN 21  // Path B PWM control (Right wheel speed)

// ================= Initialization Function =================
void setup() {
  // Initialize serial communication with a baud rate of 115200 for faster and smoother data display
  Serial.begin(115200);
  Serial.println("System initialization complete, ultrasonic following car started!");

  // Set ultrasonic pin modes
  pinMode(trigPin, OUTPUT);  // Trig pin as output to send trigger signals
  pinMode(echoPin, INPUT);   // Echo pin as input to receive echo signals

  // Set motor direction pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);
  
  // Ensure motors are stopped in the initial state
  stopMotor();
}

// ================= Main Loop Function =================
void loop() {
  // 1. Get the front distance
  long distance = getDistance();  // Call the ranging function and store the calculated distance in the distance variable.

  // 2. Print the distance in the Serial Monitor for debugging and observation.
  Serial.print("Current Distance: ");  // Print prompt text.
  Serial.print(distance);              // Print specific distance number.
  Serial.println(" cm");               // Print unit " cm" (centimeters) and newline.

  // 3. Control car actions based on distance (core logic judgment)
  if (distance > safeDistance && distance <= maxDistance) {  
    // If the measured distance is greater than 30 cm AND less than or equal to 50 cm
    // Distance is appropriate, chase after it!
    forward(150, 150);  // Call the "forward" function, set left and right wheel speeds to 150.
    
  } else if ((distance >= closeDistance && distance <= safeDistance) || distance > maxDistance) {  
    // Otherwise, if the distance is between 10 cm and 30 cm, OR the distance is greater than 50 cm
    // Maintain a proper following distance, or exceed the max range to prevent losing track
    stopMotor();  // Call the "stop" function.
    
  } else if (distance < closeDistance && distance > 0) {  
    // Otherwise, if the distance is less than 10 cm AND greater than 0 (excluding 0 or negative values caused by sensor failures)
    // Distance is too close, back up quickly!
    back(105, 150);  // Call the "back" function, left wheel 105, right wheel 150 (can be fine-tuned to prevent veering off course).
  }

  // Slight delay (pause) of 100 milliseconds (0.1 seconds).
  // Makes movements smoother, prevents spamming commands, and gives the sensor some "breathing room" to receive echoes.
  delay(100);
}

// ================= Ultrasonic Ranging Function =================
/*
 * Function Purpose: Encapsulate ultrasonic ranging code into a function to simplify the main loop code
 * Function Name: getDistance
 * Function Feature: Returns an integer distance value (Unit: cm)
*/
int getDistance() {
  // Define variables to store time and distance
  long duration;  // Round-trip time of sound waves (microseconds)
  int distance;   // Calculated distance (centimeters)

  // Step 1: Ensure Trig pin is LOW, preparing to transmit
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // Wait for 2 microseconds to ensure signal stability

  // Step 2: Give the Trig pin a 10-microsecond HIGH pulse to trigger the sensor to emit ultrasonic waves
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Step 3: Read the duration of the HIGH level on the Echo pin
  // The pulseIn function waits for the pin to go HIGH, then starts timing until the pin goes LOW
  duration = pulseIn(echoPin, HIGH);

  // Step 4: Calculate distance
  // The speed of sound in air is approximately 0.034 cm/microsecond
  // Distance = (Time × Speed) / 2, because the sound makes a round trip
  distance = duration * 0.034 / 2;

  // Limit the ultrasonic ranging range because values beyond 3 meters or below 2 cm are inaccurate
  if (distance < 2 || distance > 300) {
    distance = 0; // Out-of-range values are treated as invalid data, returning 0
  }
  
  return distance;
}

// ================= Motor Control Functions =================

// Set PWM speeds for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write left wheel PWM value
  ledcWrite(MOTOR_BEN, rightSpeed); // Write right wheel PWM value
}

// Move Forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward rotation
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward rotation
  setMotor(leftSpeed, rightSpeed);
}

// Move Backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse rotation
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse rotation
  setMotor(leftSpeed, rightSpeed);
}

// Stop Motors
void stopMotor() {
  setMotor(0, 0); // Set speeds to 0
}
```

## 3.11.7 Detailed Code Explanation

To help you not only know *what* the code does, but *why* it does it, let's break down the code piece by piece:

1.  **Pin and Threshold Definitions**:
    *   The beginning of the code defines the ultrasonic sensor's `trigPin` and `echoPin`, as well as the four motor control pins.
    *   Three distance thresholds are defined: `safeDistance` (30cm), `closeDistance` (10cm), and `maxDistance` (50cm). These constants make subsequent logic checks clearer and make it easy for you to adjust the following distance at any time.
2.  **`setup()` Initialization**:
    *   `Serial.begin(115200)` starts serial communication for printing debug information on the computer.
    *   The `pinMode()` function sets Trig as an output, Echo as an input, and all motor control pins as outputs.
3.  **`loop()` Main Loop**:
    *   Each loop first calls `getDistance()` to obtain the distance.
    *   It uses an `if...else if...else` structure for conditional judgment. Pay special attention here to the combined use of the logical operators `&&` (AND) and `||` (OR) to ensure every distance range is accurately covered.
    *   The `delay(100)` at the end is very important; it controls the car's "reaction frequency", preventing frequent motor starts and stops that cause lagging.
4.  **`getDistance()` Ranging Function**:
    *   Strictly follows the timing requirements of the HC-SR04: pulls Trig low first, then gives a 10-microsecond high pulse, and finally pulls it low.
    *   Uses `pulseIn()` to read the high-level duration of the Echo pin.
    *   Calculates the distance using physics formulas, and includes abnormal value filtering for values `< 2` or `> 300` to prevent the car from making wrong moves when the sensor is interfered with.
5.  **Motion Control Functions**:
    *   `forward()` and `back()` switch motor directions by changing the high/low states of the direction pins (AIN/BIN).
    *   Outputs a PWM (Pulse Width Modulation) signal via `analogWrite()` to control the motor speed. The parameter range is 0-255, with larger values corresponding to faster speeds.

## 3.11.8 Experimental Phenomena and Testing

After uploading the code, we need to see if the car actually "sees" things.

**How to view data?** 
At the top of the programming software, find and open the **Serial Monitor**. The **Baud rate** (the "speaking speed" for the car and computer to chat) must be set to **115200** (matching `Serial.begin(115200)` in the code, otherwise you will see garbled text).

You should see lines of scrolling numbers displaying the current distance value, such as `Current distance: 45 cm`.

**Observe the car's actions:**

1.  **Away Test**: Block the front of the ultrasonic sensor with your hand or a book at a distance of 40 centimeters. You will find the car's two wheels rotating forward to approach you.
2.  **Following Test**: Slowly move your hand closer. When the distance is around 15-20 centimeters, the car should suddenly stop and stay quietly there. If you move your hand away beyond 50 centimeters, the car will also stop (preventing it from running away).
3.  **Avoidance Test**: Continue to bring your hand closer until the distance is less than 10 centimeters. The car should immediately back up, trying to increase the distance between itself and you.

If the car moves smoothly without strange shaking, congratulations! You have successfully built a following car with preliminary artificial intelligence! Give yourself a round of applause!

## 3.11.9 Common Errors and Troubleshooting

As a beginner, you are bound to fall into a few pitfalls. Don't be discouraged; these are the necessary "trials" to becoming a master. If you encounter issues, check them against the "Pitfall Guide" below to troubleshoot:

**Pitfall 1: The distance displayed on the serial monitor is always 0 or a very large number (like 3000+)**

*   **Cause**: It might be a loose connection, meaning the signal didn't get through; or Trig/Echo are swapped; or there might be strong acoustic interference around, or something blocking the front of the sensor.
*   **Solution**: Check if pins 13 and 12 are connected correctly and plugged in tightly. Make sure there are no obstructions on the front of the sensor (the two metal cylinder "eyes"). Try unplugging the USB cable and plugging it back in to restart the development board.

**Pitfall 2: The car only turns to one side, or spins in place**

*   **Cause**: The wiring polarity of one of the motors is reversed, or the control logic of the left and right motors is inconsistent (e.g., the left goes forward, but the right goes backward).
*   **Solution**: Check the `forward` function in the code to ensure the direction pin voltage states of the left and right motors are consistent (both HIGH or both LOW). If a motor on one side still spins backward, the simplest physical fix is: power off, and swap the two wires connecting that motor to the driver module.

**Pitfall 3: The car's reactions are sluggish, or it stutters**

*   **Cause**: The `delay(100)` pause time in the code is too long, making the car "slow to react"; or the battery power is insufficient, making the motor "weak" (insufficient torque).
*   **Solution**: Try changing `delay(100)` in the code to `delay(50)` to make it react faster. If the battery voltage is low (the car runs weakly), please charge the battery or replace it with a new one.

**Pitfall 4: The ultrasonic sensor gets very hot, even burning to the touch**

*   **Cause**: VCC (positive) and GND (negative) are reversed, or connected to the wrong voltage pin, causing a short circuit.
*   **Solution**: **Cut off the power immediately!** Check if VCC is connected to 5V and GND is connected to GND. Confirm they are not reversed. Wait for the sensor to cool down before checking the circuit again.

**Pitfall 5: Code upload fails, prompting "Connection timeout" or "Board not found"**

*   **Cause**: The USB cable can only charge and cannot transmit data; or the wrong development board model or serial port (COM port) was selected in the software.
*   **Solution**: Switch to a USB cable that is confirmed to transmit data. In the software's "Tools" menu, reselect the correct board (ESP32S3) and the corresponding port number.

## 3.11.10 Safety and Precautions

*   **Testing Environment**: Do not test the car in crowded areas to avoid hitting people (even though it's a small car, getting hit doesn't feel good). It is recommended to test on an open floor or tabletop.
*   **Electrical Safety**: Do not expose the copper wire of the battery connection cables to prevent the two wires from touching each other and causing a short circuit or fire.
*   **Exception Handling**: If you smell a burning odor or see smoke, immediately disconnect the power and check the circuit. Do not touch hot components directly with your hands.
*   **Sensor Protection**: The piezoelectric ceramic sheet inside the ultrasonic sensor is relatively fragile. Do not poke the metal mesh cover on the front of the sensor with hard objects.

Have fun and enjoy exploring!