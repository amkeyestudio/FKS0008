# 4.7 Motor Control and Speed Regulation

## 4.7.1 Course Introduction

Imagine you are playing with a remote-controlled race car, or watching a sweeping robot at home automatically clean the floor. Why can they run around? And why can they go as fast or as slow as they want, and move forward or backward at will? The secret lies in the fact that they contain a "Hercules" and a "Commander" inside. This "Hercules" is the **DC Motor (a small motor that rotates on direct current, the core power component in toy cars)**, and the "Commander" is the **DRV8835 Motor Driver Module (a circuit board that can amplify current and control motor direction)** that we are going to learn about today.

In this lesson, we will no longer just light up small LEDs, but make real motors spin! We will learn how to connect a powerful motor driver module to the **ESP32S3 Pro Development Board (our core brain, a circuit board with a microcontroller chip)**, and use code to control the rotation direction and speed of the motor. This is like learning to ride a bicycle for the first time—starting from simple balance to freely controlling direction, speed, and pacing.

After completing this lesson, you will master the core skills of controlling mechanical motion. In the future, if you want to build an automatic plant-watering cart, a dancing robot, or a smart fan, you will need the knowledge learned today. Are you ready? Let's wake up the sleeping motor together!

## 4.7.2 Course Objectives

*   Correctly connect the DRV8835 motor driver module to the ESP32S3 Pro development board, DC motor, and external power supply.
*   Understand the basic concept of the **H-Bridge circuit (a circuit composed of four electronic switches that can change the direction of current)**, and know how to control the forward and reverse rotation of the motor by changing the **pin level (the voltage state output by a pin, where high level represents power present like 3.3V, and low level represents no power like 0V)**.
*   Master the method of using **PWM (Pulse Width Modulation, a technique that simulates different voltages by rapidly switching the power supply on and off)** technology to regulate motor speed.
*   Write a program to implement a complete cycle of motor "smooth acceleration -> full-speed operation -> smooth deceleration to a stop -> reverse acceleration -> stop", and understand the principle of **differential steering (steering a car by making the left and right wheels rotate at different speeds or directions, just like a tank)**.

## 4.7.3 Course Equipment

Before we start building, let's take inventory of today's "gear". Don't worry, every item has its unique role:

