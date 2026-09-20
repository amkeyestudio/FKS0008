# 从 ESP32S3_4WD_Car 库中导入小车控制类，相当于请了一位懂小车控制的“助手”
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# 导入 time（时间）模块，用于让程序暂停（延时），就像人需要时间反应一样
import time

# 定义超声波传感器的引脚编号，方便后面使用
TRIG_PIN = 13   # 触发引脚连接到 IO13，告诉传感器“发射”
ECHO_PIN = 12   # 回声引脚连接到 IO12，接收传感器“返回”的信号

# 创建一个小车对象，起名叫 car，后面我们通过 car 来控制小车
car = Keyes_ESP32S3_4WD()

# 初始化超声波传感器，告诉它 Trig 和 Echo 分别接在哪个引脚上
car.Ultrasonic_init(TRIG_PIN,ECHO_PIN)

# 定义电机驱动的引脚编号，用于控制左右车轮
MOTOR_AIN = 40  # A路方向控制引脚 (控制左轮正转还是反转)
MOTOR_AEN = 41  # A路 PWM 控制引脚 (控制左轮转动的速度)
MOTOR_BIN = 38  # B路方向控制引脚 (控制右轮正转还是反转)
MOTOR_BEN = 21  # B路 PWM 控制引脚 (控制右轮转动的速度)

# 初始化电机，把上面定义的 4 个引脚告诉小车助手
car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)

# 定义基础行驶速度参数，范围是 0(停止) 到 255(全速)，这里设为 220 比较稳
BASE_SPEED = 220  # 基础行驶速度 (0-255)

# 初始状态先停止电机，防止一上电小车就乱跑
car.stop_motor()

# 定义舵机控制的引脚编号
SERVO_PIN = 42    # 舵机信号线连接到 IO42

# 初始化舵机，告诉它信号线接在哪个引脚上
car.Servo_init(SERVO_PIN)

# 初始化舵机到 90 度位置，也就是让“脖子”正对前方
car.Servo_set_angle(90)
# 等待 1 秒，让舵机有足够的时间转到 90 度并稳定下来
time.sleep(1)

# 主循环：while True 表示“永远循环”，让小车一直重复下面的动作
while True:
    # 测量前方距离，并把结果赋值给变量 distance（距离）
    distance = car.Ultrasonic_measure_distance()

    # 通过串口（电脑和开发板通信的通道）打印结果，方便我们在电脑上查看
    print("Distance:", distance, "cm")
    
    # 如果判断：如果前方距离小于等于 15 厘米（遇到危险了）
    if distance <= 15 :
        # 立即停止电机，踩刹车
        car.stop_motor()
        # 舵机旋转到 180 度，也就是让“脖子”看向最左边
        car.Servo_set_angle(180)
        # 等待 0.3 秒，让舵机转到位并稳定，不然测距会不准
        time.sleep(0.3)
        # 测量左侧的距离，并保存在 leftDistance（左侧距离）变量中
        leftDistance = car.Ultrasonic_measure_distance()
        # 稍微等待 0.1 秒，给传感器一点喘息的时间
        time.sleep(0.1)

        # 舵机旋转到 0 度，也就是让“脖子”看向最右边
        car.Servo_set_angle(0)
        # 等待 0.3 秒，让舵机转到位并稳定
        time.sleep(0.3)
        # 测量右侧的距离，并保存在 rightDistance（右侧距离）变量中
        rightDistance = car.Ultrasonic_measure_distance()
        # 稍微等待 0.1 秒
        time.sleep(0.1)
        
        # 舵机旋转到 90 度，让“脖子”回到正前方中间位置
        car.Servo_set_angle(90)
        
        # 如果判断：如果左侧距离 大于 右侧距离（说明左边更空旷）
        if leftDistance > rightDistance :
            # 控制小车向左转（左轮后退，右轮前进，速度为 BASE_SPEED）
            car.left(BASE_SPEED, BASE_SPEED)
        # 否则（说明右边更空旷，或者两边一样）
        else :
            # 控制小车向右转（左轮前进，右轮后退，速度为 BASE_SPEED）
            car.right(BASE_SPEED, BASE_SPEED)
        # 转弯持续 0.5 秒，转个差不多就行
        time.sleep(0.5)
    # 否则（如果前方距离大于 15 厘米，很安全）
    else :
        # 控制小车向前直行（左右轮都正转，速度为 BASE_SPEED）
        car.forward(BASE_SPEED, BASE_SPEED)
    # 每次循环结束后，休息 0.1 秒，避免程序跑得太快导致传感器来不及反应
    time.sleep(0.1)