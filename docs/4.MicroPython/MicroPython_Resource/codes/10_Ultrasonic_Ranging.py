# Import the Keyes_ESP32S3_4WD toolkit from the ESP32S3_4WD_Car file, which contains various functions to control the car
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the time module to pause and delay the program
import time

# Define the pins (telling the program which numbered interface our wires are plugged into)
TRIG_PIN = 13   # Trigger pin (Trig) connected to the IO13 interface of the development board
ECHO_PIN = 12   # Echo pin (Echo) connected to the IO12 interface of the development board

# Create an object named ultrasonic, which is equivalent to activating the sensor function
ultrasonic = Keyes_ESP32S3_4WD()

# Initialize the ultrasonic sensor, telling it that Trig and Echo are connected to pins 13 and 12 respectively
ultrasonic.Ultrasonic_init(TRIG_PIN, ECHO_PIN)

# Main loop: while True means "when the condition is true", since True is always true, the code below will repeat endlessly like a clock
while True:
    # Call the measurement function to make the sensor measure the distance once, and store the result in the distance variable
    distance = ultrasonic.Ultrasonic_measure_distance()

    # Print the measurement result through the serial port (chat window on the computer) with the unit cm (centimeters)
    print("Distance:", distance, "cm")

    # Let the program rest (wait) for 1 second before the next measurement to prevent the data from refreshing too fast for the eyes to see
    time.sleep(1)