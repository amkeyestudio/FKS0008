# From the car toolkit prepared in advance (ESP32S3_4WD_Car), fetch the "magic book" specifically used to control this car (module Keyes_ESP32S3_4WD)
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the time module, as it contains many tools to control time, such as making the program "wait"
import time

# Create a "clone" for the car (called instantiating an object in programming), and name it line. From now on, we will command the car's line-tracking function through line
line = Keyes_ESP32S3_4WD()

# Tell the program that we want to use pin number 8 (io8) on the development board as the data line (SDA)
SDA = 8
# Tell the program that we want to use pin number 9 (io9) on the development board as the clock line (SCL)
SCL = 9

# Use the SCL and SDA pins defined just now to "wake up" the line-tracking sensor and connect it properly (this process is called initialization)
line.Line_init(SCL, SDA)

# Print a sentence in the computer's serial monitor (chat window) to tell us that the preparation work is done
print("5-channel line-tracking sensor initialization complete!")

# This is an "infinite loop" (while True), meaning that as long as power is not cut off, the indented code below will repeat indefinitely
while True:
    # Have the sensor "look" at the path once, and pack the results of the 5 probes into the 5 variables A, B, C, D, E (little data-storing boxes) respectively
    A, B, C, D, E = line.Line_get_data()

    # Print out the data in the 5 boxes A, B, C, D, E, separated by spaces, and automatically move to the next line after printing
    print(A,B,C,D,E)

    # Let the program rest for 500 milliseconds (half a second) to prevent the data from refreshing too fast for us to see clearly, and also to give the brain (CPU) a short break
    time.sleep_ms(500)