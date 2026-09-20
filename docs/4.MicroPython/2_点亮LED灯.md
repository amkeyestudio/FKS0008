# 4.2 Lighting up an LED

## 4.2.1 Lesson Introduction

![a1](./media/a1.png)

Imagine the nightlight in your home or the traffic lights on the street—how do they light up? In fact, there is a "commander" controlling them behind all the lights. Today, we are going to meet a new friend—the LED module. It is like an obedient little soldier; as long as you give an order, it will obediently turn on or off.

In this lesson, we will use the ESP32S3 Pro development board as the "brain," connect the LED module using a few simple wires, and write a magical piece of code. This code is like giving an order to the little soldier: "Hey, light up!", "Hey, take a break!".

When you complete this experiment, you will officially step into the world of electronics! You will learn how to make hardware obey your commands, which is the first step in making complex projects like robots and smart cars. Ready? Let's get started!

## 4.2.2 Lesson Objectives

*   **Hardware Recognition**: Be able to correctly identify the positive, negative, and signal pins of the LED module, and accurately connect them to the corresponding GPIO pins of the ESP32S3 Pro development board.
*   **Programming Practice**: Be able to write and upload MicroPython code to control the LED to flash regularly at 1-second intervals (on for 1 second, off for 1 second).
*   **Debugging Skills**: Learn to use the Thonny IDE serial monitor (REPL) to check the program running status, confirm experiment success, and acquire basic troubleshooting skills.

## 4.2.3 Lesson Equipment

To successfully complete today's experiment, please prepare the following items in advance:

| Name               | Specification/Model               | Quantity | Notes                                                        |
| :----------------- | :-------------------------------- | :------- | :----------------------------------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board     | 1        | The core "brain" of the experiment                           |
| Sensor Module      | LED Module                        | 1        | Usually labeled with S (Signal), V/+ (Power), G/- (Ground)   |
| Connection Wire    | Female-to-Female 3Pin DuPont Wire | 1        | Used to connect the development board and sensor module      |
| Data Cable         | Type-C Data Cable                 | 1        | Used for device power supply and code upload (must support data transmission) |

| Computer | Installed Thonny IDE | 1 | Terminal for writing, uploading code, and debugging |

## 4.2.4 Course Principles

### 4.2.4.1 What is an LED?

The full name of an LED is "Light Emitting Diode". You can think of it as a one-way "electronic valve". Current can only flow in from the positive terminal (anode) and flow out from the negative terminal (cathode), and it will emit light only during this unidirectional conduction process. If the positive and negative terminals are reversed, current cannot pass through, and it will not light up (don't worry, in regular low-voltage experiments, briefly reversing the connection usually will not damage the LED; it just won't work).

### 4.2.4.2 Why is a Module Needed?

An ordinary through-hole LED bead has only two thin metal pins, which are very easy to break. Furthermore, if connected directly to a power supply, it might burn out instantly due to excessive current. Therefore, we use an "LED light module". This module has already integrated a current-limiting resistor (used to limit the current and protect the LED) and the LED bead on a circuit board, and it exposes three convenient pins (Signal, Power, Ground), making it ideal for beginners to perform wiring experiments.

### 4.2.4.3 How Does the Development Board Control an LED?

The ESP32S3 Pro development board is like a room with many smart switches. Each GPIO pin on the edge of the board acts like an electronic switch that can be controlled by code.

*   When we set a pin to "High" (HIGH, usually 3.3V) via code, it is equivalent to turning on the switch; current flows out from the pin, and the LED lights up.
*   When we set a pin to "Low" (LOW, 0V) via code, it is equivalent to turning off the switch; current stops flowing out, and the LED turns off.
    *(Note: Some specially designed modules might light up on a low level, meaning they light up when given a low level and turn off when given a high level. Please refer to the actual design of the module; this tutorial defaults to high-level lighting.)*

### 4.2.4.4 Programming Logic

To achieve LED blinking, our code needs to follow this basic logic:

