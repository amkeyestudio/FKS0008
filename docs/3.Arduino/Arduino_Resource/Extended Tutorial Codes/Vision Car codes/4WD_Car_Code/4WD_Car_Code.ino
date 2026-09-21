// Define the serial pins for communication with TinyVision
#define ESP_RX_PIN 18  // ESP32S3 receive pin, connected to TinyVision's TX
#define ESP_TX_PIN 17  // ESP32S3 transmit pin, connected to TinyVision's RX

// ================= Pin Definitions =================
// Define motor control pins connected to the motor driver module
#define MOTOR_AIN 40  // Channel A direction control (left wheel) - controls forward/reverse of the left wheel
#define MOTOR_AEN 41  // Channel A PWM control (left wheel) - controls the speed of the left wheel
#define MOTOR_BIN 38  // Channel B direction control (right wheel) - controls forward/reverse of the right wheel
#define MOTOR_BEN 21  // Channel B PWM control (right wheel) - controls the speed of the right wheel

// ================= Motor Control Functions =================

// Set PWM speeds for left and right motors (0-255)
// Parameters: leftSpeed for left wheel, rightSpeed for right wheel
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);   // Write left wheel PWM value; duty cycle determines speed
  ledcWrite(MOTOR_BEN, rightSpeed);  // Write right wheel PWM value; duty cycle determines speed
}

// Forward function
// Parameters: leftSpeed for left wheel, rightSpeed for right wheel
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Set left wheel direction pin HIGH for forward rotation
  digitalWrite(MOTOR_BIN, HIGH);  // Set right wheel direction pin HIGH for forward rotation
  setMotor(leftSpeed, rightSpeed); // Set speeds for both sides
}

// Backward function
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Set left wheel direction pin LOW for reverse rotation
  digitalWrite(MOTOR_BIN, LOW);  // Set right wheel direction pin LOW for reverse rotation
  setMotor(leftSpeed, rightSpeed);
}

// Left turn function (left wheel stops, right wheel moves forward, achieving differential steering)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);   // Left wheel stops (or can be changed to reverse for in-place left turn if needed)
  digitalWrite(MOTOR_BIN, HIGH);  // Right wheel rotates forward
  setMotor(leftSpeed, rightSpeed); // Here leftSpeed actually passes 0 or a smaller value
}

// Right turn function (left wheel moves forward, right wheel stops)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH);  // Left wheel rotates forward
  digitalWrite(MOTOR_BIN, LOW);   // Right wheel stops
  setMotor(leftSpeed, rightSpeed);
}

// Stop motor function
void stopMotor() {
  setMotor(0, 0);  // Set both left and right wheel PWM speeds to 0 to stop the motors
}

void setup() {
  // Initialize hardware serial port Serial1 for communication with TinyVision
  // Parameters: baud rate 9600, data format 8 data bits, no parity, 1 stop bit, RX pin, TX pin
  Serial1.begin(9600, SERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN);
  
  // Initialize USB virtual serial port for connecting to computer to view debug info
  Serial.begin(115200);  
  Serial.println("UNO ready"); // Print startup prompt

  // Set motor direction control pins to output mode
  pinMode(MOTOR_AIN, OUTPUT);
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (Note: This uses the new syntax of ESP32 Arduino Core 3.x)
  // Parameters: pin number, PWM frequency (1000Hz suitable for motor control), resolution (8-bit, value range 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Initial state: Ensure motors are stopped upon power-up to prevent the car from running away wildly
  stopMotor();
  Serial.println("Initialization complete, starting execution!");
}

void loop() {
  // Check if data has arrived in the Serial1 serial buffer
  if (Serial1.available()) {
    // Read a single byte of data (i.e., the control command character)
    char cmd = Serial1.read();

    // Print the received command to the USB debug serial port for easy troubleshooting
    Serial.print("Receive: ");
    Serial.println(cmd);

    // Use switch-case structure to parse commands and execute corresponding actions
    switch (cmd) {
      case 'F':
        // Received 'F', motors move forward
        {
          Serial.println("MOTOR FORWARD");
          forward(200, 200); // Set left and right wheel speeds to 200 (max 255) to prevent abrupt startup
        }
        break;

      case 'B':
        // Received 'B', motors move backward
        {
          Serial.println("MOTOR BACKWARD");
          back(200, 200);
        }
        break;

      case 'L':
        // Received 'L', turn left
        {
          Serial.println("MOTOR LEFT");
          left(200, 200); 
        }
        break;

      case 'R':
        // Received 'R', turn right
        {
          Serial.println("MOTOR RIGHT");
          right(200, 200);
        }
        break;

      case 'S':
        // Received 'S', stop
        {
          Serial.println("MOTOR STOP");
          stopMotor();
        }
        break;

      case 'O':
        // Received 'O', servo open (reserved function)
        Serial.println("SERVO OPEN");
        break;

      case 'C':
        // Received 'C', servo close (reserved function)
        Serial.println("SERVO CLOSE");
        break;

      case '1':
        // Received '1', extension function 1 (reserved function)
        Serial.println("EXT1");
        break;

      case '2':
        // Received '2', extension function 2 (reserved function)
        Serial.println("EXT2");
        break;
        
      default:
        // If an unknown character is received, handling logic can be added here
        break;
    }
  }
  // Short delay of 20ms to reduce CPU usage and act as software debounce for serial reception
  delay(20);
}