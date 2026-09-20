from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD  # 导入小车专属的“驾驶手册”库，让我们能轻松控制小车
# main.py - ESP32-S3 红外接收 (NEC协议)       # 这是一句注释，告诉别人这是一个红外接收程序，使用的是NEC这种常见的红外协议
from machine import Pin                        # 导入“机器引脚”模块，让我们能控制开发板上的金属小接口（引脚）
from ir_rx.nec import NEC_8                    # 导入NEC协议的红外解码器，专门用来听懂遥控器的“语言”
import time                                    # 导入“时间”模块，用来让程序暂停或延时

car = Keyes_ESP32S3_4WD()                      # 创建一个“小车”对象，相当于把小车从图纸上召唤到代码里，起名叫 car

# 电机引脚定义（告诉大脑，控制轮子的线插在哪些接口上）
MOTOR_AIN = 40  # A路方向控制引脚 (控制左轮正转还是反转)，插在40号接口
MOTOR_AEN = 41  # A路 PWM 控制引脚 (控制左轮转多快)，插在41号接口
MOTOR_BIN = 38  # B路方向控制引脚 (控制右轮正转还是反转)，插在38号接口
MOTOR_BEN = 21  # B路 PWM 控制引脚 (控制右轮转多快)，插在21号接口

car.Motor_init(MOTOR_AIN,MOTOR_AEN,MOTOR_BIN,MOTOR_BEN) # 初始化电机，把上面定义的4个引脚告诉小车，让它准备好控制轮子

# 速度参数
BASE_SPEED = 220  # 基础行驶速度，范围是0到255。220是一个比较快但又不会失控的速度

# 初始状态停止电机
car.stop_motor() # 刚启动时，先让小车停下，防止它一通电就乱跑


# 定义红外接收引脚为 IO48（告诉大脑，接收红外信号的线插在48号接口）
IR_RX_PIN = 48

# 定义回调函数（这是一个“自动触发机关”：一旦收到红外信号，就自动执行里面的代码）
def ir_callback(data, addr, ctrl): 
    # data: 命令数据（也就是你按下的按键对应的“密码/身份证号”）
    # addr: 设备地址（遥控器的出厂编号，我们这里用不到）
    # ctrl: 控制标志（用来判断你是短按还是长按）
    if data < 0:
        # 如果 data 小于 0，通常表示你长按了遥控器，收到了“重复码”
        print("长按中...") # 在电脑上打印提示文字
    else:
        # 如果是正常按下按键，打印解码出的按键密码和地址
        print("按键值: ", data) # 在电脑上显示你按下了哪个密码
        if data == 70:
            car.forward(BASE_SPEED, BASE_SPEED) # 如果密码是70（通常是“上”键），小车全速前进
        elif data == 21:
            car.back(BASE_SPEED, BASE_SPEED)    # 如果密码是21（通常是“下”键），小车全速后退
        elif data == 68:
            car.left(BASE_SPEED, BASE_SPEED)    # 如果密码是68（通常是“左”键），小车向左转弯
        elif data == 67:
            car.right(BASE_SPEED, BASE_SPEED)   # 如果密码是67（通常是“右”键），小车向右转弯
        elif data == 64:
            car.stop_motor()                    # 如果密码是64（通常是“OK”键），小车立刻停止
        

# 初始化红外接收对象（正式开启红外接收功能）
# Pin(IR_RX_PIN, Pin.IN) 意思是把48号引脚设置为“输入模式”（只接收信号，不输出信号）
# ir_callback 是信号解码成功后自动调用的“机关”函数
ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), ir_callback) 

print("红外接收已启动，请按下遥控器按键...") # 在电脑上打印提示，告诉你准备就绪


while True: # 这是一个“死循环”，意思是让下面的代码一直重复执行，直到你断开电源
    # 主循环可以处理其他任务，红外接收由中断（一种后台自动处理机制）在后台完成
    time.sleep_ms(100) # 让程序每次循环休息 100 毫秒（0.1秒），防止大脑累坏，同时也给红外接收留出处理时间