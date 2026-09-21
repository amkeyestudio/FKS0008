// ================= Ultrasonic Pin Definitions =================
const int trigPin = 13;  // Trigger pin connected to IO13
const int echoPin = 12;  // Echo pin connected to IO12

// ================= Distance Threshold Settings =================
// Set the "boundary lines" for distances (Unit: cm)
const long safeDistance = 30;   // Define the upper limit of safe distance as 30 cm. Stop or reverse below this distance.
const long closeDistance = 10;  // Define the lower limit of danger distance as 10 cm. Reverse below this distance.
const long maxDistance = 50;    // Define the maximum following distance as 50 cm. Stop the car if it exceeds this distance to prevent it from running away.

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Path A direction control (Left wheel)
#define MOTOR_AEN 41  // Path A PWM control (Left wheel speed)
#define MOTOR_BIN 38  // Path B direction control (Right wheel)
#define MOTOR_BEN 21  // Path B PWM control (Right wheel speed)

// ================= Initialization Function =================
void setup() {
  // Initialize serial communication with a baud rate of 115200 for faster and smoother data display
  Serial.begin(115200);
  Serial.println("System initialization complete, ultrasonic following car started!");

  // Set ultrasonic pin modes
  pinMode(trigPin, OUTPUT);  // Trig pin as output to send trigger signals
  pinMode(echoPin, INPUT);   // Echo pin as input to receive echo signals

  // Set motor direction pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);
  
  // Ensure motors are stopped in the initial state
  stopMotor();
}

// ================= Main Loop Function =================
void loop() {
  // 1. Get the front distance
  long distance = getDistance();  // Call the ranging function and store the calculated distance in the distance variable.

  // 2. Print the distance in the Serial Monitor for debugging and observation.
  Serial.print("Current Distance: ");  // Print prompt text.
  Serial.print(distance);              // Print specific distance number.
  Serial.println(" cm");               // Print unit " cm" (centimeters) and newline.

  // 3. Control car actions based on distance (core logic judgment)
  if (distance > safeDistance && distance <= maxDistance) {  
    // If the measured distance is greater than 30 cm AND less than or equal to 50 cm
    // Distance is appropriate, chase after it!
    forward(150, 150);  // Call the "forward" function, set left and right wheel speeds to 150.
    
  } else if ((distance >= closeDistance && distance <= safeDistance) || distance > maxDistance) {  
    // Otherwise, if the distance is between 10 cm and 30 cm, OR the distance is greater than 50 cm
    // Maintain a proper following distance, or exceed the max range to prevent losing track
    stopMotor();  // Call the "stop" function.
    
  } else if (distance < closeDistance && distance > 0) {  
    // Otherwise, if the distance is less than 10 cm AND greater than 0 (excluding 0 or negative values caused by sensor failures)
    // Distance is too close, back up quickly!
    back(105, 150);  // Call the "back" function, left wheel 105, right wheel 150 (can be fine-tuned to prevent veering off course).
  }

  // Slight delay (pause) of 100 milliseconds (0.1 seconds).
  // Makes movements smoother, prevents spamming commands, and gives the sensor some "breathing room" to receive echoes.
  delay(100);
}

// ================= Ultrasonic Ranging Function =================
/*
 * Function Purpose: Encapsulate ultrasonic ranging code into a function to simplify the main loop code
 * Function Name: getDistance
 * Function Feature: Returns an integer distance value (Unit: cm)
*/
int getDistance() {
  // Define variables to store time and distance
  long duration;  // Round-trip time of sound waves (microseconds)
  int distance;   // Calculated distance (centimeters)

  // Step 1: Ensure Trig pin is LOW, preparing to transmit
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // Wait for 2 microseconds to ensure signal stability

  // Step 2: Give the Trig pin a 10-microsecond HIGH pulse to trigger the sensor to emit ultrasonic waves
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Step 3: Read the duration of the HIGH level on the Echo pin
  // The pulseIn function waits for the pin to go HIGH, then starts timing until the pin goes LOW
  duration = pulseIn(echoPin, HIGH);

  // Step 4: Calculate distance
  // The speed of sound in air is approximately 0.034 cm/microsecond
  // Distance = (Time × Speed) / 2, because the sound makes a round trip
  distance = duration * 0.034 / 2;

  // Limit the ultrasonic ranging range because values beyond 3 meters or below 2 cm are inaccurate
  if (distance < 2 || distance > 300) {
    distance = 0; // Out-of-range values are treated as invalid data, returning 0
  }
  
  return distance;
}

// ================= Motor Control Functions =================

// Set PWM speeds for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write left wheel PWM value
  ledcWrite(MOTOR_BEN, rightSpeed); // Write right wheel PWM value
}

// Move Forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward rotation
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward rotation
  setMotor(leftSpeed, rightSpeed);
}

// Move Backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse rotation
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse rotation
  setMotor(leftSpeed, rightSpeed);
}

// Stop Motors
void stopMotor() {
  setMotor(0, 0); // Set speeds to 0
}