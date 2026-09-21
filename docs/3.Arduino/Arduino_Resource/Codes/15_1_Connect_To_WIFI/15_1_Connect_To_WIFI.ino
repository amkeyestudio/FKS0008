/*
  Project Name: Print Wi-Fi IP Address
  Author: Keyestudio
  Description: Introduces how to use ESP32S3 to connect to Wi-Fi and print the IP address of the ESP32S3
*/

// Import the Wi-Fi library file
#include <WiFi.h>

// Please modify "your_SSID" to your Wi-Fi name
const char* ssid = "your_SSID";
// Please modify "your_PASSWORD" to your Wi-Fi password
const char* password = "your_PASSWORD";

void setup() {
  // Initialize serial communication and set baud rate to 115200
  Serial.begin(115200);
  
  // Initialize Wi-Fi connection, passing in SSID and password
  WiFi.begin(ssid, password);
  
  // Periodically check the Wi-Fi connection status
  // If not connected successfully (status is not WL_CONNECTED), keep looping in the connecting state
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Delay for 1 second
    Serial.println("Connecting to WiFi..."); // Print connection prompt message
  }
  
  // Connection successful, exit the while loop
  Serial.println("Connected to WiFi"); // Print connection success message
  // Print the obtained local IP address
  Serial.println(WiFi.localIP()); 
}

void loop() {
  // The main loop is empty because the connection operation only needs to be executed once in setup
}