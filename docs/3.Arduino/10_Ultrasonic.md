# 3.10 Ultrasonic Sensor

## 3.10.1 Lesson Introduction

![IMG_3948](./media/IMG_3948.png)

Have you ever wondered why bats never crash into walls when flying in the pitch dark? Or how dolphins swim so fast and know where schools of fish are ahead? In fact, they all possess a superpower—"echolocation"! They emit a sound we cannot hear, and when the sound bounces back after hitting an object, they can determine the distance to that object.

Today, we are going to equip the ESP32S3 Pro development board with such "super ears"—the HC-SR04 ultrasonic sensor. It acts like a miniature radar, helping us measure the distance of objects ahead. Once you master this, you can build an "anti-collision robot car" that automatically stops when it's about to crash into a wall, or an "intelligent trash can" that automatically opens its lid when a hand approaches.

Congratulations! After completing this lesson, you will not only master a magical sensor, but you will also learn how to make machines "see" the world around them. Are you ready? Let's begin this journey of exploration!

## 3.10.2 Lesson Objectives

*   Correctly wire the HC-SR04 ultrasonic sensor to the ESP32S3 Pro development board.
*   Understand the basic principle of ultrasonic ranging (transmit-receive-calculate time).
*   Write code to read sensor data and view real-time distance values (in centimeters) in the Serial Monitor.
*   Modify parameters in the code to change the measurement frequency or range.

## 3.10.3 Lesson Equipment

| Name               | Specification/Model           | Quantity | Remarks                                                      |
| :----------------- | :---------------------------- | :------- | :----------------------------------------------------------- |
| Main Control Board | ESP32S3 Pro Development Board | 1        | Core Brain                                                   |
| Expansion Board    | ESP32S3 Pro Expansion Board   | 1        | Expansion board with dedicated ultrasonic interface          |
| Ultrasonic Sensor  | HC-SR04                       | 1        | Our "super ears"                                             |
| Connection Cable   | XH2.54 Double-Ended Cable     | 1        | Reverse-proof double-ended cable, effectively preventing incorrect wiring from burning out the sensor |

## 3.10.4 Lesson Principles

### 3.10.4.1 Component Operating Principle: "Listening" to Distance Like a Bat

The HC-SR04 ultrasonic sensor looks somewhat like two big eyes; actually, they are the "mouth" (transmitter) and the "ears" (receiver).

![147f85b698172e819f9496c215a80045](./media/147f85b698172e819f9496c215a80045.png)

Its working process is quite interesting. We can imagine it as shouting out loud in a valley:

1.  **Transmission**: The sensor's "mouth" emits a burst of high-frequency sound waves (40kHz, inaudible to human ears), much like you shouting "Hey—".
2.  **Propagation**: The sound waves travel forward through the air until they hit a wall or object ahead.
3.  **Reflection**: Upon hitting an object, the sound waves bounce back, just like an echo in a valley.
4.  **Reception**: The sensor's "ears" hear this echo.
5.  **Calculation**: The ESP32S3 Pro development board records the time elapsed from when the sound was "shouted" to when it was "heard." Since the speed of sound in air is constant (approximately 340 meters per second), the distance can be calculated as long as the time is known!

The formula is simple: Distance = (Speed of Sound × Time) ÷ 2. Why divide by 2? Because the sound makes a round trip (there and back), and we only want the one-way distance.

### 3.10.4.2 Key Pin Descriptions

The HC-SR04 has four pins, each with a specific task:

*   **VCC**: Positive power supply, connected to 5V to power the sensor.
*   **Trig (Trigger)**: Trigger pin. When we send a high-level signal to this pin (like pressing a switch), the sensor emits a burst of ultrasonic waves.
*   **Echo**: Echo pin. When the sensor receives the echo, this pin outputs a high-level signal. The duration of this high level represents the round-trip time of the sound wave.
*   **GND**: Ground, negative power supply.

### 3.10.4.3 Programming Logic: The `pulseIn()` Function

In our code, we use a magical function called `pulseIn()`. It acts like a precise stopwatch:

*   It starts timing when it detects a high level on the Echo pin.
*   It stops timing when the high level ends (returns to low level).
*   Finally, it tells us how many microseconds (us) the high level lasted.

With this time, we can calculate the distance using mathematical formulas.

## 3.10.5 Wiring Instructions

Before wiring, make sure the ESP32S3 Pro development board is not connected to the computer's USB port, so that even if a wiring mistake is made, components won't burn out. We will use 4 jumper wires to connect the sensor to the development board.

Please note that the IO pins of the ESP32S3 Pro are typically 3.3V logic-compatible, but the HC-SR04 requires a 5V power supply. Fortunately, most GPIO pins of the ESP32S3 Pro can tolerate 5V input signals (fine for short-term experiments), or we can power it directly from the 5V pin while connecting the signal wire to a regular IO port. For simplicity, we follow this standard wiring method:

| Ultrasonic Sensor Pin | ESP32S3 Pro Development Board Pin | Description                                                  |
| :-------------------- | :-------------------------------- | :----------------------------------------------------------- |
| VCC                   | 5V                                | Provides 5V power, make sure not to connect to 3.3V, otherwise it may work unsteadily |
| Trig                  | io13                              | Trigger signal, controlled by the development board to emit ultrasonic waves |
| Echo                  | io12                              | Echo signal, the development board reads the high-level duration here |
| GND                   | GND                               | Ground, completes the circuit                                |

