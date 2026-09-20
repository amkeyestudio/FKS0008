# ==================== 导入模块 ====================
# 导入网络模块，用来连接 Wi-Fi
import network          
# 导入套接字模块，用来创建 HTTP 服务器（也就是微型网站）
import socket           
# 导入时间模块，用来让程序暂停一下（延时）
import time             
# 从我们自己的 ESP32S3_4WD_Car 文件中，导入小车控制类和网页代码
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD, INDEX_HTML
#from webpage import INDEX_HTML               # 从 webpage.py 导入网页内容（这行被注释掉了，暂时不用）

# 创建一个“小车”对象，相当于给小车起个名字叫 car，以后用 car 就能控制它
car = Keyes_ESP32S3_4WD()

# ==================== WiFi 配置 ====================
# 把你要连接的 WiFi 名字填在这里，比如 "FKS0008"
SSID = "FKS0008"         
# 把你要连接的 WiFi 密码填在这里，比如 "88888888"
PASSWORD = "88888888"   

# ==================== 电机引脚定义 ====================
# 定义左轮方向控制连接的接口编号是 40
MOTOR_AIN = 40          
# 定义左轮速度控制连接的接口编号是 41
MOTOR_AEN = 41          
# 定义右轮方向控制连接的接口编号是 38
MOTOR_BIN = 38          
# 定义右轮速度控制连接的接口编号是 21
MOTOR_BEN = 21          

# 把上面定义的 4 个接口编号告诉小车，初始化电机
car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)

# 速度参数：设置小车的基础行驶速度，范围是 0 到 255，这里设为 220
BASE_SPEED = 220           

# ==================== 舵机引脚与状态 ====================
# 定义舵机信号线连接的接口编号是 11
SERVO_PIN = 11          

# 把舵机的接口编号告诉小车，初始化舵机
car.Servo_init(SERVO_PIN)

# ==================== 指令处理函数 ====================
# 定义一个“处理指令”的函数，根据收到的 move（动作）参数执行对应动作
def handle_cmd(move):
    """根据 move 参数执行对应动作"""
    # 如果收到的指令是 "forward"（前进）
    if move == "forward":
        # 让小车以 200 的速度前进
        car.forward(200, 200)
    # 如果收到的指令是 "backward"（后退）
    elif move == "backward":
        # 让小车以 200 的速度后退
        car.back(200, 200)
    # 如果收到的指令是 "left"（左转）
    elif move == "left":
        # 让小车以 200 的速度左转
        car.left(200, 200)
    # 如果收到的指令是 "right"（右转）
    elif move == "right":
        # 让小车以 200 的速度右转
        car.right(200, 200)
    # 如果收到的指令是 "stop"（停止）
    elif move == "stop":
        # 让小车停止电机转动
        car.stop_motor()                                       
    # 如果收到的指令是 "claw_open"（机械爪张开）
    elif move == "claw_open":
        # 让舵机转到 140 度，使机械爪张开
        car.Servo_set_angle(140)                       
    # 如果收到的指令是 "claw_close"（机械爪闭合）
    elif move == "claw_close":
        # 让舵机转到 90 度，使机械爪闭合
        car.Servo_set_angle(90)                     

# ==================== 连接 WiFi ====================
# 创建一个 Wi-Fi 站点（STA）接口，意思是让开发板作为设备去连接路由器
wlan = network.WLAN(network.STA_IF)              
# 激活这个 Wi-Fi 接口，让它开始工作
wlan.active(True)                                
# 在屏幕上打印提示文字，告诉用户正在连接哪个 WiFi
print("正在连接 WiFi:", SSID)
# 使用前面定义的名字和密码去连接 WiFi
wlan.connect(SSID, PASSWORD)                     
# 只要还没连接成功，就一直循环等待
while not wlan.isconnected():                    
    # 每次暂停 0.5 秒，避免程序跑得太快卡死
    time.sleep(0.5)
    # 在屏幕上打印一个点，表示正在努力连接中
    print(".", end="")

