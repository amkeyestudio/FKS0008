# main.py - ESP32-S3 Infrared Reception (NEC Protocol)
# Import the Pin class from the machine module to control the metal interfaces on the development board
from machine import Pin
# Import the NEC_8 class from the ir_rx.nec module, which is a "dictionary" specially used to translate NEC protocol (8-bit address) infrared signals
from ir_rx.nec import NEC_8
# Import the time module to pause or delay the program
import time

# Define a variable IR_RX_PIN with a value of 48. This tells the program: the infrared receiver is plugged into interface number 48
IR_RX_PIN = 48

# Define a "callback function" (which is a backup assistant). When an infrared signal is successfully understood, the program will automatically call it to work
def ir_callback(data, addr, ctrl):
    # data represents "command data" (which key you pressed)
    # addr represents "device address" (whose remote control signal this is)
    # ctrl represents "control flag" (used to determine whether you pressed it briefly or held it down)
    
    # If data is less than 0, it usually means you are holding down the remote control without releasing it (a repeat code is received)
    if data < 0:
        # Print "Holding down..." on the computer screen
        print("Holding down...")
    else:
        # If it is not a long press, print the "key value" you pressed on the computer screen
        print("Key value: ", data)

# Initialize the infrared receiving object (formally hiring this translation assistant)
# Pin(IR_RX_PIN, Pin.IN) means: set interface number 48 to "input mode" (receive signals only, do not send signals)
# ir_callback means: once translation is successful, automatically call the assistant function defined above
ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), ir_callback)

# Print a prompt on the computer screen to let you know that infrared reception is ready
print("Infrared reception started, please press a remote control button...")

# Start an infinite loop (while True) to keep the program running continuously without stopping halfway
while True:
    # Make the program rest for 100 milliseconds (0.1 seconds) each time.
    # Why is a rest needed? Because infrared reception is automatically handled in the background, letting the main program rest slightly saves brain (CPU) processing power
    time.sleep_ms(100)