// Define the LED connection pin as io11
const int ledPin = 11; 

// Define the brightness variable with an initial value of 0 (dimmest)
int brightness = 0;    

// Define the step size for each change; smaller values mean smoother changes but slower speed
int fadeAmount = 5;    

void setup() {
  // Set ledPin as an output mode
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 1. Set the brightness of the current pin
  // analogWrite can output a PWM signal, with a value range of 0-255
  analogWrite(ledPin, brightness);

  // 2. Change the brightness value for the next iteration
  brightness = brightness + fadeAmount;

  // 3. Check if the boundary of maximum or minimum brightness has been reached
  // If brightness reaches 0 or 255, reverse the direction of change
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount; 
  }

  // 4. Wait for 30 milliseconds so the human eye can see the changing process clearly
  // If this time is too short, the light will flicker too fast; if too long, the breathing effect will feel disconnected
  delay(30);          
}