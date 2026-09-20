from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD  # 从专门的库文件中，请出控制小车的“专家”（类），准备开始工作
import time  # 导入时间模块，就像给小车配了一块手表，用来控制等待和延时

# 定义引脚（给接口起个好听的名字，方便后面使用）
TRIG_PIN = 13   # 定义触发引脚的编号为13，告诉小车“发射超声波”的指令从13号接口发出
ECHO_PIN = 12   # 定义回声引脚的编号为12，告诉小车“接收回声”的指令从12号接口读取

car = Keyes_ESP32S3_4WD()  # 创建一个小车对象，起名叫car，相当于给小车办个“身份证”，后面用car就能控制它

car.Ultrasonic_init(TRIG_PIN,ECHO_PIN)  # 初始化超声波传感器，告诉它使用13号和12号接口，让它准备好“看”东西

# 电机引脚（定义控制车轮转动的接口）
MOTOR_AIN = 40  # 定义左轮（A路）的方向控制引脚为40号，用来决定左轮是正转还是反转
MOTOR_AEN = 41  # 定义左轮（A路）的速度控制引脚为41号，用来调节左轮转动的快慢（通过PWM技术）
MOTOR_BIN = 38  # 定义右轮（B路）的方向控制引脚为38号，用来决定右轮是正转还是反转
MOTOR_BEN = 21  # 定义右轮（B路）的速度控制引脚为21号，用来调节右轮转动的快慢

car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN)  # 初始化电机，把上面定义的4个引脚告诉小车，让电机“肌肉”准备好工作

# 速度参数（设置小车跑多快）
BASE_SPEED = 220  # 设置基础行驶速度为220（最高是255），让小车跑得比较快但又不会失控
TURN_SPEED = 220  # 设置转向时的速度为220，保持和直行一样的速度

# 初始状态停止电机
car.stop_motor()  # 一开始先让小车停下来，确保安全，防止代码刚上传小车就乱跑撞坏东西

# 主循环（让小车不停地重复下面的动作）
while True:  # 开启一个无限循环，只要不断电，小车就会一直执行下面的代码
    # 测量距离
    distance = car.Ultrasonic_measure_distance()  # 让超声波传感器测量一次距离，并把结果存到“distance”（距离）这个变量（用来装数据的盒子）里

    # 通过串口打印结果
    print("Distance:", distance, "cm")  # 在电脑屏幕上打印出当前的距离值，方便我们观察小车“看”到了什么
    
    # 根据障碍物距离执行相应的动作（开始做选择题）
    if distance >= 30 and distance <= 50:  # 如果距离大于等于30厘米，并且（and）小于等于50厘米（目标在合适的跟随距离）
        car.forward(BASE_SPEED, BASE_SPEED)  # 让小车以基础速度向前进，追上前面的目标
    elif (distance >= 10 and distance < 30) or distance > 50:  # 否则（elif），如果距离在10到30厘米之间（太近了），或者（or）大于50厘米（太远了跟丢了）
        car.stop_motor()  # 让小车停止运动，原地待命
    elif distance < 10 and distance > 0:  # 否则，如果距离小于10厘米且大于0（马上就要撞上了）
        car.back(BASE_SPEED, BASE_SPEED)  # 让小车以基础速度向后退，安全避让
    time.sleep(0.1)  # 让程序暂停0.1秒（也就是100毫秒），给传感器一点休息时间，也让动作更平滑，不至于太灵敏导致抽搐

