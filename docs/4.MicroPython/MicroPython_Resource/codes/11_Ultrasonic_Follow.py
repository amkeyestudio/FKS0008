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