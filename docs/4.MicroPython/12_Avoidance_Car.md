# 4.12 Ultrasonic Obstacle-Avoiding Smart Car

## 4.12.1 Course Introduction

Imagine you are walking blindfolded; if there is a wall in front of you, what would you do? You might reach out your hands to touch it, or probe the way with a blind cane. Our "Ultrasonic Obstacle-Avoiding Smart Car" does the same thing! Although it has no eyes, it has magical "bat ears"—an **ultrasonic sensor** (a component that can emit and receive ultrasound waves to measure distance). This sensor can emit sounds we cannot hear. When the sound bounces back after hitting an obstacle, the car knows something is ahead.

Not only that, to make the car smarter, we also installed a rotatable "neck"—a **servo pan-tilt** (a servo is a small motor that can precisely control the rotation angle, and the pan-tilt is the base it mounts on). When the car finds an obstacle ahead, it will turn its head left and right like a human, observing the road conditions on both sides to choose the safest direction to avoid it.

In real life, this technology is extremely useful. For example, sweeping robots won't knock over your slippers, autonomous vehicles can avoid pedestrians who suddenly rush out, and even automated guided vehicles in factories can safely shuttle between shelves. Today, we are going to combine the motor drive (controlling wheel rotation), servo control (controlling neck rotation), and ultrasonic ranging (measuring distance) learned previously to turn the smart car into a clever "explorer."

After completing this lesson, you will master how to make the car make judgments based on distance: go straight if far away, stop, observe left and right, and turn if close. This is not just programming; it is teaching machines the ability to "think." Are you ready? Let's start giving wisdom to the smart car!

## 4.12.2 Course Objectives

*   Be able to correctly connect the **ultrasonic sensor**, **servo** (a small motor that controls angles), and **motor driver module** (a high-power chip that controls wheel rotation) to the **ESP32S3 Pro development board** (the "core brain" of our car, a small circuit board integrating a microcontroller and various interfaces), and understand the function of each **pin** (small metal holes or rows of metal on the chip or module used to connect wires).
*   Be able to write code to read data from the ultrasonic sensor and master the method of using the servo to drive the sensor for left-to-right scanning and distance measurement.
*   Be able to combine motor control logic to implement a complete smart obstacle avoidance process: "Detect obstacle -> Stop -> Servo scans left and right -> Compare distances -> Turn to avoid obstacle -> Continue moving forward".
*   Learn to use `if...else` (if...otherwise...) conditional statements, which are like answering multiple-choice questions, allowing the program to make different responses based on different distances and left/right road conditions.

## 4.12.3 Course Equipment

