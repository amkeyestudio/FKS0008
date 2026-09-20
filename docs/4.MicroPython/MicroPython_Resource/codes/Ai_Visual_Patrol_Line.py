from machine import Pin
import time
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
from aicam import AiCam  # 假设上面的 AiCam 类保存在 aicam.py 中

# 初始化 AiCam，摄像头接在 GPIO17 (TX) 和 GPIO16 (RX)，波特率 9600
cam = AiCam()
cam.begin(tx=17, rx=18, baud=9600)

# 设置为线条检测模式（你也可以改成 FACE / QR / CARD / LINE）
cam.setMode(AiCam.LINE)

print("AiCam 已启动，等待数据...")

car = Keyes_ESP32S3_4WD()
# 定义电机控制的引脚（引脚就是开发板上的金属接口，用来输出电信号）
MOTOR_AIN = 40  # 定义 A路（左轮）的方向控制引脚为 40 号，决定左轮正转还是反转
MOTOR_AEN = 41  # 定义 A路（左轮）的速度控制引脚为 41 号，决定左轮转多快
MOTOR_BIN = 38  # 定义 B路（右轮）的方向控制引脚为 38 号，决定右轮正转还是反转
MOTOR_BEN = 21  # 定义 B路（右轮）的速度控制引脚为 21 号，决定右轮转多快

# 告诉小车，电机接在刚才定义的 40、41、38、21 号引脚上，初始化（启动）电机
car.Motor_init(MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN)
# 定义速度参数（数值范围是 0 到 255，255 是最快，0 是停止）
BASE_SPEED = 220  # 设置基础直行速度为 220，这是一个比较快但又不会失控的速度

# 初始状态先让所有电机停止，防止小车一通电就乱跑
car.stop_motor()

while True:
    # 不断读取并解析串口数据
    cam.readData()
    
    angleVal = cam.lineAngle()
    offsetVal = cam.lineOffset()
    validVal = cam.lineValid()
    if abs(angleVal) <= 25 and abs(offsetVal) <= 50 and validVal == 5:
        car.forward(80,80)
    elif angleVal > 25 or offsetVal > 50:
        car.right(80,255)
    elif angleVal < -25 or offsetVal < -50:
        car.left(255,80)
    else :
        car.forward(80,80)
        time.sleep(0.3)
        car.stop_motor()
    
    
    
