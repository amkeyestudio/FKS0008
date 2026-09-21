#include <IRremote.hpp>  // Include the IRremote infrared remote control library

// Define GPIO48 connected to the signal pin of the infrared receiving module
#define IR_RECEIVE_PIN 48

// ================= Motor Pin Definitions =================
#define MOTOR_AIN 40  // Channel A direction control (Left wheel)
#define MOTOR_AEN 41  // Channel A PWM control (Left wheel speed)
#define MOTOR_BIN 38  // Channel B direction control (Right wheel)
#define MOTOR_BEN 21  // Channel B PWM control (Right wheel speed)

void setup() {
  // Initialize serial communication with a baud rate of 115200
  Serial.begin(115200);
  delay(1000);  // Delay for 1 second to wait for the serial port to stabilize

  Serial.println("ESP32-S3 Infrared Receiver Initialized");
  // Initialize the infrared receiver, specify the pin, and disable onboard LED feedback blinking
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  Serial.println("Waiting for infrared signals...");

  // Set motor direction pins as output mode
  pinMode(MOTOR_AIN, OUTPUT); 
  pinMode(MOTOR_BIN, OUTPUT);

  // Configure PWM channels (ESP32 Arduino Core 3.x syntax)
  // Parameters: pin, frequency (1000Hz), resolution (8-bit, i.e., 0-255)
  ledcAttach(MOTOR_AEN, 1000, 8);
  ledcAttach(MOTOR_BEN, 1000, 8);

  // Ensure motors are stopped in the initial state
  stopMotor();
}

void loop() {
  // Check if an infrared signal has been successfully decoded
  if (IrReceiver.decode()) {
    Serial.println("------ Infrared Data ------");
    
    // Get the decoded 8-bit command code
    int irCommand = IrReceiver.decodedIRData.command;
    
    Serial.print("IR Value:");
    Serial.println(irCommand); 
    Serial.println("---------------------");
    
    // Execute corresponding actions based on the 8-bit command code
    // Note: Command codes may vary for different remote controls; modify according to the actual values printed on the serial monitor
    switch (irCommand) {
      case 70: // Corresponds to the 'Up' key on the remote control (Decimal 70)
        forward(200, 200); 
        break;
      case 21: // Corresponds to the 'Down' key on the remote control (Decimal 21)
        back(200, 200); 
        break;
      case 68: // Corresponds to the 'Left' key on the remote control (Decimal 68)
        left(200, 200); 
        break;
      case 67: // Corresponds to the 'Right' key on the remote control (Decimal 67)
        right(200, 200); 
        break;
      case 64: // Corresponds to the 'OK' key on the remote control (Decimal 64)
        stopMotor(); 
        break;
      default:
        // Do nothing if an undefined key is pressed
        break;
    }

    // Resume the receiving state, clear the buffer, and prepare to receive the next infrared signal
    // This step is critical; without it, the receiver will stop working after processing one signal
    IrReceiver.resume();
  }
}

// ================= Motor Control Functions =================

// Set PWM speed for left and right motors (0-255)
void setMotor(int leftSpeed, int rightSpeed) {
  ledcWrite(MOTOR_AEN, leftSpeed);  // Write PWM value for left wheel
  ledcWrite(MOTOR_BEN, rightSpeed); // Write PWM value for right wheel
}

// Move Forward
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Move Backward
void back(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel reverse
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel reverse
  setMotor(leftSpeed, rightSpeed);
}

// Turn Left (Left wheel stops, right wheel moves forward)
void left(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, LOW);  // Left wheel stops
  digitalWrite(MOTOR_BIN, HIGH); // Right wheel forward
  setMotor(leftSpeed, rightSpeed);
}

// Turn Right (Left wheel moves forward, right wheel stops)
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_AIN, HIGH); // Left wheel forward
  digitalWrite(MOTOR_BIN, LOW);  // Right wheel stops
  setMotor(leftSpeed, rightSpeed);
}

// Stop Motors
void stopMotor() {
  setMotor(0, 0); // Set speed to 0
}