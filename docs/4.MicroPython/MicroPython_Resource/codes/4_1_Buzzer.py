# Import the "control toolbox" written specifically for this car development board, which encapsulates ready-made commands for controlling hardware such as the buzzer
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the "time" toolbox, used to make the program wait (delay) to control the rhythm of the sound
import time

# Create an object named car, which is equivalent to waking up our development board car, ready to receive commands
car = Keyes_ESP32S3_4WD()

# Define a variable buzzer_pin, telling the program that the buzzer is connected to pin number 10 (IO10)
buzzer_pin = 10

# Initialize the onboard buzzer, preparing the development board to send control signals to the buzzer through pin 10
car.Buzzer_init(buzzer_pin)

# Start an infinite loop, making the code below repeat continuously until you manually stop the program
while True:
    
    # Print a prompt line of text "Playing Alarm..." on the computer, making it easy for you to know which step the program is running
    print("Playing Alarm...")
    # 1. Alarm stage: emit a short, sharp "beep-beep-beep" sound
    # Use a for loop to make i go from 0 to 2, executing a total of 3 iterations
    for i in range(3):
        # Call the play command: set frequency to 1000Hz (sharp sound), continuous sound duration 200 milliseconds (0.2 seconds)
        car.Buzzer_play(1000, 200)
        # Pause the program for 200 milliseconds to form the pause interval between "beeps"
        time.sleep_ms(200)

    # 2. Rest stage
    # Print the prompt text "Resting..." on the computer, indicating that the alarm has ended and rest begins
    print("Resting...")
    # Pause the program quietly for 1000 milliseconds (which is 1 second); during this time, the buzzer does not sound
    time.sleep_ms(1000)

    # 3. Music stage: play the Do-Re-Mi-Fa-Sol-La-Si scale
    # Print the prompt text "Playing Scale..." on the computer, indicating the start of playing the scale
    print("Playing Scale...")
    # Call the ready-made function to play low Do (frequency about 262Hz)
    car.Buzzer_play_Do()  # Low Do
    # Call the ready-made function to play low Re (frequency about 294Hz)
    car.Buzzer_play_Re()  # Low Re
    # Call the ready-made function to play low Mi (frequency about 330Hz)
    car.Buzzer_play_Mi()  # Low Mi
    # Call the ready-made function to play low Fa (frequency about 349Hz)
    car.Buzzer_play_Fa()  # Low Fa
    # Call the ready-made function to play low Sol (frequency about 392Hz)
    car.Buzzer_play_Sol() # Low Sol
    # Call the ready-made function to play low La (frequency about 440Hz)
    car.Buzzer_play_La()  # Low La
    # Call the ready-made function to play low Si (frequency about 494Hz)
    car.Buzzer_play_Si()  # Low Si

    # 4. Long rest before looping
    # Pause the program quietly for 2000 milliseconds (which is 2 seconds), preparing to restart the next round of the loop
    time.sleep_ms(2000)