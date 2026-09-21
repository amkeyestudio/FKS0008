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