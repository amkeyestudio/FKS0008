// Define the pin number connected to the LED, here we use io11
const int ledPin = 11; 

void setup() {
  // Initialize serial communication with a baud rate of 9600, making it easy to see messages on the computer
  Serial.begin(9600);
  
  // Set ledPin (io42) to output mode because it needs to control the LED on and off
  pinMode(ledPin, OUTPUT);
  
  // Print a message to the serial monitor to tell us the program has started running
  Serial.println("LED Blink Experiment Started!");
}

void loop() {
  // 1. Turn on the LED
  digitalWrite(ledPin, HIGH); 
  Serial.println("LED is ON"); // Display "LED is ON" on the computer screen
  
  // 2. Wait for 1000 milliseconds (which is 1 second)
  delay(1000); 
  
  // 3. Turn off the LED
  digitalWrite(ledPin, LOW);  
  Serial.println("LED is OFF"); // Display "LED is OFF" on the computer screen
  
  // 4. Wait for another 1000 milliseconds (1 second)
  delay(1000); 
}