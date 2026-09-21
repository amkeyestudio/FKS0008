# Import the car control class from the ESP32S3_4WD_Car library, equivalent to hiring an "assistant" who understands car control
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the time module to pause the program (delay), just like humans need time to react
import time

# Define the pin numbers for the ultrasonic sensor for easy future reference
TRIG_PIN = 13   # Trigger pin connected to IO13, telling the sensor to "emit"
ECHO_PIN = 12   # Echo pin connected to IO12, receiving the sensor's "returned" signal

# Create a car object named car, through which we will control the car later
car = Keyes_ESP32S3_4WD()

# Initialize the ultrasonic sensor, telling it which pins Trig and Echo are connected to
car.Ultrasonic_init(TRIG_PIN,ECHO_PIN)

# Define the pin numbers of the motor driver to control the left and right wheels
MOTOR_AIN = 40  # Channel A direction control pin (controls left wheel forward or reverse rotation)
MOTOR_AEN = 41  # Channel A PWM control pin (controls left wheel rotation speed)
MOTOR_BIN = 38  # Channel B direction control pin (controls right wheel forward or reverse rotation)
MOTOR_BEN = 21  # Channel B PWM control pin (controls right wheel rotation speed)

# Initialize the motors, telling the car assistant the 4 pins defined above
car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)

# Define the basic driving speed parameter, ranging from 0 (stop) to 255 (full speed), set to 220 here for stability
BASE_SPEED = 220  # Basic driving speed (0-255)

# Stop the motors in the initial state to prevent the car from running around as soon as power is turned on
car.stop_motor()

# Define the pin number for servo control
SERVO_PIN = 42    # Servo signal line connected to IO42

# Initialize the servo, telling it which pin the signal line is connected to
car.Servo_init(SERVO_PIN)

# Initialize the servo to the 90-degree position, meaning the "neck" faces straight ahead
car.Servo_set_angle(90)
# Wait for 1 second to give the servo enough time to turn to 90 degrees and stabilize
time.sleep(1)

# Main loop: while True means "infinite loop", making the car constantly repeat the following actions
while True:
    # Measure the distance ahead and assign the result to the variable distance
    distance = car.Ultrasonic_measure_distance()

    # Print the result via the serial port (the communication channel between computer and development board) for easy viewing on the computer
    print("Distance:", distance, "cm")
    
    # Conditional check: if the distance ahead is less than or equal to 15 centimeters (danger encountered)
    if distance <= 15 :
        # Stop the motors immediately, apply brakes
        car.stop_motor()
        # Rotate the servo to 180 degrees, meaning the "neck" looks to the far left
        car.Servo_set_angle(180)
        # Wait for 0.3 seconds to let the servo reach position and stabilize, otherwise distance measurement will be inaccurate
        time.sleep(0.3)
        # Measure the distance on the left and save it in the leftDistance variable
        leftDistance = car.Ultrasonic_measure_distance()
        # Wait a little for 0.1 seconds to give the sensor a breathing interval
        time.sleep(0.1)

        # Rotate the servo to 0 degrees, meaning the "neck" looks to the far right
        car.Servo_set_angle(0)
        # Wait for 0.3 seconds to let the servo reach position and stabilize
        time.sleep(0.3)
        # Measure the distance on the right and save it in the rightDistance variable
        rightDistance = car.Ultrasonic_measure_distance()
        # Wait a little for 0.1 seconds
        time.sleep(0.1)
        
        # Rotate the servo to 90 degrees, returning the "neck" to the middle position facing straight ahead
        car.Servo_set_angle(90)
        
        # Conditional check: if the left distance is greater than the right distance (indicating the left side is more open)
        if leftDistance > rightDistance :
            # Control the car to turn left (left wheel moves backward, right wheel moves forward, speed is BASE_SPEED)
            car.left(BASE_SPEED, BASE_SPEED)
        # Otherwise (indicating the right side is more open, or both sides are equal)
        else :
            # Control the car to turn right (left wheel moves forward, right wheel moves backward, speed is BASE_SPEED)
            car.right(BASE_SPEED, BASE_SPEED)
        # Continue turning for 0.5 seconds, turning just about enough is fine
        time.sleep(0.5)
    # Otherwise (if the distance ahead is greater than 15 centimeters, very safe)
    else :
        # Control the car to move straight ahead (both left and right wheels rotate forward, speed is BASE_SPEED)
        car.forward(BASE_SPEED, BASE_SPEED)
    # After each loop ends, rest for 0.1 seconds to prevent the program from running too fast and leaving the sensor no time to react
    time.sleep(0.1)
