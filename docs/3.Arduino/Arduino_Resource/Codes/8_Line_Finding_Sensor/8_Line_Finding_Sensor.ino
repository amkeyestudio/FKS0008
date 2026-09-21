#include <Wire.h>       // Include Arduino's built-in I2C (Wire) communication library
#include <PCF8574.h>    // Include the driver library for the PCF8574 I2C expansion chip

// Instantiate the PCF8574 object and set its I2C address to 0x20 (the default address for this module)
PCF8574 pcf8574(0x20);

// Define the output pins for the 5-channel line-tracking sensor, corresponding to P0-P4 of the PCF8574
#define OUTA P4  // Probe A (far left)
#define OUTB P0  // Probe B (left)
#define OUTC P1  // Probe C (middle)
#define OUTD P2  // Probe D (right)
#define OUTE P3  // Probe E (far right)

void setup()
{
    // Initialize serial communication and set the baud rate to 115200
    Serial.begin(115200);
    Serial.println("5-channel line-tracking sensor initialization complete!");

    // Configure the 5 pins of the PCF8574 as input mode to read sensor states
    pcf8574.pinMode(OUTA, INPUT);
    pcf8574.pinMode(OUTB, INPUT);
    pcf8574.pinMode(OUTC, INPUT);
    pcf8574.pinMode(OUTD, INPUT);
    pcf8574.pinMode(OUTE, INPUT);

    // Start I2C communication
    pcf8574.begin();
}

void loop()
{
    // Read the states of the 5 probes sequentially and print them via serial, separated by spaces
    Serial.print(pcf8574.digitalRead(OUTA));
    Serial.print(" ");
    Serial.print(pcf8574.digitalRead(OUTB));
    Serial.print(" ");
    Serial.print(pcf8574.digitalRead(OUTC));
    Serial.print(" ");
    Serial.print(pcf8574.digitalRead(OUTD));
    Serial.print(" ");
    Serial.println(pcf8574.digitalRead(OUTE)); // Print the last probe with println and wrap to a new line

    // Delay for 500 milliseconds to control the data refresh frequency for easy visual observation
    delay(500);
}