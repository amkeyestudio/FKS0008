// Define the buzzer control pin as IO10
const int buzzerPin = 10; 

// Define the note frequency array (including low, medium, and high pitches)
// Indices 0-6 are low notes, 7-13 are medium notes, 14-20 are high notes
int doremi[] = {
  262, 294, 330, 349, 392, 440, 494,       // Low Do to Si 
  523, 587, 659, 698, 784, 880, 988,       // Medium Do to Si
  1047, 1175, 1319, 1397, 1568, 1760, 1967 // High Do to Si
};

// Note indices corresponding to the numbered musical notation of Happy Birthday (position numbers in the array, starting from 1)
int happybirthday[] = {
  5, 5, 6, 5, 8, 7, 
  5, 5, 6, 5, 9, 8, 
  5, 5, 12, 10, 8, 7, 6, 
  11, 11, 10, 8, 9, 8
};   

// Rhythm array, values represent relative note lengths
int meter[] = {
  1, 1, 2, 2, 2, 4, 
  1, 1, 2, 2, 2, 4, 
  1, 1, 2, 2, 2, 2, 2, 
  1, 1, 2, 2, 2, 4
};    

void setup() {
  // Set the buzzer pin to output mode
  pinMode(buzzerPin, OUTPUT); 
}

void loop() {
  // Iterate through the score, array length is 25, indices from 0 to 24
  for (int i = 0; i <= 24; i++) {       
    // Get the frequency of the current note (array index needs to minus 1)
    int frequency = doremi[happybirthday[i] - 1];
    
    // Use the tone() function to output a square wave signal of the specified frequency
    tone(buzzerPin, frequency);
    
    // Calculate the delay time based on the rhythm (base rhythm 200ms)
    delay(meter[i] * 200); 
    
    // Stop sound, prepare to play the next note
    noTone(buzzerPin);
    
    // Add a very short pause between notes to make the rhythm clearer
    delay(20); 
  }
  
  // After playing the whole song, pause for 2 seconds before looping
  delay(2000);
}