⚠️ **Special Notes**:

1.  **VCC must connect to 5V**: Although the ESP32 is a 3.3V system, the HC-SR04 module has an internal voltage regulator circuit. It is recommended to connect it directly to the development board's 5V pin for optimal performance.
2.  **Do not swap Trig and Echo**: Trig is an input (sent from the development board to the sensor), and Echo is an output (sent from the sensor to the development board).
3.  **Check Connections**: Before uploading the code, double-check the table to ensure each wire is plugged in tightly and corresponds correctly.

![19e2b0fb-87c2-4747-a2e0-70aecfac057e](./media/19e2b0fb-87c2-4747-a2e0-70aecfac057e-1789868448953-24.png)

## 3.10.6 Example Program

This code makes the ESP32S3 Pro emit ultrasonic waves every 1 second, measures the distance of the object ahead, and prints it out through the Serial Monitor. If there is no object ahead or it exceeds the measurement range, it will display "Out of range".

```cpp
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
```

```markdown
## 3.10.7 Code Explanation

**1. Pin Definitions and Variable Declaration**

```cpp
const int trigPin = 13;
const int echoPin = 12;
long duration;
int distance;
```

We store the pin numbers in variables to make wiring modifications easier later. `duration` uses the `long` type because the time may be long; `distance` uses `int` to store whole centimeters.

**2. Setup Initialization**

```cpp
Serial.begin(115200);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
```

Here, the serial communication speed and pin directions are set. Remember that Trig is used to "send commands", so it is an OUTPUT; Echo is used to "listen for commands", so it is an INPUT.

**3. Triggering the Ultrasonic Wave**

```cpp
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
```

This is the standard startup sequence for the HC-SR04: pull low first, then pull high and hold for 10 microseconds, then pull low again. This is like pressing a doorbell to tell the sensor: "Hey, time to work!"

**4. Reading Time and Calculating**

```cpp
duration = pulseIn(echoPin, HIGH);
distance = duration * 0.034 / 2;
```

`pulseIn` is the core function that automatically handles the "stopwatch timing" work for us.
In the calculation formula, `0.034` is the speed of sound (340 m/s = 0.034 cm/microsecond). Dividing by 2 is because the measured time is the total round-trip time of the sound ("there and back"), and we only need the one-way distance.

**5. Range Limitation**

```c
  if(distance < 2 || distance > 300){
    distance = 0;
  }
```

Limit the ranging distance to within 2–300 cm, because ranging data becomes inaccurate if it exceeds 300 cm or falls below 2 cm.

**6. Serial Output**

```cpp
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
```

Print the results to make it easy for us to view them on the computer. `println` adds a newline at the end so that each piece of data occupies its own line, making it look neater.

## 3.10.8 Experimental Phenomenon

1.  **Upload the Code**: Click the upload button in the Arduino IDE and wait for the progress bar to finish.
2.  **Open the Serial Monitor**: Click the magnifying glass icon in the upper right corner, and make sure the baud rate in the lower right corner is set to **115200**.
3.  **Observe the Data**:
    *   If you place your palm about 10 cm in front of the sensor, you should see a value around `Distance: 10 cm` displayed on the screen.
    *   Slowly move your palm away from the sensor, and the numbers will gradually increase.
    *   If there are no objects in front (or the object is beyond 3 meters), you might see `Distance: 0 cm` or a very large random number. This happens because the sound wave is not reflected back, causing a timeout.
4.  **Success Sign**: When you move your hand and the numbers on the screen respond sensitively and change accordingly, it means your "super ears" have been successfully installed! Awesome, you are a little programmer now!

## 3.10.9 Troubleshooting

**Problem: The Serial Monitor shows all 0s or garbled text.**
Cause: The baud rate setting might be incorrect, or the wiring is loose.
Solution: First check whether 115200 is selected in the lower right corner of the Serial Monitor. If it still doesn't work, check if VCC is connected to 5V and GND is properly connected.

**Problem: The distance values jump around significantly and are unstable.**
Cause: Ultrasonic waves are easily affected by absorption from soft objects (such as clothes, curtains), or there is other ultrasonic interference in the environment.
Solution: Try testing facing hard, flat surfaces (such as walls, books). You can also add a simple filtering algorithm in the code (such as taking the average of 5 measurements), but this is advanced content and beginners can ignore it for now.

**Problem: No matter how you move your hand, the distance shows the same fixed value.**
Cause: The Trig or Echo pins might be swapped, or the pin numbers were written incorrectly in the code.
Solution: Check the wiring table to confirm that Trig is connected to io5 and Echo is connected to io18. Check whether the `trigPin` and `echoPin` definitions in the code match the wiring.

**Problem: The sensor heats up severely.**
Cause: VCC and GND were reversed, causing a short circuit.
Solution: **Immediately disconnect the USB connection!** Check if the red wire is connected to VCC and the black wire is connected to GND. Reconnect power only after confirming everything is correct.

### 3.10.10 Safety Tips

*   **Strictly Prohibit Short Circuits**: Be very careful when wiring, do not let VCC and GND touch directly.
*   **Do Not Stare Directly**: Although ultrasonic waves are invisible to the human eye, do not point them closely at your ears for long periods of time. Although the power is very low, maintaining good habits is important.
*   **Power-Off Operation**: When modifying the wiring, please unplug the USB cable first to protect your ESP32S3 Pro development board.