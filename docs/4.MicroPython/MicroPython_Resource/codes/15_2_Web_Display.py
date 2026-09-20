# 导入 network 模块，用于管理 Wi-Fi 连接
import network
# 导入 socket 模块，用于创建 TCP 服务器（相当于建立一条通信管道）
import socket
# 导入 time 模块，用于延时
import time

# 请将以下信息替换为您的网络凭据（你的Wi-Fi名字和密码）
SSID = "your_SSID"          # 改成您的 Wi-Fi 名称
PASSWORD = "your_PASSWORD"  # 改成您的 Wi-Fi 密码

# 定义要显示的 HTML 页面内容，使用三引号保留多行格式，这就相当于我们要发给浏览器的“信件内容”
HTML = """<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 Web Server</title>
  </head>
  <body>
    <h1>Hello World</h1>
    <p>这是由 ESP32S3 Pro 提供的网页。</p>
  </body>
</html>
"""

# ==================== 连接 Wi-Fi ====================
# 创建 WLAN 对象，接口类型为 STA（站点模式，即连接路由器）
wlan = network.WLAN(network.STA_IF)
# 激活 WLAN 接口，打开 Wi-Fi 开关
wlan.active(True)
# 连接指定的 Wi-Fi 网络
wlan.connect(SSID, PASSWORD)

# 打印连接提示
print("Connecting to WiFi...")
# 循环检查连接状态，直到连接成功
while not wlan.isconnected():
    # 延时 500 毫秒（0.5秒），让连接过程稍微快一点反馈
    time.sleep(0.5)
    # 打印一个点，表示正在尝试连接，不换行
    print(".", end="")

# 连接成功，换行并打印信息
print()
print("WiFi connected")
# 获取并打印 IP 地址，ifconfig()[0] 就是提取 IP 地址
print("IP Address:", wlan.ifconfig()[0])

# ==================== 启动 HTTP 服务器 ====================
# 创建 TCP socket（套接字），使用 IPv4 和 TCP 协议。就像买了一部电话机
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 设置地址重用，避免重启后端口被占用。就像允许电话机挂断后立刻再打进来
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# 绑定到所有网络接口的 80 端口（HTTP 默认端口）。80端口就是网页访问的“默认大门”
s.bind(('', 80))
# 开始监听，最大等待连接数为 5。就像坐在电话机旁等电话，最多允许5个人排队
s.listen(5)

# 打印服务器启动信息
print("HTTP server started")

# 主循环，持续处理客户端请求。这是一个死循环，让服务器一直工作不休息
while True:
    # 等待客户端连接，返回连接对象(conn)和客户端地址(addr)。相当于接听了电话
    conn, addr = s.accept()
    # 打印客户端地址，看看是谁访问了我们
    print("Client connected from", addr)
    try:
        # 接收客户端请求数据（最多 1024 字节）。听听对方在电话里说了什么
        request = conn.recv(1024)
        # 构建 HTTP 响应头，指定内容类型为 HTML，编码为 UTF-8，然后拼上我们的 HTML 网页内容
        response = 'HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: close\r\n\r\n' + HTML
        # 发送响应内容，编码为 UTF-8。把网页内容“说”给浏览器听
        conn.send(response.encode('utf-8'))
    except Exception as e:
        # 如果发生异常（比如网络突然断了），打印错误信息
        print("Error:", e)
    finally:
        # 关闭客户端连接。相当于挂断电话，准备接下一个
        conn.close()
