// Define pins
const int trigPin = 13;   // Trigger pin connected to io13
const int echoPin = 12;  // Echo pin connected to io12

// Define variables to store time and distance
long duration;   // Round-trip time of sound wave (microseconds)
int distance;    // Calculated distance (centimeters)

void setup() {
  // Initialize serial communication with baud rate set to 115200 for faster and smoother data display
  Serial.begin(115200);
  
  // Set pin modes
  pinMode(trigPin, OUTPUT); // Trig pin as output to send signals
  pinMode(echoPin, INPUT);  // Echo pin as input to receive signals
}

void loop() {
  // Step 1: Ensure Trig pin is LOW, preparing for transmission
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Wait for 2 microseconds to ensure stable signal

  // Step 2: Send a 10-microsecond HIGH pulse to the Trig pin to trigger the sensor to emit ultrasonic waves
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Step 3: Read the high-level duration of the Echo pin
  // The pulseIn function waits for the pin to go high, then times until the pin goes low
  duration = pulseIn(echoPin, HIGH);

  // Step 4: Calculate the distance
  // The speed of sound in air is approximately 0.034 cm/microsecond
  // Distance = (time × speed) / 2, because the sound makes a round trip
  distance = duration * 0.034 / 2;
    
  // Limit the ultrasonic ranging range, as values beyond 3m or below 2cm are inaccurate
  if(distance < 2 || distance > 300){
    distance = 0;
  }

  // Step 5: Print the result via serial
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait for 1 second before the next measurement to avoid data refreshing too fast to read
  delay(1000);
}