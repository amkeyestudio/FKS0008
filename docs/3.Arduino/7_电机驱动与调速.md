# 3.7 Motor Driving and Speed Control

## 3.7.1 Lesson Introduction

Imagine you are playing with a remote-control racing car, or watching a sweeping robot automatically clean your house around. Why can they run? And why can they go as fast or as slow as they want, and move forward or backward at will? The secret lies in the fact that they hide a "strongman" and a "commander" inside their bodies. The "strongman" is the DC Motor, and the "commander" is the DRV8835 motor driver module we are going to learn about today.

In this lesson, we will no longer just light up tiny LEDs, but make a real motor spin! We will learn how to connect a powerful motor driver module to the ESP32S3 Pro development board and control the motor's rotation direction and speed through code. This is like learning to ride a bicycle for the first time—starting from simple balance to being able to freely control direction and speed.

After completing this lesson, you will have mastered the core skills of controlling mechanical motion. In the future, if you want to build an automatic plant-watering car, a dancing robot, or a smart fan, you will need the knowledge you learned today. Are you ready? Let's wake up the sleeping motor together!

## 3.7.2 Lesson Objectives

*   Correctly wire the DRV8835 motor driver module, ESP32S3 Pro development board, and DC motor.
*   Understand the basic concept of an H-bridge circuit and know how to control the motor's forward and reverse rotation by changing pin levels.
*   Master the method of using PWM (Pulse Width Modulation) technology to adjust motor speed.
*   Write a program to realize a complete motor cycle of "forward acceleration -> stop -> reverse acceleration -> stop", and understand the principle of differential steering for dual wheels.

## 3.7.3 Lesson Equipment

| Name               | Specification/Model           | Quantity | Remarks        |
| :----------------- | :---------------------------- | :------- | :------------- |
| Main Control Board | ESP32S3 Pro Development Board | 1        | Our core brain |
| Motor Driver Expansion Board | DRV8835 Motor Driver Expansion Board | 1 | The "strongman" that controls the motors |
| DC Motor | 3V-6V Small DC Motor | 4 | The controlled executor (this tutorial uses a dual-motor robot car as an example, though a single motor works too) |
| Battery Holder | 6-slot AA battery holder or 2-slot 18650 battery holder | 1 | Provides power for the motors |
| Batteries (Self-provided) | AA batteries or 18650 batteries | / | Requires 6 AA batteries or 2 18650 batteries. Choose based on your situation; 18650 batteries are recommended as a priority |

## 3.7.4 Course Principles

### 3.7.4.1 Why is a Motor Driver Module Needed?

You might ask: "The ESP32S3 Pro development board can output power by itself, so why can't we connect the motors directly to it?"

This is like trying to push a heavy truck: even though you are trying very hard (the ESP32S3 outputs 3.3V), your strength is too small (the current is only a few milliamperes), and you simply cannot move it. At this time, you need a "strongman" helper called the DRV8835. The DRV8835 can receive the weak command signals sent by the ESP32S3, and then use an external high-power power supply to output a strong current to drive the motors.

In addition, connecting motors directly poses another danger: motors generate back electromotive force (back-EMF) when starting or stopping, which can burn out the fragile ESP32S3 chip. The DRV8835 acts like a firewall, protecting our "brain."

### 3.7.4.2 H-Bridge: The Secret to Making Motors Spin Forward and Backward

A DC motor is very simple: connect the positive terminal to a high level and the negative terminal to a low level, and it spins forward; reverse the connections, and it spins backward. But we can't unplug and plug wires every time!

Inside the DRV8835, there is a circuit structure called an "H-bridge." You can think of it as an "H"-shaped bridge made up of four electronic switches. By controlling the opening and closing of these four switches, we can change the direction of the current flowing through the motor, thereby making the motor spin forward or backward without swapping wires.

In the DRV8835 module used in this tutorial (IN/EN mode), the control logic is as follows:

*   **IN pin is HIGH, EN pin inputs PWM**: Current flows forward, motor spins forward.
*   **IN pin is LOW, EN pin inputs PWM**: Current flows backward, motor spins backward.
*   **EN pin is LOW**: Regardless of the IN state, the motor stops spinning.

### 3.7.4.3 PWM Speed Control: Controlling Speed Like a Faucet

If we simply power the motor, it will only rotate at full speed. What if we want it to go slower?

Remember the PWM we learned about in the "Breathing LED" lesson? For motors, the principle is the same. We quickly send "ON" and "OFF" signals to the motor.

*   If the "ON" time is long and the "OFF" time is short (high duty cycle), the motor gets more average energy and spins faster.
*   If the "ON" time is short and the "OFF" time is long (low duty cycle), the motor gets less average energy and spins slower.

On the ESP32S3, we use the `analogWrite` function (underlyingly based on the LEDC hardware peripheral) to implement PWM output. To simplify operations, this tutorial will use the recommended PWM control method in the ESP32S3 Arduino Core library.

## 3.7.5 Wiring Instructions

Wiring is the most critical step in the experiment, so please check carefully. There are many interfaces on the DRV8835 module, and we only need to pay attention to a few key ones.

**General Concept:**

1.  The DRV8835 requires an external power supply: Because the current required for motor driving is relatively large, an external power supply must be used to drive the motors.
2.  The ESP32S3 sends control signals to the input pins of the DRV8835 (AIN, AEN, BIN, BEN).
3.  The output pins of the DRV8835 (OUTA, OUTB) connect to the motors.
4.  **Important**: The GND of the ESP32S3 must be connected to the GND of the DRV8835 (common ground), otherwise the signals cannot be recognized.

**Detailed Wiring Table:**

