#include <IRremote.hpp> // Include the IRremote infrared remote control library

// Define the GPIO number connected to the infrared receiving module signal pin
#define IR_RECEIVE_PIN 48 

void setup()
{
    // Initialize serial communication and set the baud rate to 115200
    Serial.begin(115200);
    delay(1000); // Delay for 1 second to let the serial port stabilize

    Serial.println("ESP32-S3 IR Receiver Start");
    // Initialize the infrared receiver, specify the pin, and disable onboard LED feedback blinking
    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
    Serial.println("Waiting IR...");
}

void loop()
{
    // Check if an infrared signal has been successfully decoded
    if (IrReceiver.decode())
    {
        Serial.println("------ IR DATA ------");
        Serial.print("IR Value:");
        // Print the decoded command code in decimal format
        Serial.println(IrReceiver.decodedIRData.command);
        Serial.println("---------------------");
        
        // Resume receiving state, clear the buffer, and prepare to receive the next infrared signal
        IrReceiver.resume(); 
    }
}