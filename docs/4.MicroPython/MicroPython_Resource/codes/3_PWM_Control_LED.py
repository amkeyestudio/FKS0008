# Import the Pin and PWM classes from the machine module; Pin is used to specify pins, and PWM is used to output adjustable-brightness pulses
from machine import Pin, PWM
# Import the time module to control the delay speed of the breathing light
import time

# Create a PWM object using GPIO11, with the frequency set to 1000Hz (higher frequency reduces LED flicker perception)
led = PWM(Pin(11), freq=1000)
# Set the duty cycle to 0, starting in the off state (duty cycle range is 0~65535, where 0 is darkest and 65535 is brightest)
led.duty_u16(0)

# Infinite loop to keep the breathing light running continuously
while True:
    # Brightening loop: Duty cycle starts from 0, increasing by 256 each time until close to 65535
    for duty in range(0, 65536, 256):
        # Write the current duty cycle to PWM to control LED brightness
        led.duty_u16(duty)
        # Delay for 5 milliseconds; larger values make the breathing slower, smaller values make it faster
        time.sleep_ms(5)

    # Ensure the maximum brightness of 65535 is reached
    led.duty_u16(65535)
    # Maintain full brightness state for 200 milliseconds
    time.sleep_ms(200)

    # Dimming loop: Duty cycle starts from 65535, decreasing by 256 each time until close to 0
    for duty in range(65535, -1, -256):
        # Write the current duty cycle to PWM to control LED brightness
        led.duty_u16(duty)
        # Delay for 5 milliseconds, keeping the same speed as the brightening phase
        time.sleep_ms(5)

    # Ensure the minimum brightness of 0 is reached, completely turning off
    led.duty_u16(0)
    # Maintain fully off state for 200 milliseconds
    time.sleep_ms(200)