# 换行，让后面的文字显示在下一行
print()
# 打印连接成功的提示
print("WiFi 连接成功！")
# 打印开发板获取到的 IP 地址（也就是它在网络中的门牌号）
print("IP 地址：", wlan.ifconfig()[0])

# ==================== 启动 HTTP 服务器 ====================
# 创建一个网络套接字（相当于建立一个通信通道），使用 IPv4 和 TCP 协议
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 设置套接字选项，允许地址重用，防止重启程序时报错
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# 把套接字绑定到 80 端口（80 是网页默认的端口号）
s.bind(('', 80))                                 
# 开始监听，最多允许 5 个设备同时连接
s.listen(5)                                      
# 打印提示，告诉用户网页服务器已经准备好了
print("Web 服务器已启动！")

# ==================== 主循环 ====================
# 这是一个无限循环，让服务器一直运行，不断等待手机发来的请求
while True:
    try:
        # 等待手机连接，一旦连上，就获取连接对象 conn 和手机地址 addr
        conn, addr = s.accept()                              
        # 从连接中读取手机发来的数据（最多 1024 字节），并转换成文字
        request = conn.recv(1024).decode('utf-8')            

        # 解析请求的第一行，比如：GET /cmd?move=forward HTTP/1.1
        first_line = request.split('\r\n')[0]
        # 把第一行按空格拆开，分成几个部分
        parts = first_line.split(' ')
        # 提取出请求的路径（比如 "/" 或 "/cmd?move=forward"）
        path = parts[1] if len(parts) >= 2 else "/"

        # 路由判断：如果请求的是根路径（首页）
        if path == "/" or path == "/index.html":
            # 组装一个包含网页内容（INDEX_HTML）的 HTTP 响应
            response = ('HTTP/1.1 200 OK\r\n'
                        'Content-Type: text/html; charset=UTF-8\r\n'
                        'Connection: close\r\n\r\n' + INDEX_HTML)
            # 把响应转换成编码后发送给手机
            conn.send(response.encode('utf-8'))

        # 路由判断：如果请求的是控制指令路径（以 /cmd 开头）
        elif path.startswith("/cmd"):
            # 初始化一个空的 move 变量，用来存放动作指令
            move = ""
            # 如果路径里包含问号（说明带有参数）
            if "?" in path:
                # 把问号后面的参数部分提取出来
                query = path.split("?", 1)[1]
                # 把参数按 & 符号拆开，逐个检查
                for param in query.split("&"):
                    # 如果参数是以 "move=" 开头的
                    if param.startswith("move="):
                        # 把 "move=" 后面的具体动作（比如 forward）提取出来
                        move = param.split("=", 1)[1]
            # 调用前面定义的函数，执行提取到的动作
            handle_cmd(move)
            # 组装一个成功的文本响应，告诉手机指令已执行
            response = ('HTTP/1.1 200 OK\r\n'
                        'Content-Type: text/plain\r\n'
                        'Connection: close\r\n\r\nOK')
            # 把响应发送给手机
            conn.send(response.encode('utf-8'))

        # 路由判断：如果请求的是测距路径（/distance）
        elif path == "/distance":
            # 调用超声波模块测量距离，并把结果存到 dist 变量里
            dist = 0
            # 组装一个包含距离数据的文本响应
            response = ('HTTP/1.1 200 OK\r\n'
                        'Content-Type: text/plain\r\n'
                        'Connection: close\r\n\r\n' + str(dist))
            # 把响应发送给手机
            conn.send(response.encode('utf-8'))

        # 路由判断：如果请求的是其他不认识的路径
        else:
            # 组装一个 404 找不到的错误响应
            response = ('HTTP/1.1 404 Not Found\r\n'
                        'Content-Type: text/plain\r\n'
                        'Connection: close\r\n\r\nNot Found')
            # 把错误响应发送给手机
            conn.send(response.encode('utf-8'))

    # 如果上面执行过程中出了任何错误，就捕获它
    except Exception as e:
        # 在屏幕上打印出错误信息，方便我们排查
        print("Error:", e)
    # 无论成功还是失败，最后都要执行这一步
    finally:
        # 关闭这次连接，释放资源，准备迎接下一次连接
        conn.close()
