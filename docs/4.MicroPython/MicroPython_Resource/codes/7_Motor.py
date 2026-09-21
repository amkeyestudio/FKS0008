# From the file (library) named ESP32S3_4WD_Car, import the Keyes_ESP32S3_4WD toolkit, which encapsulates the complex code for controlling the robot car for us
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD

# Import the time module so that we can "pause" or "delay" the program
import time

# Create an object named car, which is equivalent to activating the imported toolkit to prepare for controlling the car
car = Keyes_ESP32S3_4WD()

# ================= Pin Definitions =================
# Next, we tell the brain (development board) which finger (pin) is responsible for which action

# Define the direction control pin for Motor A as pin 40 (controls the forward/reverse rotation of the left wheel)
MOTOR_AIN = 40  

# Define the speed control pin for Motor A as pin 41 (controls the speed of the left wheel, i.e., the PWM pin)
MOTOR_AEN = 41  

# Define the direction control pin for Motor B as pin 38 (controls the forward/reverse rotation of the right wheel)
MOTOR_BIN = 38  

# Define the speed control pin for Motor B as pin 21 (controls the speed of the right wheel, i.e., the PWM pin)
MOTOR_BEN = 21  

# Call the initialization function to pass the 4 pin numbers defined above to the car program, preparing it to use these pins
car.Motor_init(MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN)

# ================= Initialization and Main Loop =================

# When starting up, first make the motors stop to ensure safety first and prevent the car from running around randomly as soon as power is applied
car.stop_motor()

# Print a prompt message on the computer screen to tell us that the system is ready
print("Motor driver system initialization complete, starting operation!")

# This is an "infinite loop", meaning as long as power is not cut off, the indented code below will repeat continuously
while True:
    
    # 1. Move forward at full speed for 2 seconds
    # Print "Forward" on the screen so we can observe which step the program is currently running
    print("Forward")
    
    # Call the forward function, where the two 255s represent that both left and right wheels output maximum speed (255 is full speed)
    car.forward(255, 255)
    
    # Pause (rest) the program for 2 seconds, keeping the car moving forward for 2 seconds
    time.sleep(2)

    # 2. Move backward at full speed for 2 seconds
    # Prompt "Backward" on the screen
    print("Backward")
    
    # Call the backward function, with both left and right wheels reversing at maximum speed
    car.back(255, 255)
    
    # Keep the backward state for 2 seconds
    time.sleep(2)

    # 3. Turn left in place for 2 seconds
    # Prompt "Turn Left" on the screen
    print("Turn Left")
    
    # Call the left turn function (usually the left wheel goes backward and the right wheel goes forward to achieve in-place spinning)
    car.left(255, 255)
    
    # Keep the left turn state for 2 seconds
    time.sleep(2)

    # 4. Turn right in place for 2 seconds
    # Prompt "Turn Right" on the screen
    print("Turn Right")
    
    # Call the right turn function (usually the left wheel goes forward and the right wheel goes backward)
    car.right(255, 255)
    
    # Keep the right turn state for 2 seconds
    time.sleep(2)

    # 5. Stop for 2 seconds, preparing for the next cycle
    # Prompt "Stop" on the screen
    print("Stop")
    
    # Call the stop function to cut power to all motors and stop them
    car.stop_motor()
    
    # Rest in place for 2 seconds, then return to the beginning of the while True loop to start the next round of actions
    time.sleep(2)