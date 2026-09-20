# 导入 network 模块，就像拿出手机里的“Wi-Fi设置”工具箱，用于管理 Wi-Fi 连接
import network
# 导入 time 模块，用于让程序暂停等待（延时），就像数秒一样
import time

# 请将 "your_SSID" 修改成您的 Wi-Fi 名称（注意保留英文双引号）
SSID = "your_SSID"
# 请将 "your_PASSWORD" 修改成您的 Wi-Fi 密码（注意保留英文双引号）
PASSWORD = "your_PASSWORD"

# 创建 WLAN（无线局域网）对象，接口类型为 STA（站点模式，也就是去连接路由器的模式）
wlan = network.WLAN(network.STA_IF)

# 激活 WLAN 接口，相当于打开手机的 Wi-Fi 开关
wlan.active(True)

# 打印连接提示信息，这行字会显示在电脑的串口监视器上
print("Connecting to WiFi...")

# 连接指定的 Wi-Fi 网络，把前面设置的名字和密码告诉开发板
wlan.connect(SSID, PASSWORD)

# 循环检查 Wi-Fi 连接状态
# 如果未连接成功（isconnected() 返回 False），则一直在这个循环里等待
while not wlan.isconnected():
    # 延时 1 秒，给开发板一点时间去和路由器“对暗号”
    time.sleep(1)
    # 打印连接中的提示信息，让你知道它还在努力连接
    print("Connecting to WiFi...")

# 连接成功，退出上面的等待循环，程序继续往下走
print("Connected to WiFi")

# 获取网络配置信息，ifconfig() 返回一个包含4个数据的组合：(IP地址, 子网掩码, 网关, DNS服务器)
# 我们只需要第一个元素（索引为0），即 IP 地址，并把它存进 ip_address 变量里
ip_address = wlan.ifconfig()[0]

# 打印获取到的 IP 地址，这样你就能在电脑上看到开发板的“网络门牌号”了
print("IP Address:", ip_address)