| ESP32S3 Expansion Board Motor Driver Pin | ESP32S3 Pro Development Board Pin | Description                             |
| :--------------------------------------- | :-------------------------------- | :-------------------------------------- |
| AIN                                      | IO40                              | Motor A direction control (left wheel)  |
| AEN                                      | IO41                              | Motor A PWM speed control (left wheel)  |
| BIN                                      | IO38                              | Motor B direction control (right wheel) |
| BEN                                      | IO21                              | Motor B PWM speed control (right wheel) |

**Please note that the motor driver pins are fixed and cannot be changed!**

## 3.7.6 Example Code

This code will make the motors (or the car's dual wheels) first accelerate forward, then decelerate to a stop, then accelerate in reverse, stop again, and demonstrate differential steering, repeating this cycle.

```cpp
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
```

## 3.7.7 Code Explanation

Let's break down this code to see how it directs the "Hercules" to work.

**1. Pin Definitions and PWM Configuration**

```cpp
#define MOTOR_AIN 40  
#define MOTOR_AEN 41  
#define MOTOR_BIN 38  
#define MOTOR_BEN 21  
```

We store the connected pin numbers using macro definitions. This makes the subsequent code clearer and allows us to modify the pins in just one place if needed.

**2. Motion Control Function Logic**

Taking `forward()` as an example:

```cpp
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel direction: forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel direction: forward
  analogWrite(MOTOR_AEN, leftSpeed);
  analogWrite(MOTOR_BEN, rightSpeed);
}
```

In IN/EN mode, setting the direction pins (AIN/BIN) to `HIGH` represents forward rotation, and `LOW` represents reverse rotation. Then, speed is controlled by outputting a PWM value from 0 to 255 to the enable pins (AEN/BEN) via `analogWrite`. The `left()` and `right()` functions achieve in-place turns by utilizing speed or direction differences between the two wheels.

**3. Initialization in `setup()`**

```cpp
  analogWriteResolution(8);
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);
```

`analogWriteResolution(8)` ensures that the PWM value ranges from 0 to 255. The direction pins must be configured as `OUTPUT` mode. The `analogWrite` function of the ESP32S3 automatically configures PWM channels, so there is no need to manually bind channels like in older versions of Arduino.

## 3.7.8 Experimental Results

After uploading the code and turning on the power, observe your motor (or car):

1.  **Startup**: The motor should start from a standstill and accelerate very smoothly into rotation (forward direction). You will hear the motor sound gradually getting louder.
2.  **Full Speed**: After acceleration is complete, the motor will maintain full-speed rotation for about 2 seconds.
3.  **Stop**: Then, the motor will slowly decelerate until it comes to a complete stop. At this time, the motor should be completely silent, and the shaft should not rotate.
4.  **Reverse and Turning**: After a 1-second pause, the motor will slowly accelerate in the opposite direction; subsequently, in-place left turns and right turns will be demonstrated.
5.  **Loop**: After completing all actions, it stops for 2 seconds and then repeats the above process.

If you open the Serial Monitor (with the baud rate set to 115200), you will see the current status prompts printed by the program, such as "Status: Forward", "Status: Turn Left", etc. This helps you confirm which step the program is currently executing.

**Awesome! If you see the motor running to this rhythm, congratulations! You have successfully controlled real mechanical power!**

## 3.7.9 FAQ

**Problem: The motor only buzzes but does not rotate.**

*   **Cause**: Insufficient power supply. The current provided by USB may not be enough to drive the motor to start, especially when the torque is low at low speeds.
*   **Solution**: Try using an external 5V-12V power supply connected to the VMOT input of the DRV8835, and ensure that GND shares a common ground with the ESP32S3. Alternatively, check if the motor is physically jammed.

**Problem: The motor rotates in the opposite direction of what was expected.**

*   **Cause**: OUTA and OUTB are swapped during wiring, or the logic of AIN/BIN in the code is reversed.
*   **Solution**: The easiest method is to swap the two motor wires connected to OUTA and OUTB of the DRV8835. There is no need to modify the code.

**Problem: The motor can only spin at full speed and cannot be speed-regulated.**

*   **Cause**: The AEN/BEN pins are not connected correctly, or the value of `analogWrite` in the code is always 255.
*   **Solution**: Check the DRV8835 module to ensure that the jumper wires for the AEN and BEN pins are correctly connected to IO41 and IO21 of the ESP32S3, and are not mistakenly connected to regular digital pins.

**Problem: An error occurs when uploading code or the motor does not move.**

*   **Cause**: Pin definitions are wrong, or the ESP32S3 is not recognized correctly.
*   **Solution**: Check whether the pin numbers in the code match the wiring table (IO40, IO41, IO38, IO21). Make sure the correct board model "ESP32S3 Dev Module" is selected in the Arduino IDE.

## 3.7.10 Precautions and Safety Tips

*   **Strictly Prohibit Short Circuits**: The output terminals (OUTA/OUTB) of the DRV8835 must never be short-circuited directly, nor should they be connected to the positive and negative poles of the power supply, otherwise, the module will be instantly burned out.
*   **Pay Attention to Heat**: The DRV8835 may heat up while working, which is a normal phenomenon. However, if it is scalding hot, please cut off the power immediately and check whether the load is too large or the wiring is wrong.
*   **Finger Safety**: The motor has great power when rotating. Do not bring your fingers close to the rotating shaft or gears while the motor is rotating to avoid pinch injuries.
*   **Common Ground Principle**: In electronic circuits, the GND (ground wire) of all modules must be connected together, otherwise, the control signal cannot form a loop, causing the device to fail to work.