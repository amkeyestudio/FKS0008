# 从厂家提供的“汽车/机器人”工具包里，拿出一个叫 Keyes_ESP32S3_4WD 的“魔法盒”（控制模块）
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# 导入“时间”工具包，因为我们需要让程序“等一等”，控制时间节奏
import time

# 给舵机的信号线分配一个“门牌号”（引脚号），这里我们选 42 号引脚
SERVO_PIN = 42

# 把刚才拿出的“魔法盒”打开，创建一个叫 servo（舵机）的控制对象
servo = Keyes_ESP32S3_4WD()

# 告诉魔法盒：“请初始化（准备）42号引脚，我们要开始控制舵机啦！”
servo.Servo_init(SERVO_PIN)

# 发出第一个指令：让舵机先转到 90 度（正中间位置）
servo.Servo_set_angle(90)
# 让程序暂停 1 秒钟，给舵机一点时间慢慢转到 90 度并站稳
time.sleep(1)

# 开启一个“永远循环”的魔法阵，只要不断电，里面的代码就会一直重复执行
while True:
    # 第一个阶段：让角度变量 angle 从 0 开始，每次加 1，一直增加到 180（包含180）
    for angle in range(0, 181, 1):
        # 把当前的角度值发给舵机，命令它转到这个角度
        servo.Servo_set_angle(angle)
        # 每次转动后，暂停 15 毫秒（千分之十五秒），让舵机有时间物理转动，实现平滑效果
        time.sleep_ms(15)

    # 到达 180 度后，让舵机在这里休息（停顿） 1 秒钟
    time.sleep(1)

    # 第二个阶段：让角度变量 angle 从 180 开始，每次减 1，一直减少到 0（包含0）
    for angle in range(180, -1, -1):
        # 把当前的角度值发给舵机，命令它反向转到这个角度
        servo.Servo_set_angle(angle)
        # 同样暂停 15 毫秒，保持平滑转动的节奏
        time.sleep_ms(15)

    # 回到 0 度后，再让舵机休息 1 秒钟，然后重新开始下一轮循环
    time.sleep(1)