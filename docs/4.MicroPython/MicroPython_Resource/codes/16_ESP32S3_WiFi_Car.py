# ==================== Import Modules ====================
# Import the network module to connect to Wi-Fi
import network          
# Import the socket module to create an HTTP server (i.e., a tiny website)
import socket           
# Import the time module to pause the program (delay)
import time             
# Import the car control class and webpage code from our own ESP32S3_4WD_Car file
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD, INDEX_HTML
#from webpage import INDEX_HTML               # Import webpage content from webpage.py (this line is commented out and not used for now)

# Create a "car" object, equivalent to naming the car 'car', which will be used to control it from now on
car = Keyes_ESP32S3_4WD()

# ==================== WiFi Configuration ====================
# Fill in the name of the WiFi you want to connect to, e.g., "FKS0008"
SSID = "FKS0008"         
# Fill in the password of the WiFi you want to connect to, e.g., "88888888"
PASSWORD = "88888888"   

# ==================== Motor Pin Definitions ====================
# Define the interface number connected to the left wheel direction control as 40
MOTOR_AIN = 40          
# Define the interface number connected to the left wheel speed control as 41
MOTOR_AEN = 41          
# Define the interface number connected to the right wheel direction control as 38
MOTOR_BIN = 38          
# Define the interface number connected to the right wheel speed control as 21
MOTOR_BEN = 21          

# Tell the car the 4 interface numbers defined above to initialize the motors
car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)

# Speed parameter: set the base driving speed of the car, ranging from 0 to 255, set to 220 here
BASE_SPEED = 220  

# ==================== Ultrasonic Pin Definitions ====================
# Define the interface number connected to the ultrasonic trigger signal as 13
TRIG_PIN = 13           
# Define the interface number connected to the ultrasonic echo signal as 12
ECHO_PIN = 12           

# Tell the car the ultrasonic interface numbers to initialize the ultrasonic module
car.Ultrasonic_init(TRIG_PIN,ECHO_PIN)

# ==================== Servo Pin and State ====================
# Define the interface number connected to the servo signal line as 42
SERVO_PIN = 42          

# Tell the car the servo interface number to initialize the servo
car.Servo_init(SERVO_PIN)

# ==================== Command Handling Function ====================
# Define a "handle command" function to execute corresponding actions based on the received move parameter
def handle_cmd(move):
    """Execute corresponding actions based on the move parameter"""
    # If the received command is "forward"
    if move == "forward":
        # Make the car move forward at a speed of 200, 200
        car.forward(200, 200)
    # If the received command is "backward"
    elif move == "backward":
        # Make the car move backward at a speed of 200, 200
        car.back(200, 200)
    # If the received command is "left"
    elif move == "left":
        # Make the car turn left at a speed of 200, 200
        car.left(200, 200)
    # If the received command is "right"
    elif move == "right":
        # Make the car turn right at a speed of 200, 200
        car.right(200, 200)
    # If the received command is "stop"
    elif move == "stop":
        # Stop the motor rotation
        car.stop_motor()
        # Return the servo to the 90-degree middle position when stopping
        car.Servo_set_angle(90)                      
    # If the received command is "servo_plus" (increase servo angle)
    elif move == "servo_plus":
        # Turn the servo to the maximum angle of 180 degrees
        car.Servo_set_angle(180)                     
    # If the received command is "servo_minus" (decrease servo angle)
    elif move == "servo_minus":
        # Turn the servo to the minimum angle of 0 degrees
        car.Servo_set_angle(0)                       
    # If the received command is "claw_open" (open mechanical claw)
    elif move == "claw_open":
        # Turn the servo to 0 degrees to open the mechanical claw
        car.Servo_set_angle(0)                       
    # If the received command is "claw_close" (close mechanical claw)
    elif move == "claw_close":
        # Turn the servo to 180 degrees to close the mechanical claw
        car.Servo_set_angle(180)                     

# ==================== Connect to WiFi ====================
# Create a Wi-Fi Station (STA) interface, meaning the development board acts as a device to connect to a router
wlan = network.WLAN(network.STA_IF)              
# Activate this Wi-Fi interface to make it work
wlan.active(True)                                
# Print a prompt message on the screen telling the user which WiFi is being connected to
print("Connecting to WiFi:", SSID)
# Connect to WiFi using the name and password defined earlier
wlan.connect(SSID, PASSWORD)                     
# Keep looping and waiting as long as it is not successfully connected
while not wlan.isconnected():                    
    # Pause for 0.5 seconds each time to prevent the program from running too fast and freezing
    time.sleep(0.5)
    # Print a dot on the screen indicating that it is trying hard to connect
    print(".", end="")

