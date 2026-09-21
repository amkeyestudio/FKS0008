# From the pre-prepared "toolbox" (ESP32S3_4WD_Car library), grab the "magic book" specifically designed to control this development board (Keyes_ESP32S3_4WD class)
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD

# Import the "time" toolbox so we can make the program "wait" or "sleep for a while" (delays)
import time

# Create a variable named matrix, granting it superpowers to control the development board
matrix = Keyes_ESP32S3_4WD()

# Define communication pins, keeping them completely consistent with our physical wiring table
CLK_PIN = 9   # Clock pin, connects to the CLK pin on the module (conductor's baton)
DIN_PIN = 8   # Data pin, connects to the DIN pin on the module (megaphone)

# Tell the "magic book" which two pins we want to use to communicate with the dot matrix module
matrix.Matrix_init(CLK_PIN, DIN_PIN)


# Define data for the "smile" pattern (16 bytes, corresponding to 16 columns, where each byte's 8 bits correspond to 8 rows)
# These numbers starting with 0x are the "pattern passwords" we generated in the modulus tool
smile = bytes([
    0x00, 0x00, 0x1C, 0x02, 0x02, 0x02, 0x5C, 0x40,
    0x40, 0x5C, 0x02, 0x02, 0x02, 0x1C, 0x00, 0x00
])

# Define data for the "heart" pattern
heart = bytes([
    0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x7c,
    0x7c, 0x3e, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00
])


# Initialize screen-clearing operation, sending 16 zero-bytes to turn off all lights and prevent garbled displays upon initial power-up
matrix.Matrix_display(bytes(16))       

# Main loop: makes the code run in circles, continuously repeating the following content
while True:
    # 1. Display the heart pattern for 3 seconds
    matrix.Matrix_display(heart)       # Send the "heart password" to the dot matrix module to display a heart
    time.sleep(3)               # Make the program "sleep" for 3 seconds (during which the heart stays lit)

    # 2. Display the smile pattern for 2 seconds
    matrix.Matrix_display(smile)       # Send the "smile password" to the dot matrix module, replacing the heart
    time.sleep(2)               # Make the program "sleep" for 2 seconds (during which the smile stays lit)

    # 3. Simple scrolling effect demonstration (calls the built-in scroll function to cyclically shift the smile data to the left)
    matrix.Matrix_Scroll_display(smile)