1.  **Pin Initialization**: First, tell the development board which pin we want to use, and configure it as "Output mode" (Output) so that it can output level signals outward.
2.  **State Switching**: Change the output state of the pin through code—first output a high level to light up the LED, and then output a low level to turn off the LED.
3.  **Time Delay**: Add a delay between state switches. Because computers execute code at an extremely fast speed (microsecond level), without a delay, the blinking of the LED would be so fast that the human eye could not distinguish it, making it look like it is constantly on.
4.  **Loop Execution**: Use an infinite loop (such as `while True:`) to wrap the aforementioned "light up - delay - turn off - delay" process, making the action repeat continuously.

## 4.2.5 Wiring Instructions

Wiring is the most critical step in the experiment, so please be very careful! **Before wiring, please disconnect the USB data cable of the development board to ensure the device is powered off**, so that even if a mistake is made, the components will not be burned out.

An LED module usually has 3 pins, marked as **S** (Signal), **+** or **V** (VCC, positive power supply), **-** or **G** (GND, Ground).

Please connect them strictly according to the table below:

| LED Module Pin | ESP32S3 Pro Development Board Pin | Functional Description                                       |
| :------------- | :-------------------------------- | :----------------------------------------------------------- |
| **-** (GND)    | **GND**                           | Ground, forms a circuit loop, must be connected              |
| **+** (VCC)    | **5V**                            | Provides working power, allowing the LED to emit light       |
| **S** (Signal) | **io11**                          | Signal pin, receives the "on/off" command from the development board |

⚠️ **Special Note**:

1.  **Color Matching**: Usually, the black jumper wire connects to GND, the red wire connects to 5V, and other colors (such as yellow, green, blue) connect to the signal pin io11.
2.  **Pin Confirmation**: There are many pins on the ESP32S3 Pro development board. Please check the silkscreen carefully to find the header hole marked with `io11`.
3.  **Check Before Power-on**: Before plugging in the USB cable, check the table again to ensure that every wire is plugged in tightly and the position is completely correct.

![image-20260918085332631](./media/image-20260918085332631.png)

## 4.2.6 Sample Program

This code will make the LED connected to the io11 pin blink once every 1 second. Please copy the following code completely and paste it into the code editing area of Thonny IDE.

```python
# Import the Pin class from the machine module, used for controlling GPIO pin input and output
from machine import Pin
# Import the time module, used to implement program delay functions
import time

# Create an LED control object, specify connection to GPIO11 (i.e., io11), and set it to output mode (Pin.OUT)
led = Pin(11, Pin.OUT)

# Use an infinite loop to make the LED blink continuously
while True:
    # Set GPIO11 to high level (1) to light up the LED 
    # (Note: If your module lights up on a low level, change 1 here to 0)
    led.value(1)
    # Keep the current lit state for 1 second
    time.sleep(1)
    
    # Set GPIO11 to low level (0) to turn off the LED 
    # (Note: If it is a low-level lighting module, change 0 here to 1)
    led.value(0)
    # Keep the current off state for 1 second
    time.sleep(1)
```

## 4.2.7 Code Explanation

To help you better understand how the code works, we will break down the above program section by section:

1.  **`from machine import Pin`**:
    `machine` is a module in MicroPython used to access underlying hardware resources. The `Pin` class is specifically used to configure and control the input/output pins (GPIO) of the microcontroller.
2.  **`import time`**:
    Imports the time module so that we can use the `time.sleep()` function to pause program execution for a specified amount of time.
3.  **`led = Pin(11, Pin.OUT)`**:
    This line of code instantiates an object named `led`. It tells the development board: "I want to use pin numbered 11 (io11) and use it as an output terminal (`Pin.OUT`)".
4.  **`while True:`**:
    This is an infinite loop. In embedded development, the main program usually needs to run continuously without exiting, so `while True:` is used to make the internal code repeat forever.
5.  **`led.value(1)` and `led.value(0)`**:
    The `value()` method is used to set the level state of the pin. Passing `1` represents outputting a high level (3.3V), and passing `0` represents outputting a low level (0V).
6.  **`time.sleep(1)`**:
    Pauses the program for 1 second. Without these two delay lines, the code would complete an on-off cycle within a few milliseconds, and the human eye would not be able to perceive the blinking at all.

