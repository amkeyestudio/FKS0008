# 4.11 Ultrasonic Tracking Car

## 4.11.1 Course Introduction

In this lesson, we will combine the **motor driver** (the module that controls wheel rotation, acting as the smart car's "muscles") and the **ultrasonic sensor** (the car's "eyes") that we learned about previously to build a smart "following car." It can not only avoid obstacles but also closely follow an object moving in your hand. Imagine the car following you wherever you go—isn't that super cool?

After completing this lesson, you will master how to make the car automatically adjust its motion state based on distance: chase after the target if it's far away, stop if it's close, and back up to avoid it if it gets too close. Congratulations, you are about to give the car true "intelligence"! Are you ready for the challenge? Let's get started!

## 4.11.2 Lesson Objectives

*   Correctly connect the **ultrasonic sensor** to the **ESP32S3 Pro development board** (the car's "brain," a circuit board integrating a microcontroller responsible for processing all calculations and commands).
*   Write code to read distance data from the ultrasonic sensor (allowing the car to "see" distances and store them as numbers).
*   Control the car to move forward, stop, or reverse based on distance values (making the car "move" by turning the brain's thoughts into muscular actions).
*   Understand the application of **logical judgments** using "if... then..." statements (similar to multiple-choice questions in daily life: if condition A is met, do action A; if condition B is met, do action B) in robot control.

## 4.11.3 Lesson Equipment

Before we start, let's check today's "gear." Don't worry, every component has its unique role, so let's get to know them one by one:

| Component Name     | Specification/Model           | Qty  | Notes (Layman's Explanation)                                 |
| :----------------- | :---------------------------- | :--- | :----------------------------------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board | 1    | Core Brain: Responsible for receiving signals, calculating distances, and issuing commands. It has many **pins** (small metal holes or pins on a chip or module used to connect wires). |
| Ultrasonic Sensor  | HC-SR04                       | 1    | Car's "Eyes": Responsible for transmitting ultrasonic waves and receiving echoes, calculating the distance by measuring the time the sound takes to make a round trip. |
| Expansion Board    | ESP32S3 Pro Expansion Board   | 1    | Bridge: Because the interfaces (pins) on the development board are too small and dense, the expansion board enlarges and classifies the interfaces to make plugging in wires easier, acting like a "power strip" for the brain. |

## 4.11.4 Programming Logic: Simple Decision Tree

To make the car smart, it doesn't actually need to understand human language; we just need to teach it to make "multiple-choice questions." In programming, this is called a **decision tree** (a branching decision process like tree branches) or **logical judgment**, which uses "if... then..." rules:

*   **If** distance > 30 cm and ≤ 50 cm (the target is ahead and far away), **then** the car moves forward (chases it).
*   **If** distance is between 10 and 30 cm (ideal distance), **or** distance > 50 cm (exceeding the maximum following range), **then** the car stops (maintains the follow state, avoids crashing, or prevents getting lost).
*   **If** distance < 10 cm and > 0 cm (too close, about to collide), **then** the car reverses (safely avoids it).

## 4.11.5 Hardware Wiring Instructions

Wiring is the most crucial step in the experiment. Please carefully check the table below. **Why is wiring so important?** Because electricity is like water flow; if the path isn't built correctly, the water won't flow through properly, and it might even "flood" and damage the parts. To ensure safety, please unplug the USB cable of the ESP32S3 Pro development board (disconnect power) before wiring.

**Overall Concept**: The ultrasonic sensor is responsible for "seeing," the ESP32S3 Pro is responsible for "thinking," and the motor driver is responsible for "moving."

*Note: The pin numbers in the original text table differed from the code. To ensure your experiment succeeds, they have been uniformly corrected here to pins 13 and 12, which are actually used in the code.*

| Ultrasonic Sensor Pin | ESP32S3 Pro Development Board Pin | Description                                                  |
| :-------------------- | :-------------------------------- | :----------------------------------------------------------- |
| VCC                   | 5V                                | Provides 5V power. Be careful not to connect it to 3.3V, as 3.3V is insufficient, and an underpowered sensor will work unstably. |
| Trig                  | IO13                              | Trigger signal. Controlled by the development board to tell the sensor, "Please emit ultrasonic waves." |
| Echo                  | IO12                              | Echo signal. After receiving the echo, the sensor uses this to tell the development board, "I have received it." The development board calculates the distance by measuring the duration of the high level (powered state). |
| GND                   | GND                               | Ground. Forms a complete current loop; just like water needs to flow back to a pool after use, without grounding, the circuit won't work. |

**⚠️ Wiring Precautions**:

1.  **Power-Off Operation**: When plugging or unplugging **dupont wires** (plugged wires specially used for connecting electronic components), always ensure the development board is powered off. **Why?** To prevent electric sparks caused by live plugging, which can lead to a **short circuit** (current bypassing appliances and connecting directly, causing heat or even burning out components).
2.  **Direction Confirmation**: The two metal cylinders on the front of the ultrasonic sensor (like eyes) must face forward and must not be blocked by the car chassis or wires. **Why?** Because ultrasonic waves travel in straight lines, and if blocked, it cannot "see" things ahead.
3.  **Firm Connection**: Ensure that the dupont wires are plugged in tightly to prevent poor contact from causing data to jump around. **Why?** If the wires are loose, signals will become intermittent, causing the car to behave erratically and move inconsistently.

## 4.11.6 Sample Program

This code makes the car continuously detect the distance of objects in front of it. If an object is between 30 and 50 cm away, the car moves forward; if it is between 10–30 cm or greater than 50 cm, the car stops; if it is less than 10 cm, the car reverses.

Don't be intimidated by the length of the code; we've added detailed "translations" to every line so you'll understand it at a glance! (Note: The original code block was marked as `cpp`, but the actual syntax is Python. It has been corrected here to `python` to avoid misunderstandings).

```python
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD  # Import the car-controlling "expert" (class) from the dedicated library file to get ready for work
import time  # Import the time module, like equipping the car with a watch to control waiting and delays

# Define pins (give interfaces friendly names for later use)
TRIG_PIN = 13   # Define the trigger pin number as 13, telling the car that the command to "emit ultrasonic waves" is sent from interface 13
ECHO_PIN = 12   # Define the echo pin number as 12, telling the car that the command to "receive echo" is read from interface 12

car = Keyes_ESP32S3_4WD()  # Create a car object named car, equivalent to giving the car an "ID card," so we can control it using car later

car.Ultrasonic_init(TRIG_PIN,ECHO_PIN)  # Initialize the ultrasonic sensor, telling it to use interfaces 13 and 12, and getting it ready to "see" things

# Motor pins (define the interfaces that control wheel rotation)
MOTOR_AIN = 40  # Define the left wheel (Path A) direction control pin as 40, used to determine whether the left wheel rotates forward or backward
MOTOR_AEN = 41  # Define the left wheel (Path A) speed control pin as 41, used to adjust how fast the left wheel rotates (via PWM technology)
MOTOR_BIN = 38  # Define the right wheel (Path B) direction control pin as 38, used to determine whether the right wheel rotates forward or backward
MOTOR_BEN = 21  # Define the right wheel (Path B) speed control pin as 21, used to adjust how fast the right wheel rotates

car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)  # Initialize the motors by passing the 4 pins defined above to the car, getting the motor "muscles" ready for work

# Speed parameters (set how fast the car runs)
BASE_SPEED = 220  # Set the base driving speed to 220 (max is 255), making the car run relatively fast without getting out of control
TURN_SPEED = 220  # Set the turning speed to 220, keeping it the same speed as moving straight

# Stop motors in initial state
car.stop_motor()  # Make the car stop at the very beginning to ensure safety and prevent it from running around and crashing right after code upload

# Main loop (make the car repeat the following actions endlessly)
while True:  # Start an infinite loop; as long as power is on, the car will keep executing the code below
    # Measure distance
    distance = car.Ultrasonic_measure_distance()  # Have the ultrasonic sensor measure the distance once and store the result in the variable (data container) named "distance"

    # Print results via serial port
    print("Distance:", distance, "cm")  # Print the current distance value on the computer screen so we can observe what the car "sees"
    
    # Execute corresponding actions based on obstacle distance (start making choices)
    if distance >= 30 and distance <= 50:  # If the distance is greater than or equal to 30 cm and (and) less than or equal to 50 cm (target is at a proper following distance)
        car.forward(BASE_SPEED, BASE_SPEED)  # Make the car move forward at base speed to catch up with the target ahead
    elif (distance >= 10 and distance < 30) or distance > 50:  # Otherwise (elif), if the distance is between 10 and 30 cm (too close), or (or) greater than 50 cm (too far and lost track)
        car.stop_motor()  # Make the car stop moving and stand by in place
    elif distance < 10 and distance > 0:  # Otherwise, if the distance is less than 10 cm and greater than 0 (about to collide)
        car.back(BASE_SPEED, BASE_SPEED)  # Make the car reverse at base speed to safely avoid a collision
    time.sleep(0.1)  # Pause the program for 0.1 seconds (i.e., 100 milliseconds) to give the sensor a short break and make movements smoother, avoiding twitching caused by being overly sensitive
```

## 4.11.7 Detailed Code Explanation

To make sure you thoroughly understand how the smart car thinks, let's break down the code piece by piece:

1.  **"Invoking Experts" and "Getting a Watch"**:
    The first two lines of code, `from ... import ...` and `import time`, are like bringing the manual (library files) and a time manager for the smart car into our program. Without them, the car wouldn't know how to move or when to wait.
2.  **"Recognizing Interfaces" and "Getting an ID Card"**:
    Next, we use statements like `TRIG_PIN = 13` to give each interface an easy-to-remember name. Then, the line `car = Keyes_ESP32S3_4WD()` is crucial; it "clones" a virtual smart car inside the computer and names it `car`. From then on, whenever we call `car`, it knows we are talking to it.
3.  **"Initialization" (Warm-up Exercises)**:
    `car.Ultrasonic_init` and `car.Motor_init` are like having the smart car warm up before running. They tell its "eyes" (sensors) and "muscles" (motors) which interfaces they are connected to, putting them into a ready state.
4.  **"Infinite Loop" (Perpetual Motion Machine)**:
    `while True:` is a very classic structure in programming. `True` means "truth/correct", so this sentence means "as long as the condition is always true, keep looping endlessly." Because the condition is always true, the smart car will tirelessly repeat the code below until you unplug the power.
5.  **"Taking a Multiple-Choice Test" (Logical Judgment)**:
    `if` and `elif` mean "if" and "else if". The program checks the conditions one by one from top to bottom.
    *   First, look at `if`: Is the distance between 30 and 50? If so, execute `car.forward` (move forward), then skip the remaining conditions and go straight back to the beginning of the loop.
    *   If not, look at `elif`: Is the distance between 10–30, or greater than 50? If so, execute `car.stop_motor` (stop).
    *   If still not, look at the last `elif`: Is the distance less than 10? If so, execute `car.back` (move backward).
        This structure ensures that the car only executes one action at any given moment and doesn't get "schizophrenic."

## 4.11.8 Experimental Phenomena and Testing

After uploading the code, we need to check whether the car truly "sees" things.

**How to view data?** 
At the top of the programming software, find and open the **Serial Monitor** (the "dialog box" where the computer and development board chat). The **Baud rate** (the "speech speed" of the car and computer chatting) must be set to **115200** (consistent with the code, otherwise mismatched speeds are like a Chinese person listening to French—all they hear is gibberish).

You should see lines of jumping numbers displaying the current distance value, such as `当前距离: 45 cm` (Current distance: 45 cm).

**Observing the car's actions:**

1.  **Away Test**: Block 40 cm in front of the ultrasonic sensor with your hand or a book. You will find the car's two wheels rotating forward to approach you.
2.  **Follow Test**: Slowly move your hand closer. When the distance is about 15–20 cm, the car should suddenly stop and stay quietly there. If you move your hand away beyond 50 cm, the car will also stop (preventing it from running away).
3.  **Avoid Test**: Continue to bring your hand closer until the distance is less than 10 cm. The car should immediately back up, trying to increase the distance between it and you.

If the car moves smoothly without strange shaking, congratulations! You have successfully built a following smart car with preliminary artificial intelligence! Give yourself a round of applause!

## 4.11.9 Common Errors and Troubleshooting

As a beginner hitting the road, it's inevitable to fall into a few pits. Don't be discouraged; these are the necessary "trials" to becoming a master. If you encounter problems, use the "Pitfall Guide" below to troubleshoot:

**Pit 1: The distance displayed by the serial monitor is always 0 or a very large number (e.g., 3000+)**

*   **Cause**: Loose wiring may have prevented the signal from passing through; or Trig/Echo are connected backward; or there might be strong acoustic wave interference around, or something is blocking the front of the sensor.
*   **Solution**: Check whether pins 13 and 12 are connected correctly and plugged in tightly. Make sure there are no obstructions on the front of the sensor (the two metal cylinders that look like eyes). Try unplugging the USB cable and plugging it back in to restart the development board.

**Pit 2: The car only turns to one side, or spins in place**

*   **Cause**: The wiring polarity of one of the motors is reversed, or the control logic of the left and right motors is inconsistent (e.g., the left side moves forward, but the right side goes backward). In electronics, **HIGH level** (equivalent to turning on power, outputting high voltage) and **LOW level** (equivalent to grounding, outputting 0 voltage) determine the forward and reverse rotation of the motors.
*   **Solution**: Check the `forward` function in the code to ensure that the two direction pin level states of the left and right motors are consistent (both HIGH or both LOW). If a motor on one side still spins backward, the simplest physical solution is: power off, and swap the two wires connecting that side's motor to the driver module.

**Pit 3: The car's movements are sluggish or stuttering**

*   **Cause**: The pause time `delay(100)` in the code (Note: in this code it is `time.sleep(0.1)`) is too long, causing the car to "react a beat slow"; or the battery power is insufficient, resulting in insufficient motor **torque** ("strength" or "twist" generated when the motor rotates).
*   **Solution**: Try shortening the pause time in the code (such as `time.sleep(0.05)`) to make it react faster. If the battery voltage is low (the car runs weakly), please charge the battery or replace it with a new one.

**Pit 4: The ultrasonic sensor is severely heating up or even burning to the touch**

*   **Cause**: VCC (positive) and GND (negative) are connected backward, or connected to the wrong voltage pins, causing a short circuit.
*   **Solution**: **Cut off the power immediately!** Check whether VCC is connected to 5V and GND is connected to GND. Confirm they are not reversed. Wait for the sensor to cool down before re-checking the circuit.

**Pit 5: Code upload fails, prompting "Connection timed out" or "Board not found"**

*   **Cause**: The USB cable can only charge and cannot transmit data (many cheap cables only have charging functions); or the wrong board model or serial port (COM port, the USB interface number recognized by the computer) was selected in the software.
*   **Solution**: Switch to a USB cable confirmed to support data transmission. In the "Tools" menu of the software, re-select the correct board (ESP32S3) and the corresponding port number.

## 4.11.10 Safety and Precautions

*   **Testing Environment**: Do not test the smart car in crowded places to avoid hitting people (although it's a small car, getting hit still hurts). It is recommended to test on open floors or tabletops.
*   **Electrical Safety**: Do not expose copper wires on battery connection cords to prevent short circuits and fires caused by two wires touching each other.
*   **Exception Handling**: If you smell a burning odor or see smoke, immediately disconnect the power and check the circuit. Do not touch overheating components directly with your hands.
*   **Sensor Protection**: The **piezoelectric ceramic sheet** inside the ultrasonic sensor (a special ceramic material that vibrates to make sound when energized) is fragile. Do not poke or prod the metal mesh cover on the front of the sensor with hard objects.

Have fun and enjoy exploring! If you have any questions, feel free to review the previous content; the answers are often hidden in the details!