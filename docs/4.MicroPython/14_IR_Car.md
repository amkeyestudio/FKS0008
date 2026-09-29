# 4.14 Infrared Remote Control Smart Car

## 4.14.1 Course Introduction

Remember the "ultrasonic obstacle avoidance smart car" we learned about earlier? It is like a blindfolded little robot walking around, relying only on "touching" (emitting ultrasonic waves) to avoid obstacles. Today, we are going to equip the car with "ears" and a "brain" so that it can understand your remote commands! This is what we are going to make—the **Infrared Remote Control Smart Car**.

Imagine holding a TV remote control in your hand, pressing "Forward," and the car obediently runs forward; pressing "Turn Left," and it flexibly turns. This is just like playing a video game, except this time you are controlling the real physical world!

**💡 Beginner's Science Pop-Up:**

*   **Infrared Remote Control**: This is a technology that uses "infrared light" (a type of light invisible to the naked eye) to transmit instructions. It is very common in daily life. Besides TVs, air conditioners, fans, and even many toy cars use it.
*   **ESP32S3 Pro Development Board**: This is the "brain" of our car, a small circuit board that integrates a processor and various interfaces.

In this lesson, we will combine the motor driving knowledge we learned previously (making the wheels turn) with the newly learned infrared receiving knowledge (understanding remote control commands). You will write code yourself to let the ESP32S3 Pro development board interpret the mysterious signals sent by the remote control and command the car's wheels to turn. After completing this lesson, you will master a magic skill for wirelessly controlling hardware, taking you one step closer to becoming a true "robot engineer." Are you ready? Let's get started!

## 4.14.2 Course Objectives

*   Correctly connect the **infrared receiving module** (the component responsible for receiving remote control signals) to the specified **pin** (the small metal interface on the chip used to connect external devices) of the ESP32S3 Pro development board.
*   Understand the basic principle of infrared remote control: the remote control acts like a "telegraph transmitter" sending signals, and the receiver acts like an "antenna" receiving signals.
*   Use the `IRremote` library (a code toolbox written by someone else specifically for processing infrared signals) to read the **hexadecimal code** corresponding to the buttons on the remote control (the "digital ID card" exclusive to each button).
*   Write programs to make the car execute forward, backward, left turn, right turn, and stop actions when different buttons on the remote control are pressed.
*   Learn how to debug code and solve problems such as unresponsive remote controls or incorrect car actions.

## 4.14.3 Course Equipment

| Name               | Specification/Model                                     | Quantity | Remarks                                                      |
| :----------------- | :------------------------------------------------------ | :------- | :----------------------------------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board                           | 1        | The core brain responsible for data processing and issuing commands |
| Expansion Board    | ESP32S3 Pro Expansion Board                             | 1        | Integrates the infrared receiving sensor and motor driver, acting like the nerves and muscles of the car |
| Remote Control     | Infrared Remote Control                                 | 1        | The "remote control" used to issue commands                  |
| Battery Holder     | 6-slot AA battery holder or 2-slot 18650 battery holder | 1        | Bring your own batteries to provide power for the car        |

**Preparation**: Please follow the previous tutorials to assemble the mechanical parts and circuit board of the car.

## 4.14.4 Course Principles

The car is controlled by evaluating the values received by the infrared receiving sensor (i.e., the "ID number" of the remote control button).

**Layman's Terms**:
Press a button on the remote control -> Sends an infrared password -> The receiver on the car receives the password -> The brain (ESP32S3) translates the password -> Recognizes it as the "Forward" password -> Commands the motor to rotate -> The car moves forward!

1. **Start**: The car is powered on, and the infrared receiving function is enabled.
2. **Wait**: Continuously listen for incoming infrared signals.
3. **Evaluate**: Has a signal been received?
   - Not received -> Go back to step 2 and continue waiting.
   - Received -> Proceed to step 4.
4. **Translate**: Which button does this signal represent? (e.g., whether it is the "Up" button or the "Left" button).
5. **Execute**: Perform the corresponding action based on the button (move forward, turn left, etc.).
6. **Loop**: After the action is executed, return to step 2, ready to listen for the next command.

## 4.14.5 Pre-class Preparation

1. **Check Remote Control Batteries**: Ensure that new batteries are installed in the infrared remote control.
2. **Get to Know Your Remote Control**: Pick up the remote control and look at the buttons on it. Usually, there are directional buttons (up, down, left, right) and a confirmation button (OK).
3. **Open Programming Software**: Ensure that the software used to write code for the ESP32S3 (such as Thonny or VS Code) is open on your computer, and be ready to upload the code to the car.

## 4.14.6 Hardware Wiring

**Why is complicated wiring not required?**
Because the motor driver (the circuit that controls wheel rotation) and the infrared receiving sensor (the component that receives signals) are already **integrated** (pre-soldered and connected) on the ESP32S3 expansion board.

**Operating Steps**:

1. Confirm that the expansion board is firmly plugged into the main control board (ESP32S3 Pro).
2. Confirm that the battery holder is properly connected.
3. No additional jumper wires need to be plugged or unplugged; simply assemble the car and upload the code to see the reaction. This greatly reduces the risk of beginners making wiring mistakes!

