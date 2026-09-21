# Take out a "magic box" (control module) named Keyes_ESP32S3_4WD from the "Car/Robot" toolkit provided by the manufacturer
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the "time" toolkit, because we need the program to "wait" and control the rhythm of time
import time

# Assign a "house number" (pin number) to the servo's signal wire. Here we choose pin 42
SERVO_PIN = 42

# Open the "magic box" just taken out and create a control object named servo
servo = Keyes_ESP32S3_4WD()

# Tell the magic box: "Please initialize (prepare) pin 42, we are going to start controlling the servo!"
servo.Servo_init(SERVO_PIN)

# Issue the first command: make the servo rotate to 90 degrees (the exact middle position) first
servo.Servo_set_angle(90)
# Pause the program for 1 second, giving the servo a little time to slowly rotate to 90 degrees and stand firmly
time.sleep(1)

# Start a magic circle of "infinite loop". As long as the power is not cut off, the code inside will keep repeating execution
while True:
    # First stage: make the angle variable start from 0, add 1 each time, all the way up to 180 (inclusive)
    for angle in range(0, 181, 1):
        # Send the current angle value to the servo, commanding it to rotate to this angle
        servo.Servo_set_angle(angle)
        # After each rotation, pause for 15 milliseconds (15/1000 of a second) to give the servo time to physically rotate for a smooth effect
        time.sleep_ms(15)

    # After reaching 180 degrees, let the servo rest (pause) here for 1 second
    time.sleep(1)

    # Second stage: make the angle variable start from 180, subtract 1 each time, all the way down to 0 (inclusive)
    for angle in range(180, -1, -1):
        # Send the current angle value to the servo, commanding it to rotate in reverse to this angle
        servo.Servo_set_angle(angle)
        # Also pause for 15 milliseconds to maintain the smooth rotation rhythm
        time.sleep_ms(15)

    # After returning to 0 degrees, let the servo rest for another 1 second, then restart the next round of the loop
    time.sleep(1)