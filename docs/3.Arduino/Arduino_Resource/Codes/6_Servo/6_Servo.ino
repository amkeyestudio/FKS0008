#include <ESP32Servo.h>  // Include the ESP32 servo control library

Servo myServo;           // Create a servo object named myServo

// Define the pin connected to the servo signal line as GPIO42 (please modify according to actual wiring)
const int SERVO_PIN = 42; 

void setup() {
  // Initialize the servo
  myServo.attach(SERVO_PIN);
  myServo.write(90);        // Initialize the servo angle to 90 degrees
  delay(1000);              // Wait for 1000 milliseconds (1 second) for the servo to stabilize
}

void loop() {
  // Phase 1: Smoothly rotate from 0 degrees to 180 degrees
  for (int angle = 0; angle <= 180; angle += 1) { 
    myServo.write(angle);   // Command the servo to turn to the current angle
    delay(15);              // Wait for 15 milliseconds to give the servo time to move
  }

  delay(1000);              // Pause for 1 second upon reaching 180 degrees

  // Phase 2: Smoothly rotate back from 180 degrees to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 1) { 
    myServo.write(angle);   // Command the servo to turn to the current angle
    delay(15);              // Wait 15 milliseconds as well
  }

  delay(1000);              // Pause for another 1 second after returning to 0 degrees
}