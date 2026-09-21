from machine import Pin
import time
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
from aicam import AiCam  # Assuming the AiCam class above is saved in aicam.py

# Initialize AiCam, camera connected to GPIO17 (TX) and GPIO16 (RX), baud rate 9600
cam = AiCam()
cam.begin(tx=17, rx=18, baud=9600)

# Set to line detection mode (you can also change it to FACE / QR / CARD / LINE)
cam.setMode(AiCam.LINE)

print("AiCam started, waiting for data...")

car = Keyes_ESP32S3_4WD()
# Define the pins for motor control (pins are the metal interfaces on the development board used to output electrical signals)
MOTOR_AIN = 40  # Define A-channel (left wheel) direction control pin as 40, which determines whether the left wheel rotates forward or backward
MOTOR_AEN = 41  # Define A-channel (left wheel) speed control pin as 41, which determines how fast the left wheel rotates
MOTOR_BIN = 38  # Define B-channel (right wheel) direction control pin as 38, which determines whether the right wheel rotates forward or backward
MOTOR_BEN = 21  # Define B-channel (right wheel) speed control pin as 21, which determines how fast the right wheel rotates

# Tell the car that the motors are connected to the 40, 41, 38, and 21 pins defined above, and initialize (start) the motors
car.Motor_init(MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN)
# Define speed parameters (value range is 0 to 255, 255 is the fastest, 0 is stop)
BASE_SPEED = 220  # Set the base forward speed to 220, which is relatively fast yet controllable

# Stop all motors initially to prevent the car from running around as soon as it is powered on
car.stop_motor()

while True:
    # Continuously read and parse serial data
    cam.readData()
    
    angleVal = cam.lineAngle()
    offsetVal = cam.lineOffset()
    validVal = cam.lineValid()
    if abs(angleVal) <= 25 and abs(offsetVal) <= 50 and validVal == 5:
        car.forward(80,80)
    elif angleVal > 25 or offsetVal > 50:
        car.right(80,255)
    elif angleVal < -25 or offsetVal < -50:
        car.left(255,80)
    else :
        car.forward(80,80)
        time.sleep(0.3)
        car.stop_motor()
