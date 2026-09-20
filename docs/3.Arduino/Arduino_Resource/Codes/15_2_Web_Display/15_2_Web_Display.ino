/*
  项目名称: 网页显示
  作者: Keyestudio
  描述: 介绍如何使用 ESP32S3 连接 Wi-Fi 并且搭建网页显示 "Hello World"
*/

#include <WiFi.h>
#include <esp_http_server.h> // 引入 ESP32 HTTP 服务器库

// 请将以下信息替换为您的网络凭据
const char *ssid = "your_SSID";          // 改成您的 Wi-Fi 名称
const char *password = "your_PASSWORD";  // 改成您的 Wi-Fi 密码

httpd_handle_t web_httpd = NULL;  // HTTP 服务器句柄，用于启动和管理服务器

// 简化后的 HTML 页面，仅包含标题内容
// 使用 R"rawliteral(...)rawliteral" 可以原样保留多行字符串，无需转义引号
static const char INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 Web Server</title>
  </head>
  <body>
    <h1>Hello World</h1>  <!-- 页面正文中的标题文本 -->
    <p>这是由 ESP32S3 Pro 提供的网页。</p>
  </body>
</html>
)rawliteral";

// 处理根 URL（"/"）请求的回调函数
static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");  // 设置响应的内容类型为 HTML
  // 发送包含标题的 HTML 响应
  return httpd_resp_send(req, INDEX_HTML, strlen(INDEX_HTML));  
}

// 启动 HTTP 服务器并注册处理根路径（"/"）的请求
void startWebServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();  // 使用默认的 HTTP 配置
  config.server_port = 80;  // 设置 HTTP 服务器监听端口为 80（HTTP 默认端口）

  // 配置根路径（"/"）的请求处理规则
  httpd_uri_t index_uri = {
    .uri = "/",               // 设置请求的 URI 路径为 "/"
    .method = HTTP_GET,       // 设置请求方法为 GET
    .handler = index_handler, // 设置处理请求的回调函数
    .user_ctx = NULL          // 没有额外的上下文数据
  };

  // 启动 HTTP 服务器
  if (httpd_start(&web_httpd, &config) == ESP_OK) {
    // 注册 URI 处理程序，将根路径与回调函数绑定
    httpd_register_uri_handler(web_httpd, &index_uri);  
  }
}

void setup() {
  Serial.begin(115200);  // 启动串口，设置波特率为 115200

  // 连接 Wi-Fi 网络
  WiFi.begin(ssid, password);  // 启动 Wi-Fi 连接
  // 如果未连接 Wi-Fi，则一直等待
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);  // 每 500ms 延时一次
    Serial.print(".");  // 在串口监视器上打印点，以指示正在尝试连接
  }
  Serial.println("");  // 换行
  Serial.println("WiFi connected");  // Wi-Fi 连接成功时打印信息
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());   // 打印分配到的 IP 地址
  
  // 启动 HTTP 服务器
  startWebServer();  // 启动 Web 服务器并注册处理程序
  Serial.println("HTTP server started");
}

void loop() {
  // 主循环为空，因为 HTTP 服务器在后台通过中断/任务处理请求
}