## 4.14.7 Sample Program

This code achieves the following: it reads the button values from the infrared remote control and controls the car's forward, backward, left turn, right turn, and stop actions based on the pressed buttons.

```python
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD  # Import the car-specific "driving manual" library so we can easily control the car
# main.py - ESP32-S3 Infrared Reception (NEC Protocol)       # This is a comment telling others this is an infrared receiving program using the common NEC infrared protocol
from machine import Pin                        # Import the "machine pin" module to control the metal small interfaces (pins) on the development board
from ir_rx.nec import NEC_8                    # Import the NEC protocol infrared decoder, specially designed to understand the remote control's "language"
import time                                    # Import the "time" module to pause or delay the program

car = Keyes_ESP32S3_4WD()                      # Create a "car" object, which is equivalent to summoning the car from the blueprint into code, and name it car

# Motor pin definitions (telling the brain which interfaces the wheel control wires are plugged into)
MOTOR_AIN = 40  # Channel A direction control pin (controls whether the left wheel rotates forward or backward), plugged into interface 40
MOTOR_AEN = 41  # Channel A PWM control pin (controls how fast the left wheel rotates), plugged into interface 41
MOTOR_BIN = 38  # Channel B direction control pin (controls whether the right wheel rotates forward or backward), plugged into interface 38
MOTOR_BEN = 21  # Channel B PWM control pin (controls how fast the right wheel rotates), plugged into interface 21

car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN) # Initialize the motors, telling the car the 4 pins defined above so it gets ready to control the wheels

# Speed parameters
BASE_SPEED = 220  # Base driving speed, ranging from 0 to 255. 220 is a relatively fast speed without losing control

# Stop motors in initial state
car.stop_motor() # When first started, make the car stop first to prevent it from running around randomly upon power-up


# Define the infrared reception pin as IO48 (telling the brain that the wire receiving the infrared signal is plugged into interface 48)
IR_RX_PIN = 48

# Define the callback function (this is an "auto-trigger mechanism": once an infrared signal is received, the code inside is automatically executed)
def ir_callback(data, addr, ctrl): 
    # data: Command data (the "password/ID number" corresponding to the button you pressed)
    # addr: Device address (the factory serial number of the remote control, not used here)
    # ctrl: Control flag (used to determine whether a press is short or long)
    if data < 0:
        # If data is less than 0, it usually means you held down the remote control button, receiving a "repeat code"
        print("Holding down...") # Print prompt text on the computer
    else:
        # If it is a normal button press, print the decoded button password and address
        print("Button value: ", data) # Display which password you pressed on the computer
        if data == 70:
            car.forward(BASE_SPEED, BASE_SPEED) # If the password is 70 (usually the "Up" button), the car moves forward at full speed
        elif data == 21:
            car.back(BASE_SPEED, BASE_SPEED)    # If the password is 21 (usually the "Down" button), the car moves backward at full speed
        elif data == 68:
            car.left(BASE_SPEED, BASE_SPEED)    # If the password is 68 (usually the "Left" button), the car turns left
        elif data == 67:
            car.right(BASE_SPEED, BASE_SPEED)   # If the password is 67 (usually the "Right" button), the car turns right
        elif data == 64:
            car.stop_motor()                    # If the password is 64 (usually the "OK" button), the car stops immediately
        

# Initialize the infrared receiving object (officially start the infrared receiving function)
# Pin(IR_RX_PIN, Pin.IN) means setting pin 48 to "input mode" (only receives signals, does not output signals)
# ir_callback is the "mechanism" function automatically called when signal decoding succeeds
ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), ir_callback) 

print("Infrared reception started, please press a remote control button...") # Print a prompt on the computer telling you it's ready


while True: # This is an "infinite loop", meaning the code below will repeat continuously until you disconnect the power
    # The main loop can handle other tasks, while infrared reception is done in the background by interrupts (a background auto-handling mechanism)
    time.sleep_ms(100) # Make the program rest for 100 milliseconds (0.1 seconds) per loop to prevent the brain from getting overworked, while also leaving processing time for infrared reception
```

## 4.14.8 Code Explanation

For absolute beginners, this code might look a bit long. Don't worry, let's break it down:

1.  **Summoning the car and setting up the wheels**:
    At the beginning, we "summon" the car using `car = Keyes_ESP32S3_4WD()`. Next, we tell the brain (ESP32S3) which **pins** (interfaces) the wires controlling the left and right wheels are plugged into. `BASE_SPEED = 220` sets how fast the car runs, where 255 is the limit speed, and 220 is a safe yet smooth speed.
2.  **Setting up the "auto-trigger mechanism" (callback function)**:
    The `def ir_callback(...)` code is the core! It defines a rule: **Whenever** the receiving head hears a signal from the remote control, it **immediately** executes the judgments inside.
    *   It first retrieves the button's "password" (`data`).
    *   Then it uses `if...elif...` (if...else if...) to check the password. For example, if it matches the password `70`, it calls `car.forward()` to make the car move forward.
