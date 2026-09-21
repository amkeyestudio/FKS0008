/*
  Project Name: Web Page Display
  Author: Keyestudio
  Description: Introduces how to use ESP32S3 to connect to Wi-Fi and set up a web page displaying "Hello World"
*/

#include <WiFi.h>
#include <esp_http_server.h> // Include the ESP32 HTTP server library

// Please replace the following information with your network credentials
const char *ssid = "your_SSID";          // Change to your Wi-Fi name
const char *password = "your_PASSWORD";  // Change to your Wi-Fi password

httpd_handle_t web_httpd = NULL;  // HTTP server handle for starting and managing the server

// Simplified HTML page containing only the title content
// Using R"rawliteral(...)rawliteral" preserves multi-line strings as-is without escaping quotes
static const char INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 Web Server</title>
  </head>
  <body>
    <h1>Hello World</h1>  <!-- Title text in the page body -->
    <p>This web page is provided by ESP32S3 Pro.</p>
  </body>
</html>
)rawliteral";

// Callback function to handle requests for the root URL ("/")
static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");  // Set response content type to HTML
  // Send the HTML response containing the title
  return httpd_resp_send(req, INDEX_HTML, strlen(INDEX_HTML));  
}

// Start the HTTP server and register the request handler for the root path ("/")
void startWebServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();  // Use default HTTP configuration
  config.server_port = 80;  // Set the HTTP server listening port to 80 (default HTTP port)

  // Configure request handling rules for the root path ("/")
  httpd_uri_t index_uri = {
    .uri = "/",               // Set the request URI path to "/"
    .method = HTTP_GET,       // Set the request method to GET
    .handler = index_handler, // Set the callback function for handling requests
    .user_ctx = NULL          // No extra context data
  };

  // Start the HTTP server
  if (httpd_start(&web_httpd, &config) == ESP_OK) {
    // Register the URI handler, binding the root path to the callback function
    httpd_register_uri_handler(web_httpd, &index_uri);  
  }
}

void setup() {
  Serial.begin(115200);  // Start the serial monitor and set the baud rate to 115200

  // Connect to the Wi-Fi network
  WiFi.begin(ssid, password);  // Start Wi-Fi connection
  // Wait continuously if Wi-Fi is not connected
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);  // Delay for 500ms each time
    Serial.print(".");  // Print a dot on the serial monitor to indicate connection attempt
  }
  Serial.println("");  // New line
  Serial.println("WiFi connected");  // Print message when Wi-Fi is successfully connected
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());   // Print the assigned IP address
  
  // Start the HTTP server
  startWebServer();  // Start the web server and register handlers
  Serial.println("HTTP server started");
}

void loop() {
  // The main loop is empty because the HTTP server handles requests in the background via interrupts/tasks
}