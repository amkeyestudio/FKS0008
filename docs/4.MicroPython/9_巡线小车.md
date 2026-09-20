# 4.9 Line Tracking Smart Car

## 4.9.1 Course Introduction

Imagine you are driving a super smart racing car, but this racing car doesn't require you to hold a steering wheel—it can steadily run along the black line on the ground all by itself! Does this sound like magic? Actually, no, this is the credit of "machine vision" (allowing machines to understand images like humans) and "logical judgment" (deciding what to do next based on what is seen). In real life, automated guided vehicles in factories and automated guided carts in supermarkets operate on similar principles. They look at the road through "eyes" (**sensors**: electronic components that can perceive the surrounding environment and convert it into electrical signals), think with a "brain" (**ESP32S3 Pro development board**: a small computer integrating a microcontroller and various interfaces), and then command the "legs" (motors) to act.

Today, we are going to string together all the knowledge we've learned before! We will use the motor driver learned in Lesson 6 and the 5-channel line tracking sensor learned in Lesson 7. We will write a smart piece of code to turn the car into a "track detective," constantly staring at the black line on the ground. If it drifts to the left, we make it turn right; if it drifts to the right, we make it turn left.

After completing this lesson, you will not only be able to make the car move, but also make it "intelligent." Congratulations, you are about to make a huge leap from "controlling a single part" to "building a complete smart system"! Ready to witness the magic? Let's get started!

## 4.9.2 Lesson Objectives

*   Correctly connect the 5-channel line tracking sensor and motor driver module to the ESP32S3 Pro development board (**Development Board**: Our core control board featuring many **pins**, which are metal interfaces used to connect various electronic components).
*   Understand the role of "if... then..." logical judgments in line tracking (e.g., if the left side sees a black line, turn right).
*   Write and upload code to make the car automatically and smoothly drive along a black track.
*   Learn to adjust the sensitivity of the car by observing data from the **Serial Monitor** (a "dialogue" window on the computer used to display text information sent back by the car).

## 4.9.3 Lesson Equipment

