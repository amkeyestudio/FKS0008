from ESP32_4WDV_Car import keyes_ESP32S3_4WD
from machine import Pin
from ir_rx.nec import NEC_8
import time

SERVO_PIN = 42
IR_RX_PIN = 48

servo =keyes_ESP32S3_4WD()

servo.Servo_init(SERVO_PIN)

servo.Servo_set_angle(90)
time.sleep(1)

def ir_callback(data,addr,ctrl):
    if data < 0:
        print("Holding down...")
    else:
        print("Key value: ", data)

ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), ir_callback)
print("Infrared reception started, please press a remote control button...")


while True:
    
    time.sleep_ms(100)
    
    for angle in range(0, 181, 1):
        servo.Servo_set_angle(angle)
        time.sleep_ms(15)
        
    time.sleep(1)
    for angle in range(180, -1, -1):
        servo.Servo_set_angle(angle)
        time.sleep_ms(15)
        
    time.sleep(1)
    
    
    
    

    
    