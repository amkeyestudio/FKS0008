# Import the "control toolbox" written specifically for this car development board
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# Import the "time" toolbox for delays
import time

# Create an object named car to wake up the development board
car = Keyes_ESP32S3_4WD()

# Define the buzzer control pin as IO10, because the buzzer is soldered to this pin
BUZZER_PIN = 10

# Initialize the onboard buzzer, preparing pin 10 to output control signals
car.Buzzer_init(BUZZER_PIN)


# Define a list (which can be thought of as a row of boxes) containing the note indices for "Happy Birthday"
# Numbers represent Do=1, Re=2, Mi=3... These numbers correspond to the preset scale positions in the underlying library
HAPPY_BIRTHDAY = [
    5, 5, 6, 5, 8, 7,       # Wish you a happy birthday
    5, 5, 6, 5, 9, 8,       # Wish you a happy birthday
    5, 5, 12, 10, 8, 7, 6,  # Wish you a happy birthday (climactic part)
    11, 11, 10, 8, 9, 8     # Wish you a happy birthday
]

# Define a list containing the beat length (duration of the sound) corresponding to each note
# Values represent relative beat lengths, where the base beat is 200ms (0.2 seconds), e.g., 2 means 400ms
METER = [
    1, 1, 2, 2, 2, 4,       # Duration corresponding to the first phrase
    1, 1, 2, 2, 2, 4,       # Duration corresponding to the second phrase
    1, 1, 2, 2, 2, 2, 2,    # Duration corresponding to the third phrase
    1, 1, 2, 2, 2, 4        # Duration corresponding to the fourth phrase
]

# Start an infinite loop to keep playing the song
while True:
    # Call the underlying encapsulated music playback function, passing the note list and meter list to it for automatic performance
    car.Buzzer_play_Music(HAPPY_BIRTHDAY,METER)
    # After the entire song finishes playing, pause the program for 2000 milliseconds (2 seconds) before looping to play it again
    time.sleep_ms(2000)
        