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