| Component Name        | Specification/Model                     | Qty  | Notes (Layman's Explanation)                                 |
| :-------------------- | :-------------------------------------- | :--- | :----------------------------------------------------------- |
| Main Control Board    | ESP32S3 Pro Development Board           | 1    | The core brain responsible for thinking and issuing commands |
| Line Tracking Sensor  | 5-Channel Infrared Line Tracking Module | 1    | The car's "eyes," used to recognize black and white lines on the ground |
| Expansion Board       | ESP32S3 Pro Expansion Board             | 1    | An "assistant" plugged into the development board with a built-in motor driver chip for easy wiring |
| DC Geared Motor       | TT Motor                                | 4    | Equipped with wheels, responsible for converting electrical energy into mechanical energy to make the car run |
| External Power Supply | 18650 Battery or AA Battery Box         | 1    | Provides electrical power, equivalent to the car's "lunchbox" |
| Line Tracking Map     | Black Electrical Tape + White Paper     | 1    | A simple black track providing a driving route for the car   |

## 4.9.4 Lesson Principles

### 4.9.4.1 How Do the "Eyes" of the Line Tracking Sensor See?

Remember the line tracking sensor we learned in Lesson 7? It has a row of **infrared transmitting tubes** (bulbs emitting invisible light) and **receiving tubes** (eyes receiving light) at the bottom. Simply put, it constantly asks the ground: "Are you white or black?"

*   **Encountering White Ground**: Infrared light is reflected back, the sensor receives the signal, and outputs a **High Level** (can be understood as "powered," representing the digit 1 in computers).
*   **Encountering Black Lines**: Infrared light is absorbed by the black color, very little is reflected, the sensor receives no signal, and outputs a **Low Level** (can be understood as "unpowered," representing the digit 0 in computers).

We have 5 such "eyes" arranged in a row. The middle eye is the most important because it is responsible for watching the road directly ahead.

### 4.9.4.2 How Does the "Brain" of the Car Think?

The ESP32S3 Pro development board is our commander. It checks the sensor status hundreds of times per second. Its thinking logic is very simple, just like keeping your balance when riding a bicycle:

*   **Case A**: The middle sensor sees the black line -> **Go straight** (Awesome, correct direction!)
*   **Case B**: The left sensor sees the black line -> **Turn right** (Oops, the car has drifted to the left, need to correct to the right)
*   **Case C**: The right sensor sees the black line -> **Turn left** (Oops, the car has drifted to the right, need to correct to the left)

This is called "**feedback control**" (sending the output result back to influence the current control, forming a closed loop). We don't need complex mathematical formulas; we only need simple "if... then..." logic.

### 4.9.4.3 How Do the Motors Coordinate for Steering?

To make the car turn, we don't need to turn the front wheels like a real car. We rely on "**differential steering**" (turning by running the left and right wheels at different speeds):

*   **Turn Left**: The left motor stops or slows down, and the right motor runs fast.
*   **Turn Right**: The right motor stops or slows down, and the left motor runs fast.

## 4.9.5 Wiring Instructions

Wiring is the most critical step in the car experiment because there are many wires, making it easy to make mistakes. Please **power off** the ESP32S3 Pro development board first (unplug the USB cable and battery, **Why do this? To prevent short circuits and burning out the expensive development board while wiring!**), and then carefully connect according to the steps below.

**Detailed Pin Correspondence Table**:

| 5-Channel Line Tracking Sensor | ESP32S3 Pro Development Board/Expansion Board Pin |                         Description                          |
| :----------------------------: | :-----------------------------------------------: | :----------------------------------------------------------: |
|              VCC               |                        5V                         |          Sensor power positive, supplies 5V voltage          |
|              GND               |                        GND                        | Sensor power negative, must share a common ground with the battery negative |
|              SDA               |                        IO8                        |        I2C data line, used to transmit specific data         |
|              SCL               |                        IO9                        | I2C clock line, used to synchronize the rhythm of data transmission |

## 4.9.6 Sample Program

This code makes the car continuously read the status of the 5 sensors. If the middle sensor detects a black line, the car goes straight; if the left side detects it, the car turns right to correct; if the right side detects it, the car turns left to correct.

*(Note: The original code block was marked as cpp, but the actual content is Python syntax. It has been corrected to python for proper syntax highlighting.)*

```python
# ==================== Import Modules ====================
# Take out the "remote control" (Keyes_ESP32S3_4WD) specially designed to control the car from the "toolbox" named ESP32S3_4WD_Car
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the time module to make the program "wait" (delay)
import time

# Start the "remote control" and give it a short name called car; we will use car to command it later
car = Keyes_ESP32S3_4WD()

# ==================== Pin Definitions ====================
# The line tracking sensor is connected via the PCF8574 expansion chip. Here we define the I2C (a communication protocol) pins
I2C_SCL = 9   # Set the I2C "clock line" pin to pin 9, used to synchronize the rhythm of data transmission
I2C_SDA = 8   # Set the I2C "data line" pin to pin 8, used to actually transmit data

# Tell the car that the line tracking sensor is connected to the previously defined pins 9 and 8, and initialize (start) the sensor
car.Line_init(I2C_SCL, I2C_SDA)

# Define the motor control pins (pins are the metal interfaces on the development board used to output electrical signals)
MOTOR_AIN = 40  # Define channel A's (left wheel) direction control pin as pin 40, determining whether the left wheel rotates forward or backward
MOTOR_AEN = 41  # Define channel A's (left wheel) speed control pin as pin 41, determining how fast the left wheel rotates
MOTOR_BIN = 38  # Define channel B's (right wheel) direction control pin as pin 38, determining whether the right wheel rotates forward or backward
MOTOR_BEN = 21  # Define channel B's (right wheel) speed control pin as pin 21, determining how fast the right wheel rotates

# Tell the car that the motors are connected to the previously defined pins 40, 41, 38, and 21, and initialize (start) the motors
car.Motor_init(MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN)

# Define speed parameters (numerical range is 0 to 255, where 255 is the fastest and 0 is stopped)
BASE_SPEED = 220  # Set the base straight-line speed to 220, which is relatively fast yet controllable
TURN_SPEED = 220  # Set the turning speed to 220, keeping it consistent with the straight-line speed

# Stop all motors in the initial state to prevent the car from running around as soon as it is powered on
car.stop_motor()
# Print a startup message on the computer to tell us that the program has started running
print("Line Tracking Start")

# ==================== Main Loop ====================
# This is an "infinite loop", meaning as long as the car has power, the code inside will execute over and over again
while True:
    # Read the status of the 5 sensors and assign the results to A, B, C, D, E respectively (1 means seeing a black line, 0 means seeing white)
    A, B, C, D, E = car.Line_get_data()

    # Concatenate the status of A, B, C, D, E into a sentence and print it to the computer's "Serial Monitor" for easy observation
    print("ABCDE: {}{}{}{}{}".format(A, B, C, D, E))

    # Combine the 5 numbers A, B, C, D, E into a single "status code" (using bitwise operations to shift bits and assemble them)
    state = (A << 4) | (B << 3) | (C << 2) | (D << 1) | E

    # According to the combined status code, use "if... then..." logic to decide how the car should move
    if state == 0b01110:
        # If the status code is 0b01110 (meaning the middle 3 sensors B, C, D see the black line), it means the car is right in the middle
        # Make the car move straight forward at the base speed
        car.forward(BASE_SPEED, BASE_SPEED)
    elif state in (0b11100, 0b11000, 0b10000):
        # If the status code is 0b11100, 0b11000, or 0b10000 (meaning the left sensors A or B see the black line), it means the car has drifted to the left
        # Make the car turn left (left wheel speed set to BASE_SPEED, right wheel speed set to BASE_SPEED/0 depending on differential implementation, achieving a left turn via differential speed)
        car.left(BASE_SPEED, BASE_SPEED)
    elif state in (0b00111, 0b00011, 0b00001):
        # If the status code is 0b00111, 0b00011, or 0b00001 (meaning the right sensors D or E see the black line), it means the car has drifted to the right
        # Make the car turn right (achieving a right turn via differential speed)
        car.right(BASE_SPEED, BASE_SPEED)
    elif state == 0b00000:
        # If the status code is 0b00000 (meaning none of the 5 sensors see the black line), it means the car has completely deviated from the track
        # Immediately stop all motors to prevent the car from getting lost
        car.stop_motor()
    else:
        # For other complex states (such as seeing two lines at the same time), the situation is special
        # Make the car go straight at a slower speed (150) to try to slowly correct the direction
        car.forward(150, 150)

    # Pause the program for 50 milliseconds (0.05 seconds) to control the loop speed, preventing the car from reacting too quickly and oscillating (swaying left and right)
    time.sleep_ms(50)
```

## 4.9.7 Code Explanation

To help you fully understand the code above, we break down the core logic and explain it in plain terms:

1.  **Import and Initialization**: Just like getting pots and pans ready before cooking, we first `import` the dedicated module for controlling the car and instantiate the car (naming it `car`). Next, we tell the car which "pins" (metal interfaces) the sensors and motors are plugged into.
2.  **Main Loop `while True:`**: This is a loop that is always true. As long as the car is not turned off, this code will execute several times a second like a heartbeat.
3.  **Reading Data `car.Line_get_data()`**: In each cycle, the car asks the sensors: "What do the 5 of you see right now?" The sensors reply with 5 numbers (0 or 1).
4.  **State Code Combination (Bitwise Operations)**:
    *   The `<<` in the code is the "left shift" operator, and `|` is the "bitwise OR" operator. Sounds intimidating? It's actually very simple!
    *   Suppose the sensors return `A=0, B=1, C=1, D=1, E=0`.
    *   The purpose of this line of code is to string these 5 independent numbers together into a 5-digit binary code, much like skewering candied haws: `01110`.
    *   This way, the car only needs to evaluate this single "state code" to know exactly where the black line is.
5.  **Logic Evaluation `if...elif...else`**:
    *   If the state code is `01110` (the middle three are 1s), it means the black line is right in the center, so `car.forward` (move straight) is executed.
    *   If the state code is `11100` or similar (the left side is 1), it means the black line has drifted to the left, so `car.left` (turn left to correct) is executed.
    *   If the state code is `00000` (all 0s), it means the line is completely out of view, so `car.stop_motor` (stop to stay safe) is executed.
6.  **Delay `time.sleep_ms(200)`**: After each loop execution, the program rests for 200 milliseconds. **Why?** If the car reacts too quickly and makes sharp steering adjustments the moment it deviates slightly, it will weave back and forth across the road like a novice driver. Adding a little delay makes the car's movements smoother.

## 4.9.8 Experimental Phenomenon

When you upload the code to the ESP32S3 Pro development board and place the car on a white sheet of paper with a black tape track:

1.  **Initial State**: The car should remain stationary, or the motors should have a slight holding torque.
2.  **Placed on the Line**: When you align the middle sensor of the car with the black line, the car should start **moving straight forward**.
3.  **Deviation Correction**:
    *   If you intentionally push the car slightly to the right so that the black line appears under the left sensors, the car should immediately **turn right** in an attempt to find the black line again.
    *   Conversely, if the black line appears on the right, the car will **turn left**.
4.  **Success Indicator**: The car can travel continuously along the curved black tape track without running off it. Even with mild curves in the track, it automatically adjusts its direction.

Open the **Serial Monitor** (set the **Baud Rate** to 115200; the **Baud Rate** is the "speaking speed" for communication between the computer and the car—both sides must use the same speed to understand each other). You will see a string of numbers jumping around, such as `01110`, which indicates that the middle sensor has detected the black line. When the car turns, you will see the position of `1` shifting to the left or right.

## 4.9.9 Common Errors and Solutions

When beginners build a line-following car for the first time, they typically stumble into the following pitfalls. Don't worry, encountering problems is a great opportunity to learn!

**Error 1: The car keeps spinning in place and won't move forward**

*   **Cause**: The wiring for the left and right motors might be reversed, or the left/right logic in the code is inverted. For example, the code says "turn left," but actually only the left wheel turns while the right wheel stays still, causing a right turn.
*   **Solution**:
    1.  Open the Serial Monitor to observe the data. When the black line is on the left (the left side of the state code is 1), check whether the car turns right.
    2.  If it turns left (reversed), swap the motor control logic inside the `car.left` and `car.right` functions in the code (swap the speed parameters for the left and right wheels).
    3.  Alternatively, physically swap the wiring of the left and right motors.

**Error 2: The car drives crookedly, dancing around (swaying wildly left and right)**

*   **Cause**: The sensitivity is too high or the speed is too fast. The car turns sharply at the slightest deviation, only to overshoot to the other side, creating an "oscillation" effect.
*   **Solution**:
    1.  Lower the value of `BASE_SPEED` in the code, for example, from 220 down to 150, to make the car run slower.
    2.  Increase the value of `time.sleep_ms()` at the end of the `while True` loop, for example, changing it from 200 to 300 or 500, to slow down the car's reaction time and give the mechanical structure a buffer.

**Error 3: The car doesn't move at all, acting like a wooden figure**

*   **Cause**: The battery is dead, the motor driver's enable pin is not connected properly, or **most crucially: lacking a common ground (GND)**.
*   **Solution**:
    1.  Check the battery voltage to ensure it has power.
    2.  **Key Check**: Is the battery negative terminal connected **simultaneously** to the motor driver GND and the ESP32S3 Pro GND? This is the most easily overlooked mistake for beginners! Without a common ground, the development board's signals cannot reach the motors.
    3.  Gently spin the wheels by hand to check for resistance, ruling out mechanical jamming.

**Error 4: Sensor readings are all 1s or all 0s, unable to distinguish black from white**

*   **Cause**: The ambient light is too bright, interfering with the infrared sensors, or the sensors are positioned too high/low from the ground.
*   **Solution**:
    1.  Adjust the potentiometer on the sensor module (if available, turn it with a Phillips screwdriver) until the indicator light switches sensitively at the black-and-white boundary.
    2.  Ensure the sensors are about 1-2 centimeters away from the ground (roughly the thickness of a finger).
    3.  Avoid testing under direct sunlight (sunlight contains strong infrared radiation); indoor lighting works best.

## 4.9.10 Safety and Precautions

*   **Anti-Entanglement Warning**: The wheels spin at high speeds when the car is running. Please keep fingers, hair, and loose clothing away from the wheel axles and motor gears.
*   **Anti-Fall Warning**: Please conduct tests on an open, flat floor to prevent the car from driving off the track, falling, and damaging the development board or sensors.
*   **Electrical Safety**: If you smell a burning odor or notice the battery or motors becoming abnormally hot, immediately disconnect the battery power!
*   **Power Management**: After the experiment, make sure to turn off the power switch on the expansion board or unplug the battery case to prevent over-discharge and battery damage.