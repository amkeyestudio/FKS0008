from machine import Pin
import time
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
from aicam import AiCam  # Assuming the AiCam class above is saved in aicam.py

# Initialize AiCam, camera connected to GPIO17 (TX) and GPIO16 (RX), baud rate 9600
cam = AiCam()
cam.begin(tx=17, rx=18, baud=9600)

# Set to color recognition mode (you can also change it to FACE / QR / CARD / LINE)
cam.setMode(AiCam.COLOR)

print("AiCam started, waiting for data...")

car = Keyes_ESP32S3_4WD()
# Define motor control pins (pins are the metal interfaces on the development board used to output electrical signals)
MOTOR_AIN = 40  # Define direction control pin for channel A (left wheel) as 40, which determines whether the left wheel rotates forward or backward
MOTOR_AEN = 41  # Define speed control pin for channel A (left wheel) as 41, which determines how fast the left wheel rotates
MOTOR_BIN = 38  # Define direction control pin for channel B (right wheel) as 38, which determines whether the right wheel rotates forward or backward
MOTOR_BEN = 21  # Define speed control pin for channel B (right wheel) as 21, which determines how fast the right wheel rotates

# Tell the car that the motors are connected to the previously defined pins 40, 41, 38, and 21, and initialize (start) the motors
car.Motor_init(MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN)
# Define speed parameters (value range is 0 to 255, 255 is the fastest, 0 is stopped)
BASE_SPEED = 220  # Set the base straight-line speed to 220, which is relatively fast without losing control

# In the initial state, stop all motors to prevent the car from running around randomly when powered on
car.stop_motor()

# Assign a "house number" (pin number) to the servo signal line; here we choose pin 11
SERVO_PIN = 11   
# Tell the magic box: "Please initialize (prepare) pin 11, we are going to start controlling the servo!"
car.Servo_init(SERVO_PIN)

# Issue the first command: make the servo turn to 140 degrees first (gripper closed)
car.Servo_set_angle(140)

while True:
    # Continuously read and parse serial data

    cam.flush()
    time.sleep(0.1)
    cam.readData()
    colorVal = cam.color()
    if colorVal == "RED" or colorVal == "YELLOW" :
        car.Servo_set_angle(90)
        time.sleep(0.3)
        print(colorVal)
        if colorVal == "RED":
            car.left(BASE_SPEED, BASE_SPEED)
        else :
            car.right(BASE_SPEED, BASE_SPEED)   
        time.sleep(0.3)
        car.forward(BASE_SPEED, BASE_SPEED)
        time.sleep(0.3)
        car.stop_motor()
        time.sleep(0.3)
        car.Servo_set_angle(140)
        time.sleep(0.2)
        car.back(BASE_SPEED, BASE_SPEED)
        time.sleep(0.3)
        if colorVal == "RED":
            car.right(BASE_SPEED, BASE_SPEED)
        else :
            car.left(BASE_SPEED, BASE_SPEED)
        time.sleep(0.3)
        car.stop_motor()
    else :
        car.stop_motor()

