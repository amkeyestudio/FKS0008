// Define communication pins, keeping consistent with the wiring table
#define CLK_Pin 9   // Clock pin, connected to module CLK
#define DIN_Pin 8   // Data pin, connected to module DIN

// Smile pattern data (16 bytes, corresponding to 16 columns, each byte has 8 bits for 8 rows)
unsigned char smile[] = {
  0x00, 0x00, 0x1C, 0x02, 0x02, 0x02, 0x5C, 0x40, 
  0x40, 0x5C, 0x02, 0x02, 0x02, 0x1C, 0x00, 0x00
};

// Heart pattern data (added to implement the functionality introduced in the lesson)
unsigned char heart[] = {
    0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x7c,
    0x7c, 0x3e, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  // Set pins to output mode
  pinMode(CLK_Pin, OUTPUT);
  pinMode(DIN_Pin, OUTPUT);
  
  // Initialize default levels to ensure an idle state
  digitalWrite(CLK_Pin, HIGH);
  digitalWrite(DIN_Pin, HIGH);
  
  // Clear screen operation to prevent garbled display on power-up
  unsigned char clear_data[16] = {0};
  matrix_display(clear_data);
}

void loop() {
  // 1. Display the heart pattern for 3 seconds
  matrix_display(heart);
  delay(3000);
  
  // 2. Display the smile pattern for 2 seconds
  matrix_display(smile);
  delay(2000);
  
  // 3. Simple scrolling effect demonstration (cyclically shift data to the left)
  for (int shift = 0; shift < 16; shift++) {
    unsigned char scroll_data[16];
    for (int i = 0; i < 16; i++) {
      // Cyclically left-shift the smile array to achieve a scrolling effect
      scroll_data[i] = smile[(i + shift) % 16]; 
    }
    matrix_display(scroll_data);
    delay(150); // Control scrolling speed, smaller value means faster scrolling
  }
}

// Core function: Write 16 bytes of pattern data to the AiP1640 VRAM
void matrix_display(unsigned char matrix_value[]) {
  // Step 1: Send data command (0x40), set to write data mode, address auto-increment by 1
  IIC_start();
  IIC_send(0x40);
  IIC_end();
  
  // Step 2: Send address command (0xC0), set VRAM start address to 0
  IIC_start();
  IIC_send(0xC0);
  
  // Step 3: Cyclically send 16 bytes of pattern data
  for (int i = 0; i < 16; i++) {
    IIC_send(matrix_value[i]);
  }
  IIC_end();
  
  // Step 4: Send display control command (0x8A), turn on display, set pulse width to 8/16 (maximum brightness)
  IIC_start();
  IIC_send(0x8A);
  IIC_end();
}

// Simulate I2C-like protocol start condition: When CLK is HIGH, DIN transitions from HIGH to LOW
void IIC_start() {
  digitalWrite(CLK_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, LOW);
  delayMicroseconds(3);
}

// Simulate I2C-like protocol data transmission: Change data when CLK is LOW, latch when HIGH
void IIC_send(unsigned char send_data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(CLK_Pin, LOW);  // Pull clock LOW to prepare data
    delayMicroseconds(3);
    
    // Set DIN pin level based on the least significant bit of the byte
    if (send_data & 0x01) {
      digitalWrite(DIN_Pin, HIGH);
    } else {
      digitalWrite(DIN_Pin, LOW);
    }
    delayMicroseconds(3);
    
    digitalWrite(CLK_Pin, HIGH); // Pull clock HIGH to latch data
    delayMicroseconds(3);
    
    send_data = send_data >> 1;  // Right shift data by one bit to prepare for the next bit
  }
}

// Simulate I2C-like protocol stop condition: When CLK is HIGH, DIN transitions from LOW to HIGH
void IIC_end() {
  digitalWrite(CLK_Pin, LOW);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, LOW);
  delayMicroseconds(3);
  digitalWrite(CLK_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN_Pin, HIGH);
  delayMicroseconds(3);
}