3.  **Opening the infrared ears and the infinite loop**:
    The line `ir = NEC_8(...)` officially opens the car's "infrared ears."
    The final `while True:` is an **infinite loop** (a loop that repeats continuously). Why is it needed? Because the program needs to stay running at all times, ready to receive commands from the remote control. Without it, the program would stop after running once, and the car would fall "asleep."

## 4.14.9 Experimental Phenomena and Debugging

1.  **Upload the Code**: Compile the code (translating human-readable code into machine-understandable instructions) and upload it to the ESP32S3 Pro development board.
2.  **Open the Monitor**: Open the "Serial Monitor" in the programming software (the **Serial port** is the "chat box" where the computer and the car talk, and the **Monitor** is the screen showing the chat history).
3.  **Test the Buttons**: Pick up the infrared remote control, point it at the receiving head on the car (usually a black, small dome-like part), and press the "Up" button.
    *   **Serial Output**: You should see text similar to `Button value: 70`.
    *   **Car Action**: Both wheels of the car should rotate forward simultaneously.
4.  **Test Other Buttons**:
    *   Press "Down": The car moves backward.
    *   Press "Left": The car turns left (the principle is that the left wheel reverses and the right wheel rotates forward, making the car turn left in place).
    *   Press "Right": The car turns right (left wheel rotates forward, right wheel reverses).
    *   Press "OK": The car stops immediately.
5.  **Success Sign**: If you press a button, the car responds with the exact corresponding action, and the correct password print information appears on the serial monitor, congratulations! You have successfully built an infrared remote-controlled car!

## 4.14.10 Common Errors and Solutions

It is completely normal for beginners to run into problems during the build process! Here are the pitfalls everyone is most likely to encounter and their solutions:

**Problem 1: Pressing the remote control yields no response on the serial port, and the car doesn't move.**

*   **Cause**: It could be due to incorrect wiring of the infrared receiver, dead remote control batteries, or a mismatch between the pin definition in the code and the actual wiring.
*   **Solution**:
    1.  Check if the VCC (positive), GND (negative), and OUT (signal) of the infrared receiver are connected correctly. In particular, VCC and GND must never be reversed, or it will burn out the components! *(Note: This lesson uses an integrated expansion board, so wiring errors are unlikely, but you still need to check if the expansion board is plugged in securely)*.
    2.  **Mobile Phone Camera Test Method**: Open your mobile phone camera, point the remote control transmitter (the small bulb on top) at the lens, and press a button. Look at the phone screen to see if white or purple light is flashing (phone cameras can capture infrared light) to determine if the remote control is transmitting normally. If it doesn't flash, change the batteries!
    3.  Confirm that the value of `IR_RX_PIN` in the code (currently 48) matches the pin number you are actually using.

**Problem 2: The serial port prints data, but the printed command codes are different from the ones defined in my code.**

*   **Cause**: Every infrared remote control has a different encoding protocol and button codes. Directly copying the button values from someone else's code will not work. The code for the "Up" button on your remote might not be 70.
*   **Solution**: First, comment out the car movement control part (add `#` at the beginning of the line), leaving only the serial printing code. After uploading, press each button on your remote control, record the number corresponding to each button in the Serial Monitor (e.g., `70`, `21`), and then modify the number in `if data == 70:` in the code to match the number measured from your own remote control.

**Problem 3: The car's action is reversed (e.g., it moves backward when you press forward).**

*   **Cause**: The positive and negative wires of the motor are reversed, or the direction control logic in the code is written backward.
*   **Solution**: The easiest method is to physically swap the two wires of the corresponding motor on the motor driver module (e.g., swap A-IA and A-IB); or modify the direction logic in the code. *(Note: Since we are using the encapsulated `car.forward()` function, if the direction is reversed, it is usually because the motor wires are wired incorrectly. Please check the motor wires first)*.

*   **Problem 4: The car moves only for a moment and then stops, or the movement stutters.**

*   **Cause**: The infrared receiver library needs to be manually reset after processing a signal, or insufficient battery power has caused a voltage drop.
*   **Solution**: Ensure the battery is fully charged. In addition, make sure to check whether the program gets a rest at the end of the main loop.

## Safety and Precautions

*   **Anti-Drop**: Please test the car on an open and flat floor to prevent hardware damage from falls from high places.
*   **Anti-Stall**: Do not let the car **stall** for a long time (wheels are stuck and cannot turn, but the motor is still powered and trying hard), as this will cause excessive current, generate heat, and potentially burn out the motor or driver module. If you hear a "humming" sound from the car but it does not move, please press the stop button or turn off the power immediately.
*   **Anti-Short-Circuit**: When wiring, disconnecting wires, or modifying hardware, **be sure to disconnect the power** (unplug the battery or turn off the switch) to prevent the positive and negative poles from touching each other, which causes a **short circuit** (current connects directly without passing through electrical appliances, instantly generating high temperatures that burn out the motherboard).
*   **Line-of-Sight Control**: Infrared remote control is "line-of-sight control," which travels in a straight line just like light. Make sure there are no severe obstacles between the remote control and the receiver, and the distance does not exceed 5–8 meters.