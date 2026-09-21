# Import the network module, akin to taking out the "Wi-Fi settings" toolbox from a phone, used to manage Wi-Fi connections
import network
# Import the time module, used to pause/wait (delay) execution, much like counting seconds
import time

# Please modify "your_SSID" to your Wi-Fi name (be sure to keep the English double quotes)
SSID = "your_SSID"
# Please modify "your_PASSWORD" to your Wi-Fi password (be sure to keep the English double quotes)
PASSWORD = "your_PASSWORD"

# Create a WLAN (Wireless Local Area Network) object with interface type STA (Station mode, which connects to a router)
wlan = network.WLAN(network.STA_IF)

# Activate the WLAN interface, equivalent to turning on the phone's Wi-Fi switch
wlan.active(True)

# Print a connection prompt message, which will be displayed in the computer's serial monitor
print("Connecting to WiFi...")

# Connect to the specified Wi-Fi network, providing the previously set name and password to the development board
wlan.connect(SSID, PASSWORD)

# Loop to check the Wi-Fi connection status
# If not connected successfully (isconnected() returns False), it will stay in this loop waiting
while not wlan.isconnected():
    # Delay for 1 second, giving the development board a moment to "handshake" with the router
    time.sleep(1)
    # Print a connecting prompt message so you know it is still trying to connect
    print("Connecting to WiFi...")

# Connection successful, exit the waiting loop above, and the program proceeds downwards
print("Connected to WiFi")

# Retrieve network configuration information; ifconfig() returns a tuple containing 4 items: (IP address, subnet mask, gateway, DNS server)
# We only need the first element (index 0), which is the IP address, and store it in the ip_address variable
ip_address = wlan.ifconfig()[0]

# Print the obtained IP address so you can see the development board's "network house number" on your computer
print("IP Address:", ip_address)
