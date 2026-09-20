# 从 ESP32S3_4WD_Car 文件中，导入 Keyes_ESP32S3_4WD 这个工具包，它包含了控制小车的各种功能
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# 导入 time（时间）模块，用来让程序暂停、延时
import time

# 定义引脚（也就是告诉程序，我们的线插在哪个编号的接口上）
TRIG_PIN = 13   # 触发引脚（Trig）连接到开发板的 IO13 接口
ECHO_PIN = 12   # 回声引脚（Echo）连接到开发板的 IO12 接口

# 创建一个名为 ultrasonic（超声波）的对象，相当于把传感器功能激活
ultrasonic = Keyes_ESP32S3_4WD()

# 初始化超声波传感器，告诉它 Trig 和 Echo 分别接在 13 和 12 号引脚上
ultrasonic.Ultrasonic_init(TRIG_PIN, ECHO_PIN)

# 主循环：while True 意思是“当条件为真时”，因为 True 永远为真，所以下面的代码会像钟表一样一直反复执行
while True:
    # 调用测量函数，让传感器测一次距离，并把结果存到 distance（距离）这个变量中
    distance = ultrasonic.Ultrasonic_measure_distance()

    # 通过串口（电脑上的聊天窗口）打印出测量结果，带上单位 cm（厘米）
    print("Distance:", distance, "cm")

    # 让程序休息（等待）1秒，再进行下一次测量，避免数据刷新太快眼睛看不清
    time.sleep(1)