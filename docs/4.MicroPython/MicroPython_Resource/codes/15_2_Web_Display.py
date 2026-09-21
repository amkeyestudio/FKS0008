# Import the network module to manage Wi-Fi connections
import network
# Import the socket module to create a TCP server (equivalent to establishing a communication channel)
import socket
# Import the time module for delays
import time

# Please replace the following information with your network credentials (your Wi-Fi name and password)
SSID = "your_SSID"          # Change to your Wi-Fi name
PASSWORD = "your_PASSWORD"  # Change to your Wi-Fi password

# Define the HTML page content to be displayed, using triple quotes to preserve multi-line formatting; this is akin to the "letter content" we want to send to the browser
HTML = """<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 Web Server</title>
  </head>
  <body>
    <h1>Hello World</h1>
    <p>This is a web page provided by ESP32S3 Pro.</p>
  </body>
</html>
"""

# ==================== Connect to Wi-Fi ====================
# Create a WLAN object with interface type STA (Station mode, i.e., connecting to a router)
wlan = network.WLAN(network.STA_IF)
# Activate the WLAN interface, turning on the Wi-Fi switch
wlan.active(True)
# Connect to the specified Wi-Fi network
wlan.connect(SSID, PASSWORD)

# Print connection prompt
print("Connecting to WiFi...")
# Loop to check connection status until successfully connected
while not wlan.isconnected():
    # Delay for 500 milliseconds (0.5 seconds) to allow faster feedback during the connection process
    time.sleep(0.5)
    # Print a dot to indicate an ongoing connection attempt, without a newline
    print(".", end="")

# Connection successful, print a newline and message
print()
print("WiFi connected")
# Get and print the IP address; ifconfig()[0] extracts the IP address
print("IP Address:", wlan.ifconfig()[0])

# ==================== Start HTTP Server ====================
# Create a TCP socket using IPv4 and TCP protocols, akin to getting a telephone
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Set address reuse to avoid port occupation after restart, akin to allowing a phone to be called right after hanging up
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# Bind to port 80 (the default HTTP port) across all network interfaces. Port 80 is the "default front door" for web access
s.bind(('', 80))
# Start listening with a maximum backlog of 5 connections, akin to waiting by the phone with a maximum queue of 5 people
s.listen(5)

# Print server startup information
print("HTTP server started")

# Main loop to continuously handle client requests. This is an infinite loop that keeps the server running without resting
while True:
    # Wait for a client connection, returning the connection object (conn) and client address (addr), akin to answering a phone call
    conn, addr = s.accept()
    # Print the client address to see who is visiting us
    print("Client connected from", addr)
    try:
        # Receive client request data (up to 1024 bytes), listening to what the other party said over the phone
        request = conn.recv(1024)
        # Construct the HTTP response header, specifying the content type as HTML, encoding as UTF-8, and appending our HTML web content
        response = 'HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: close\r\n\r\n' + HTML
        # Send the response content encoded in UTF-8, "speaking" the web content to the browser
        conn.send(response.encode('utf-8'))
    except Exception as e:
        # If an exception occurs (e.g., a sudden network drop), print the error message
        print("Error:", e)
    finally:
        # Close the client connection, akin to hanging up the phone and getting ready for the next call
        conn.close()