## 4.2.8 Experimental Phenomenon

1.  **Physical Phenomenon**: After the code is successfully uploaded and run, the LED module connected to the io11 pin will start to blink regularly. Specifically: it lights up for 1 second, then turns off for 1 second, repeating this cycle back and forth.
2.  **Verification and Expansion**: You can try modifying the number in `time.sleep(1)` in the code (for example, changing it to `0.2` or `2`). After running it again, observe the change in the blinking frequency of the LED to verify the impact of the delay function on hardware control.

## 4.2.9 FAQ

**Problem: The LED does not light up at all**

*   **Troubleshooting Steps**:
    1.  Check the wiring: Confirm that VCC is connected to 5V, GND is connected to GND, and the signal wire S is indeed connected to io11, with no loose jumper wires.
    2.  Check the code: Confirm that the code has been successfully uploaded and is running (running status is displayed in Thonny).

    3.  Level logic: Some LED modules are designed as "active low" (lights up on low level). Try changing `led.value(1)` in the code to `led.value(0)`, and `led.value(0)` to `led.value(1)`, then run it again.
    4.  Hardware damage: If everything above is normal, the LED module itself might be damaged. You can try replacing the module or testing it on a different pin.

**Problem: The LED stays constantly on (or constantly off) and does not blink**

*   **Troubleshooting Steps**:
    1.  Check the loop structure: Confirm that `while True:` is spelled correctly and that the code underneath is properly indented (Python is very strict about indentation).
    2.  Check the delay function: Confirm that `time.sleep(1)` is not commented out and is spelled correctly.
    3.  Check the running status: Click the "Stop" button in Thonny to see if the LED stops blinking. If it stops, it means the code logic might be stuck somewhere, or the uploaded code is not the latest version.

**Problem: Failed to upload code**

*   **Troubleshooting Steps**:
    1.  Check the data cable: Make sure you are using a Type-C cable that **supports data transfer**. Some cheap or charging-only cables only supply power and cannot transmit data.
    2.  Check the serial port settings: In the bottom right corner of Thonny, confirm that the selected serial port (COM port or `/dev/ttyUSBx`) and interpreter (`MicroPython (ESP32)`) are correct.
    3.  Enter download mode: If you still cannot upload, press and hold the `BOOT` button on the development board, then briefly press the `RST` (reset) button. Release `RST` first and then release `BOOT` to let the board enter download mode, and then try uploading again.

**Problem: The serial monitor (REPL) shows garbled text or is unresponsive**

*   **Troubleshooting Steps**:
    1.  Restart the interpreter: Click the "Stop" button in Thonny, and then click "Run" to reload the interpreter.
    2.  Check the baud rate: Although the default baud rate for MicroPython REPL is usually 115200, if you are using custom serial prints, make sure the baud rate setting in the bottom right corner of Thonny matches the initialization in your code.
    3.  Hardware interference: Check whether the wiring is firm, especially whether the GND is common (shared) and the power supply is stable.

## 4.2.10 Safety Tips

*   **Strictly prohibit short circuits**: Never use jumper wires to directly connect the development board's `5V` (or `3.3V`) and `GND` together. This will instantly generate a massive short-circuit current, which can easily burn out the development board or even the computer's USB port.
*   **Pay attention to polarity**: Although the LED module comes with a built-in current-limiting resistor, it is very important to develop the good wiring habit of "red is positive, black is negative" (connect red to VCC, black to GND). This avoids many potential hardware issues.
*   **Power-off operation**: When plugging/unplugging jumper wires, modifying wiring, or replacing modules, be sure to unplug the USB data cable first to ensure the development board is completely powered off, ensuring personal and equipment safety.
*   **Electrostatic protection**: During dry seasons, static electricity carried by the human body may break down precision chips on the development board. It is recommended to touch a grounded metal object (such as a metal water pipe or computer chassis casing) to discharge static electricity before operating.

Congratulations! You have successfully completed your first embedded hardware experiment and taken an important step toward becoming a junior engineer! Keep your curiosity alive and keep going!