| Name                         | Specification/Model                                     | Quantity | Remarks                                                      |
| :--------------------------- | :------------------------------------------------------ | :------- | :----------------------------------------------------------- |
| Main Control Board           | ESP32S3 Pro Development Board                           | 1        | Our core brain, responsible for sending control commands (like the human brain, responsible for thinking and issuing orders) |
| Motor Driver Expansion Board | DRV8835 Motor Driver Expansion Board                    | 1        | The Hercules controlling the motor, responsible for power amplification and direction switching (the brain doesn't have much strength, so it needs this helper to do the heavy lifting) |
| Battery Holder               | 6-slot AA battery holder or 2-slot 18650 battery holder | 1        | Provides sufficient external power for the motor (equivalent to the car's "lunch box", providing energy) |
| Battery (Self-provided)      | AA battery or 18650 battery                             | /        | 6 AA batteries or 2 18650 batteries are required. Choose according to the situation; it is recommended to prioritize 18650 batteries to provide a more stable high current |

## 4.7.4 Course Principles

### 4.7.4.1 Why Do We Need a Motor Driver Module?

You might ask: "The ESP32S3 Pro development board itself can output electricity, why can't we connect the motor directly to it?"

This is like trying to push a heavy truck: although you are trying very hard (the ESP32S3 outputs a 3.3V voltage), your strength is too small (the **pin (the small metal interface on the chip used to connect wires)** output current is only a few milliamperes), making it impossible to move. At this point, you need a helper named DRV8835, the "Hercules". The DRV8835 can receive the weak command signals sent by the ESP32S3, and then use an external high-power supply to output a strong current to drive the motor.

In addition, connecting a motor directly has a fatal danger: the motor generates **back electromotive force (a momentary reverse high voltage generated like a generator when the motor rotates)** during startup, shutdown, or direction change, which can directly burn out the fragile internal pins of the ESP32S3 chip. The DRV8835 acts like a firewall, isolating strong and weak currents to protect our brain.

### 4.7.4.2 H-Bridge: The Secret to Motor Forward and Reverse Rotation

A DC motor is very simple: connect the positive terminal to a high level and the negative terminal to a low level, and it rotates forward; connect it the other way around, and it reverses. But we can't manually plug and unplug wires every time!

The DRV8835 has a circuit structure inside called an "**H-Bridge**". You can imagine it as an "H"-shaped bridge composed of four electronic switches. By controlling the opening and closing of these four switches, we can change the direction of current flowing through the motor, thereby making the motor rotate forward or backward without changing wires.

In the DRV8835 module (IN/EN mode) used in this tutorial, the control logic is as follows:

*   **IN pin is high level (powered), EN pin inputs PWM (rapidly switching pulses)**: Current flows forward, and the motor rotates forward.
*   **IN pin is low level (no power), EN pin inputs PWM**: Current flows in reverse, and the motor reverses.
*   **EN pin is low level (or IN is low and EN is low)**: Regardless of the state of IN, the motor stops rotating (in a high-impedance or braking state, equivalent to power-off braking).

### 4.7.4.3 PWM Speed Regulation: Controlling Speed Like a Faucet

Now that the direction is resolved, how do we control the speed? We cannot directly change the size of the 3.3V voltage output by the ESP32S3, but we can use **PWM (Pulse Width Modulation)** technology.

Imagine controlling the water flow of a faucet: if you open the faucet completely, the water flow is maximum; if you open it just a little bit, the water flow is small. PWM is like making the faucet continuously "open-close-open-close" at an extremely fast speed.

*   When the "open" time (high-level time) accounts for a large proportion of the entire cycle, the equivalent output voltage is high, and the motor spins fast.
*   When the "open" time proportion is very small, the equivalent output voltage is low, and the motor spins slowly.

This ratio of "open time to total time" is called the **Duty Cycle**. In code, we usually map the duty cycle to a value from 0 to 255 (or 0 to 65.535), where 0 represents completely closed (stopped) and 255 represents completely open (full speed). Because the switching speed is extremely fast (usually several thousand **Hertz (Hz, the unit of frequency, referring to the number of switches per second)**), the motor does not perceive the flickering due to inertia, and will only exhibit smooth changes in rotational speed.

## 4.7.5 Wiring Instructions

Wiring is the most critical step in the experiment, so please check carefully. There are many interfaces on the DRV8835 module, and we need to connect the control signals, power supply, and motor correctly.

**Why should you pay attention to these concepts?**

1.  **Independent Power Supply**: The DRV8835 requires an external power supply (battery box) because motor driving demands a large current that the development board's USB power supply can never handle (the USB current is too small and cannot drive large motors).
2.  **Signal Connection**: The ESP32S3 sends control signals to the input pins of the DRV8835 (AIN, AEN, BIN, BEN) to tell it how to move.
3.  **Load Connection**: The output pins of the DRV8835 (AO1/AO2, BO1/BO2) connect to the DC motors (**load (the device being controlled)**), converting electrical energy into mechanical energy.
4.  **Common Ground Principle (Extremely Important)**: The GND of the ESP32S3 must be connected together with the GND of the DRV8835 and the negative terminal of the battery box (**common ground (connecting the negative terminals of all devices together to provide a return path for current)**); otherwise, the control signals cannot form a complete loop, and the motors will not respond at all.

**Detailed Wiring Table (Please go step-by-step, do not skip steps):**

| DRV8835 Pin | Connection Target                           | Explanation (Why wire it this way)                           |
| :---------- | :------------------------------------------ | :----------------------------------------------------------- |
| **VMOT**    | Battery box positive terminal (VCC / 5V-9V) | Motor driver power input to provide power. Do not reverse the polarity, or it will burn out the module. |
| **GND**     | Battery box negative terminal & ESP32S3 GND | **Must share a common ground**. The reference zero potential for the entire system, allowing signals to form a return loop. |
| **AIN**     | ESP32S3 Pro development board IO40          | Motor A direction control (left wheel), receives direction commands from the brain. |
| **AEN**     | ESP32S3 Pro development board IO41          | Motor A PWM speed regulation (left wheel), receives speed commands from the brain. |
| **BIN**     | ESP32S3 Pro development board IO38          | Motor B direction control (right wheel), receives direction commands from the brain. |
| **BEN**     | ESP32S3 Pro development board IO21          | Motor B PWM speed regulation (right wheel), receives speed commands from the brain. |

**Please note that the motor driver pins are fixed and cannot be changed!!**

## 4.7.6 Example Code

This code makes the motors (or the robot's dual wheels) first smoothly accelerate forward, then run at full speed, followed by a smooth deceleration to a stop; then they accelerate in reverse, and finally demonstrate in-place differential steering, repeating this cycle.

```python
# From the file (library) named ESP32S3_4WD_Car, import the Keyes_ESP32S3_4WD toolkit, which encapsulates the complex code for controlling the robot car for us
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD

# Import the time module so that we can "pause" or "delay" the program
import time

# Create an object named car, which is equivalent to activating the imported toolkit to prepare for controlling the car
car = Keyes_ESP32S3_4WD()

# ================= Pin Definitions =================
# Next, we tell the brain (development board) which finger (pin) is responsible for which action

# Define the direction control pin for Motor A as pin 40 (controls the forward/reverse rotation of the left wheel)
MOTOR_AIN = 40  

# Define the speed control pin for Motor A as pin 41 (controls the speed of the left wheel, i.e., the PWM pin)
MOTOR_AEN = 41  

# Define the direction control pin for Motor B as pin 38 (controls the forward/reverse rotation of the right wheel)
MOTOR_BIN = 38  

# Define the speed control pin for Motor B as pin 21 (controls the speed of the right wheel, i.e., the PWM pin)
MOTOR_BEN = 21  

# Call the initialization function to pass the 4 pin numbers defined above to the car program, preparing it to use these pins
car.Motor_init(MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN)

# ================= Initialization and Main Loop =================

# When starting up, first make the motors stop to ensure safety first and prevent the car from running around randomly as soon as power is applied
car.stop_motor()

# Print a prompt message on the computer screen to tell us that the system is ready
print("Motor driver system initialization complete, starting operation!")

# This is an "infinite loop", meaning as long as power is not cut off, the indented code below will repeat continuously
while True:
    
    # 1. Move forward at full speed for 2 seconds
    # Print "Forward" on the screen so we can observe which step the program is currently running
    print("Forward")
    
    # Call the forward function, where the two 255s represent that both left and right wheels output maximum speed (255 is full speed)
    car.forward(255, 255)
    
    # Pause (rest) the program for 2 seconds, keeping the car moving forward for 2 seconds
    time.sleep(2)

    # 2. Move backward at full speed for 2 seconds
    # Prompt "Backward" on the screen
    print("Backward")
    
    # Call the backward function, with both left and right wheels reversing at maximum speed
    car.back(255, 255)
    
    # Keep the backward state for 2 seconds
    time.sleep(2)

    # 3. Turn left in place for 2 seconds
    # Prompt "Turn Left" on the screen
    print("Turn Left")
    
    # Call the left turn function (usually the left wheel goes backward and the right wheel goes forward to achieve in-place spinning)
    car.left(255, 255)
    
    # Keep the left turn state for 2 seconds
    time.sleep(2)

    # 4. Turn right in place for 2 seconds
    # Prompt "Turn Right" on the screen
    print("Turn Right")
    
    # Call the right turn function (usually the left wheel goes forward and the right wheel goes backward)
    car.right(255, 255)
    
    # Keep the right turn state for 2 seconds
    time.sleep(2)

    # 5. Stop for 2 seconds, preparing for the next cycle
    # Prompt "Stop" on the screen
    print("Stop")
    
    # Call the stop function to cut power to all motors and stop them
    car.stop_motor()
    
    # Rest in place for 2 seconds, then return to the beginning of the while True loop to start the next round of actions
    time.sleep(2)
```

## 4.7.7 Code Explanation

Let's break down this code to see how it commands our "mighty worker." Don't be intimidated by the length of the code; let's divide it into a few chunks:

1.  **The "Call for Helper" Stage (Import and Initialization)**:
    *   `from ... import ...` and `car = ...`: Just like buying a set of Lego blocks, these two steps involve taking the blocks out of the box and assembling the base. We bring in a specialized "toolkit" to control this car and name it `car`.
2.  **The "Recognize Fingers" Stage (Pin Definitions)**:
    *   `MOTOR_AIN = 40` and the other four lines: The brain (development board) has many "fingers" (pins), and we must clearly tell it that finger number 40 is responsible for the left wheel's direction, and 41 is responsible for the left wheel's speed. This prevents it from giving messy commands.
    *   `car.Motor_init(...)`: Formally hands over the list of recognized fingers to the car program, completing "pre-job training."
3.  **The "Safe Start" Stage (Initial State)**:
    *   `car.stop_motor()`: Hits the brakes right at the beginning. Why? Because just in case the motors were still spinning when the power was last cut off, or to prevent the car from suddenly darting forward and startling you when power is first applied, stopping first is the safest option.
    *   `print(...)` : Broadcasts a message on the computer telling you "I am ready."
4.  **The "Infinite Loop" Stage (Main Loop)**:
    *   `while True:`: This is a magic instruction meaning "repeat the things below forever." As long as the battery has power, the car will continuously loop through this set of actions.
    *   `car.forward(255, 255)`: `255` is the maximum value for speed (like pressing the gas pedal all the way down). This line of code makes the left and right wheels move forward at full speed.

    *   `time.sleep(2)`: Let time "fly" for 2 seconds. During these 2 seconds, the car keeps its current action unchanged.
    *   The subsequent moving backward, turning left, turning right, and stopping follow the exact same logic: perform an action first, then maintain it for a period of time. By changing the combinations of speed and direction of the left and right wheels, various fancy actions are achieved!

## 4.8 Experimental Phenomena

After uploading the code and connecting the external power supply, please observe your motor (or car):

1.  **Smooth Forward Movement**: After receiving the command, the motor starts to accelerate very smoothly from a standstill and rotates (forward). You will hear the motor sound gradually getting louder. After the acceleration is complete, it maintains full-speed rotation for about 1 second.
2.  **Smooth Backward Movement**: Next, the motor smoothly decelerates until it stops, then seamlessly starts smooth reverse acceleration, backing up at full speed for about 0.6 seconds.
3.  **Differential Steering**: Subsequently, the car demonstrates turning left in place (left wheel reverse, right wheel forward) for 2 seconds, followed by turning right in place for 2 seconds. At this point, the car rotates rapidly around its center point.
4.  **Cyclic Pause**: After completing all actions, the motor completely stops for 2 seconds, and then repeats the entire process described above.

If you open the **Serial Monitor (a window on the computer used to display text information sent back by the development board)** (with the **Baud Rate (speed of data transmission)** set to 115200), you will see the program print out current status prompts such as "Forward", "Backward", "Turn Left", etc., which helps you confirm which step the program is currently running.

**Awesome! If you see the motor running smoothly at this rhythm, congratulations! You have successfully controlled real mechanical power!**

## 4.9 Common Errors and Solutions (Beginner's Pitfall Guide)

As a beginner starting out, it is inevitable to encounter some minor situations. Don't panic; the traps that everyone is most likely to step into and their solutions are listed here:

**Problem 1: The motor only hums, but does not rotate.**

*   **Cause**: It may be due to insufficient power supply. The current provided by the USB may not be enough to drive the motor to start, especially at low speeds where the torque (rotational force) is smaller, or the battery power is exhausted.
*   **解决方法**: Try using brand new or fully charged batteries connected to the VMOT input of the DRV8835, and ensure that GND shares a strict common ground with the ESP32S3. Also, check if the motor is physically stuck (stalled).

**Problem 2: The motor rotation direction is opposite to expectations (e.g., the code writes forward, but it actually goes backward).**

*   **Cause**: The two wires of the motor output terminals (AO1/AO2 or BO1/BO2) are wired backwards during connection, or the logic of AIN/BIN in the code does not match your hardware.
*   **Solution**: The easiest method that does not require modifying the code is: swap the two motor wires connected to the corresponding output terminals of the DRV8835.

**Problem 3: The motor can only rotate at full speed and cannot control speed, or the speed does not change with the code.**

*   **Cause**: The AEN/BEN pins are not connected correctly, or the value of `duty_u16` in the code is always restricted to the maximum value.
*   **Solution**: Check the DRV8835 module to ensure that the jumper wires for the AEN and BEN pins are correctly connected to IO41 and IO21 of the ESP32S3 (must be pins that support PWM) and are not loose.

**Problem 4: Errors occur when uploading code, or the motor does not move, and the serial port has no output.**

*   **Cause**: Pin definition error, development board not correctly recognized, or MicroPython firmware not running properly.
*   **Solution**: Check whether the pin numbers in the code match the wiring table (IO40, IO41, IO38, IO21). Make sure the correct serial port is selected in the MicroPython environment such as Thonny IDE, and that the MicroPython firmware has been successfully flashed onto the development board.

**Problem 5: Dupont wires plugged into the wrong holes or not plugged in tightly (most common for beginners!).**

*   **Cause**: Dupont wires were plugged into adjacent holes, or although plugged in, they did not make contact with the metal pieces.
*   **Solution**: Trace back along the lines to check, ensuring that the plug of every wire is firmly inserted into the correct pin hole. Unplug and re-plug it once, applying a little force.

**Problem 6: Battery box switch is not turned on, or batteries are installed backward / out of power.**

*   **Cause**: Forgot to turn on the physical small switch on the battery box, or installed the battery positive and negative poles backward leading to a lack of power.
*   **Solution**: Check whether the battery box switch is flipped to "ON", use a multimeter or the substitution method to confirm whether the batteries have power, and ensure the positive and negative poles are installed correctly.

## 4.10 Precautions and Safety Tips

Safety always comes first! Before letting the car "speed up", please make sure to keep the following points in mind:

*   **Watch out for heat**: The DRV8835 may heat up when driving high-current loads, which is normal. However, if it is scalding hot (exceeding 60°C), please cut off the power immediately and check if the load is too heavy, if the motor is stalled, or if heat dissipation is poor.
*   **Prevent stalling (motor powered on but forcibly held by external force preventing rotation)**: Try to avoid keeping the motor in a stalled state for a long time. When stalled, the current will reach its maximum value, which can easily burn out the motor coils or the driver module. If you find that the car is stuck, turn off the power immediately!

---

*Congratulations on completing this lesson's study! From lighting up a single LED to driving a motor, you have taken a huge step forward in embedded development. Keep your curiosity and continue exploring!*