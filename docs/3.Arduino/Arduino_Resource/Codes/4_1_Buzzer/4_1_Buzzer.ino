// Define the buzzer control pin as IO10
const int buzzerPin = 10; 

// Custom function to play notes, encapsulating sound generation, delay, and stop logic to make the code cleaner
void playNote(int pin, int frequency, int duration) {
  tone(pin, frequency);       // Generate sound at the specified frequency
  delay(duration);            // Sustain for the specified duration
  noTone(pin);                // Stop sound generation
  delay(50);                  // Short pause between notes to make the sound clearer
}

void setup() {
  // Initialize serial communication with a baud rate of 115200 for debugging output
  Serial.begin(115200);
  // Set the buzzer pin to output mode
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  Serial.println("Playing Alarm...");
  // 1. Alarm stage: Produce short, sharp "beep-beep-beep" sounds
  for (int i = 0; i < 3; i++) {
    playNote(buzzerPin, 1000, 200); // 1000Hz sharp sound, lasting 200ms
    delay(200);                     // Pause for 200ms
  }
  
  // 2. Rest stage
  Serial.println("Resting...");
  delay(1000); // Quiet for 1 second

  Serial.println("Playing Scale...");
  // 3. Music stage: Play the Do-Re-Mi-Fa-Sol-La-Si scale
  playNote(buzzerPin, 262, 500); // Low Do
  playNote(buzzerPin, 294, 500); // Low Re
  playNote(buzzerPin, 330, 500); // Low Mi
  playNote(buzzerPin, 349, 500); // Low Fa
  playNote(buzzerPin, 392, 500); // Low Sol
  playNote(buzzerPin, 440, 500); // Low La
  playNote(buzzerPin, 494, 500); // Low Si
  
  // 4. Long rest before looping
  delay(2000); // Quiet for 2 seconds before restarting
}