# Print a newline so that subsequent text appears on the next line
print()
# Print a prompt that connection was successful
print("WiFi connected successfully!")
# Print the IP address acquired by the development board (its house number in the network)
print("IP address:", wlan.ifconfig()[0])

# ==================== Start HTTP Server ====================
# Create a network socket (equivalent to establishing a communication channel) using IPv4 and TCP protocols
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Set socket options to allow address reuse and prevent errors when restarting the program
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# Bind the socket to port 80 (80 is the default port number for web pages)
s.bind(('', 80))                                 
# Start listening, allowing a maximum of 5 devices to connect simultaneously
s.listen(5)                                      
# Print a prompt telling the user that the web server is ready
print("Web server started!")

# ==================== Main Loop ====================
# This is an infinite loop that keeps the server running and constantly waiting for requests from phones
while True:
    try:
        # Wait for a phone connection; once connected, get the connection object conn and the phone address addr
        conn, addr = s.accept()                              
        # Read data sent by the phone from the connection (up to 1024 bytes) and decode it into text
        request = conn.recv(1024).decode('utf-8')            

        # Parse the first line of the request, e.g.: GET /cmd?move=forward HTTP/1.1
        first_line = request.split('\r\n')[0]
        # Split the first line by spaces into multiple parts
        parts = first_line.split(' ')
        # Extract the requested path (e.g., "/" or "/cmd?move=forward")
        path = parts[1] if len(parts) >= 2 else "/"

        # Route matching: If the requested path is the root path (homepage)
        if path == "/" or path == "/index.html":
            # Assemble an HTTP response containing the webpage content (INDEX_HTML)
            response = ('HTTP/1.1 200 OK\r\n'
                        'Content-Type: text/html; charset=UTF-8\r\n'
                        'Connection: close\r\n\r\n' + INDEX_HTML)
            # Encode the response and send it to the phone
            conn.send(response.encode('utf-8'))

        # Route matching: If the requested path is a control command path (starting with /cmd)
        elif path.startswith("/cmd"):
            # Initialize an empty move variable to store the action command
            move = ""
            # If the path contains a question mark (indicating parameters are attached)
            if "?" in path:
                # Extract the parameter part after the question mark
                query = path.split("?", 1)[1]
                # Split parameters by the & symbol and check them one by one
                for param in query.split("&"):
                    # If the parameter starts with "move="
                    if param.startswith("move="):
                        # Extract the specific action (e.g., forward) after "move="
                        move = param.split("=", 1)[1]
            # Call the function defined earlier to execute the extracted action
            handle_cmd(move)
            # Assemble a successful text response telling the phone that the command has been executed
            response = ('HTTP/1.1 200 OK\r\n'
                        'Content-Type: text/plain\r\n'
                        'Connection: close\r\n\r\nOK')
            # Send the response to the phone
            conn.send(response.encode('utf-8'))

        # Route matching: If the requested path is the distance measurement path (/distance)
        elif path == "/distance":
            # Call the ultrasonic module to measure distance and store the result in the dist variable
            dist = car.Ultrasonic_measure_distance()
            # Assemble a text response containing the distance data
            response = ('HTTP/1.1 200 OK\r\n'
                        'Content-Type: text/plain\r\n'
                        'Connection: close\r\n\r\n' + str(dist))
            # Send the response to the phone
            conn.send(response.encode('utf-8'))

        # Route matching: If any other unrecognized path is requested
        else:
            # Assemble a 404 Not Found error response
            response = ('HTTP/1.1 404 Not Found\r\n'
                        'Content-Type: text/plain\r\n'
                        'Connection: close\r\n\r\nNot Found')
            # Send the error response to the phone
            conn.send(response.encode('utf-8'))

    # Catch any errors that occur during the above execution process
    except Exception as e:
        # Print the error message on the screen for troubleshooting
        print("Error:", e)
    # Execute this step finally, regardless of success or failure
    finally:
        # Close this connection, release resources, and get ready for the next connection
        conn.close()
