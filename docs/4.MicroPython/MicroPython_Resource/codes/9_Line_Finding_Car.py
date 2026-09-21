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