| Name               | Specification/Model           | Quantity | Remarks (Layman's explanation)                               |
| :----------------- | :---------------------------- | :------- | :----------------------------------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board | 1        | The core brain, responsible for processing data and sending commands. |
| Ultrasonic Module  | HC-SR04                       | 1        | The "eyes" of the car, used to emit and receive ultrasonic waves to measure the distance ahead. |
| Micro Servo        | SG90                          | 1        | The "neck" of the car, which can accurately rotate angles, carrying the ultrasonic module to look left and right. |
| Expansion Board    | ESP32S3 Pro Expansion Board   | 1        | Integrates motor driving functions, making it convenient for us to directly plug in wires to control the four wheels. |

## 4.12.4 Course Principles

**1. Servo Pan-Tilt: The Car's "Flexible Neck"**
If the ultrasonic sensor is fixed, the car can only see directly ahead. When it encounters an obstacle, it doesn't know whether the left or right side is more open. By introducing a **Servo**, we can mount the ultrasonic sensor on the servo turntable. When the car detects an obstacle ahead, it controls the servo to turn to the left (180 degrees) and right (0 degrees) respectively to measure distances, thereby obtaining environmental information on both the left and right sides, providing data support for intelligent steering.

**2. Programming Logic: How the Car's "Brain" Thinks**
We need to use `if...else` (if...otherwise...) statements, which are like making a choice:

*   **If (if)** the distance ahead is greater than the safety threshold (e.g., 15 centimeters, safe), then both motors rotate forward (move straight).
*   **Otherwise (else)** (the distance ahead is less than or equal to 15 centimeters, danger), then:
    1. Stop the motors (brake).
    2. Control the servo to turn to 180 degrees and measure the distance on the left.
    3. Control the servo to turn to 0 degrees and measure the distance on the right.
    4. Return the servo to the center (90 degrees), resuming a straight-ahead view.
    5. Compare the left and right distances: if the left distance is greater than the right, turn left; otherwise, turn right.
    6. Stop after turning for a certain period of time and wait for the next detection.

## 4.12.5 Wiring Instructions

Wiring is the most critical step in the car experiment. Because there are many wires, it is easy to make mistakes. Please disconnect the battery first to ensure safety first! We will divide the wiring into three parts: the sensor part, the servo part, and the motor part.

⚠️ **Note**: The following wiring pins are completely consistent with the definitions in the sample code. If you change the wiring pins in actual operation, please be sure to update the pin definitions in the code accordingly.

**Part 1: Ultrasonic Sensor (HC-SR04)**
The ultrasonic module has 4 pins. The HC-SR04 operates at 5V, while the ESP32's **GPIO** (General Purpose Input/Output pins, which are those metal holes on the edge of the development board) are at 3.3V logic level. For stability, we directly use the 5V power supply from the development board, and connect the signal lines directly to the GPIO.

| Component Pin | ESP32S3 Pro Development Board Pin | Description (Why it is connected this way)                   |
| :------------ | :-------------------------------- | :----------------------------------------------------------- |
| VCC           | 5V                                | **VCC** stands for positive power supply, providing 5V power to "feed" the module. |
| GND           | GND                               | **GND** stands for ground (power negative), forming a current loop. |
| Trig          | IO13                              | **Trig** (Trigger) pin, controlled by code to emit sound waves. |
| Echo          | IO12                              | **Echo** (Receive) pin, receives the returned signal and tells the brain. |

**Part 2: Servo (SG90)**
The servo has 3 wires, usually colored brown/red/orange or black/red/yellow.

| Component Pin (Wire Color) | ESP32S3 Pro Development Board Pin | Description (Why it is connected this way)                   |
| :------------------------- | :-------------------------------- | :----------------------------------------------------------- |
| Brown/Black (GND)          | GND                               | Ground (negative), must be connected to the power negative.  |
| Red (VCC)                  | 5V (or VIN)                       | Provides 5V power. The servo requires significant force and needs sufficient power. |
| Orange/Yellow (Signal)     | IO42                              | **PWM** (Pulse Width Modulation) signal control line, receives rotation commands. |

⚠️ **Important Notes (Must Read for Beginners):**

1. **Common Ground Principle**: The negative terminal of the battery, the GND of the development board, and the GND of the motor driver must be connected together, otherwise the car will move randomly or fail to work. **Why?** Because all circuits require a common "reference zero point"; without a common ground, signals will fly around randomly.
2. **Power Separation**: Motors and servos consume a lot of power. Please use a battery to directly power the motor driver and servo (VIN or external interface), and use USB only to supply power to the development board and download programs. **Why?** If the motor and development board share USB power, the voltage will drop instantly whenever the motor rotates, causing the development board to reboot.
3. **Check Polarity**: If the car moves forward instead of backward, simply swap the two wires of the corresponding motor without changing the code. **Why?** The rotation direction of the motor is determined by the current direction, and swapping the wires reverses the current direction.

## 4.12.6 Sample Code

This code makes the car continuously detect the distance ahead. If the distance is greater than 15 centimeters, the car moves straight; if it is less than 15 centimeters, the car stops, turns its head left and right using the servo to measure the distances on both sides, and then turns toward the more open side to avoid obstacles.

```python
# Import the car control class from the ESP32S3_4WD_Car library, equivalent to hiring an "assistant" who understands car control
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the time module to pause the program (delay), just like humans need time to react
import time

# Define the pin numbers for the ultrasonic sensor for easy future reference
TRIG_PIN = 13   # Trigger pin connected to IO13, telling the sensor to "emit"
ECHO_PIN = 12   # Echo pin connected to IO12, receiving the sensor's "returned" signal

# Create a car object named car, through which we will control the car later
car = Keyes_ESP32S3_4WD()

# Initialize the ultrasonic sensor, telling it which pins Trig and Echo are connected to
car.Ultrasonic_init(TRIG_PIN,ECHO_PIN)

# Define the pin numbers of the motor driver to control the left and right wheels
MOTOR_AIN = 40  # Channel A direction control pin (controls left wheel forward or reverse rotation)
MOTOR_AEN = 41  # Channel A PWM control pin (controls left wheel rotation speed)
MOTOR_BIN = 38  # Channel B direction control pin (controls right wheel forward or reverse rotation)
MOTOR_BEN = 21  # Channel B PWM control pin (controls right wheel rotation speed)

# Initialize the motors, telling the car assistant the 4 pins defined above
car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)

# Define the basic driving speed parameter, ranging from 0 (stop) to 255 (full speed), set to 220 here for stability
BASE_SPEED = 220  # Basic driving speed (0-255)

# Stop the motors in the initial state to prevent the car from running around as soon as power is turned on
car.stop_motor()

# Define the pin number for servo control
SERVO_PIN = 42    # Servo signal line connected to IO42

# Initialize the servo, telling it which pin the signal line is connected to
car.Servo_init(SERVO_PIN)

# Initialize the servo to the 90-degree position, meaning the "neck" faces straight ahead
car.Servo_set_angle(90)
# Wait for 1 second to give the servo enough time to turn to 90 degrees and stabilize
time.sleep(1)

# Main loop: while True means "infinite loop", making the car constantly repeat the following actions
while True:
    # Measure the distance ahead and assign the result to the variable distance
    distance = car.Ultrasonic_measure_distance()

    # Print the result via the serial port (the communication channel between computer and development board) for easy viewing on the computer
    print("Distance:", distance, "cm")
    
    # Conditional check: if the distance ahead is less than or equal to 15 centimeters (danger encountered)
    if distance <= 15 :
        # Stop the motors immediately, apply brakes
        car.stop_motor()
        # Rotate the servo to 180 degrees, meaning the "neck" looks to the far left
        car.Servo_set_angle(180)
        # Wait for 0.3 seconds to let the servo reach position and stabilize, otherwise distance measurement will be inaccurate
        time.sleep(0.3)
        # Measure the distance on the left and save it in the leftDistance variable
        leftDistance = car.Ultrasonic_measure_distance()
        # Wait a little for 0.1 seconds to give the sensor a breathing interval
        time.sleep(0.1)

        # Rotate the servo to 0 degrees, meaning the "neck" looks to the far right
        car.Servo_set_angle(0)
        # Wait for 0.3 seconds to let the servo reach position and stabilize
        time.sleep(0.3)
        # Measure the distance on the right and save it in the rightDistance variable
        rightDistance = car.Ultrasonic_measure_distance()
        # Wait a little for 0.1 seconds
        time.sleep(0.1)
        
        # Rotate the servo to 90 degrees, returning the "neck" to the middle position facing straight ahead
        car.Servo_set_angle(90)
        
        # Conditional check: if the left distance is greater than the right distance (indicating the left side is more open)
        if leftDistance > rightDistance :
            # Control the car to turn left (left wheel moves backward, right wheel moves forward, speed is BASE_SPEED)
            car.left(BASE_SPEED, BASE_SPEED)
        # Otherwise (indicating the right side is more open, or both sides are equal)
        else :
            # Control the car to turn right (left wheel moves forward, right wheel moves backward, speed is BASE_SPEED)
            car.right(BASE_SPEED, BASE_SPEED)
        # Continue turning for 0.5 seconds, turning just about enough is fine
        time.sleep(0.5)
    # Otherwise (if the distance ahead is greater than 15 centimeters, very safe)
    else :
        # Control the car to move straight ahead (both left and right wheels rotate forward, speed is BASE_SPEED)
        car.forward(BASE_SPEED, BASE_SPEED)
    # After each loop ends, rest for 0.1 seconds to prevent the program from running too fast and leaving the sensor no time to react
    time.sleep(0.1)

```

## 4.12.7 Code Explanation

1. **Import and Initialization Phase**:
   * Just like preparing ingredients and tools before cooking, at the beginning of the code we first `import` the car control library and time library.
   * Next, we define the **pin numbers** for various hardware connections (such as `TRIG_PIN = 13`). Defining pin numbers as variables (giving them English names) means we don't have to memorize numbers when writing code later, and it makes modifications much easier.
   * `car = Keyes_ESP32S3_4WD()` is a very crucial step. It is equivalent to waking up the "soul" of the car, allowing us to call various functions using the `car.xxx` format.

2. **Safe Startup Phase**:
   * Immediately after initializing the motors, execute `car.stop_motor()`. This is a very good programming habit! Because sometimes when code is just uploaded, motors might spin wildly due to an uncertain initial state. Braking first protects your car.

3. **Core Logic: `while True` Infinite Loop**:
   * The intelligence of the car is reflected in its ability to "observe continuously." `while True` is an infinite loop; as long as the car has power, it will repeat the process of "ranging -> judging -> acting".

4. **Obstacle Avoidance Decision Phase (`if...else`)**:
   * **Safe Condition (`else`)**: If `distance > 15`, execute `car.forward`, simple and clear.
   * **Dangerous Condition (`if distance <= 15`)**:
     * Brake first with `stop_motor()`.
     * **Why is `time.sleep(0.3)` needed?** The servo motor requires physical time to turn. If the code makes it turn to 180 degrees and measure distance immediately, the servo might still be moving halfway, and the distance measured by the ultrasound will be wrong. Therefore, it must wait to "settle down."
     * **Turning Logic**: Compare `leftDistance` and `rightDistance`. Whichever is larger indicates that the obstacle is farther away and there is more space, so the car turns toward that side (`car.left` or `car.right`).

## 4.12.8 Experimental Phenomenon

Upload the code to the ESP32S3 Pro development board, connect the motor power supply, and place the car on a flat floor.

When you block the front of the car with your hand or a book at a distance of 15 cm:

*   The car will immediately stop moving forward.
*   You will see the ultrasonic module mounted on the servo first turn to the left (180 degrees) and pause briefly; then turn to the right (0 degrees) and pause briefly; and finally return to the front center (90 degrees).
*   Based on the left and right distance measurement results, the car will turn left or right (the left and right wheels rotate in reverse directions).
*   The turn lasts for about 0.5 seconds and then stops, after which it continues to detect the distance ahead.
*   If you remove your hand and the space ahead is clear, the car will attempt to move straight forward again.

Congratulations! You have just controlled real hardware with code, giving it "vision," a "flexibly rotating neck," and "intelligent decision-making capabilities"!

## 4.12.9 Common Errors and Solutions

It is completely normal for beginners to encounter problems during the building process! Do not lose heart; we have listed the most common pitfalls and solutions:

**Problem 1: Serial monitor shows Distance: 0 cm, but there is clearly something in front**

*   **Cause**: The wiring may be loose, or Trig/Echo are connected backward. It could also be environmental noise interference, or the obstacle surface absorbs sound (such as soft sponge or clothing).
*   **Solution**: Check whether IO13 and IO12 are connected correctly. Make sure the ultrasonic module is facing a hard, flat obstacle (like a wall, cardboard). Try testing with a different angle or a different background.

**Problem 2: The car keeps spinning in circles and does not go straight**

*   **Cause**: The wiring logic of the left and right motors is inconsistent, or one of the motor wires is connected backward.
*   **Solution**: Observe the `forward` function logic in the code to ensure that the control signals for both left and right wheels make them rotate forward. If you are not sure, test each motor individually, adjust the jumper wires (wires used to connect pins) until both wheels rotate forward.

**Problem 3: The car does not stop when encountering an obstacle, and crashes straight into it**

*   **Cause**: The safe distance threshold (`15` in the code, often called `closeDistance` in the original C++ version) is set too small, or the delay time (`time.sleep` in the code, called `delay` in the original) is too short, causing it to react too slowly.
*   **Solution**: Change `15` in the code to `20` or `30` to increase reaction space. Check the battery power; low battery will cause motor torque (turning force) to drop, resulting in untimely braking.

**Problem 4: The servo only jitters and does not rotate, or the rotation angle is incorrect**

*   **Cause**: Insufficient power supply for the servo, or the pulse width (signal parameters controlling the servo rotation angle) does not match.
*   **Solution**: Make sure the red wire of the servo is connected to 5V or VIN, never to the 3.3V pin. *(Note: `myServo.attach(servoPin, 500, 2400);` mentioned in the original text is written in C/C++ language. If you are using the matching Python version, please check the parameters of `Servo_init`; if you are using the C++ version, please modify the pulse width parameters here, such as changing to 1000, 2000).*

**Problem 5: Error "Connection Error" reported when uploading code**

*   **Cause**: The ESP32S3 Pro may need to enter download mode, or the USB cable can only charge and cannot transmit data.
*   **Solution**: Press and hold the `BOOT` button on the board (download mode button), click upload, and release it after seeing the prompt. Make sure you use a high-quality data cable (one that can transmit data, not a pure charging cable).

**Problem 6: Beginner-exclusive trap — Code not saved before uploading / Wrong development board selected**

*   **Cause**: Modified the code and clicked upload directly, or selected the wrong development board model in the software.
*   **Solution**: Form a habit before uploading: press `Ctrl+S` to save the code first. In the "Tools" menu of the software, confirm that the selected board is "ESP32S3 Dev Module" or the correct corresponding model.

### ⚠️ Safety Tips

* **Strictly Prohibit Short Circuits**: Never let the positive and negative terminals of the battery touch directly, and do not short-circuit the output end of the motor driver. This will instantly burn out components, cause the battery to heat up, or even cause danger.
* **Lift Testing**: When debugging the motor direction and servo angle, it is recommended to prop up the car (e.g., place it on a paper box) so that the wheels are suspended in the air, preventing the car from running around randomly, damaging items, or hurting people.

---

*Learning embedded development is like playing with Lego. At first, you might not find the parts or put them together wrong, but as long as you patiently follow the tutorial and try a few more times, you will definitely build the coolest projects! Keep it up!*