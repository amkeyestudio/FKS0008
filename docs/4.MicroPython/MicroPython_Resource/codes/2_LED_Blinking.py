# 导入 machine 模块中的 Pin 类，用于控制 GPIO 引脚的输入输出
from machine import Pin
# 导入 time 模块，用于实现程序的延时功能
import time

# 创建 LED 控制对象，指定连接到 GPIO11 (即 io11)，并设置为输出模式 (Pin.OUT)
led = Pin(11, Pin.OUT)

# 使用无限循环，让 LED 持续不断地闪烁
while True:
    # 将 GPIO11 设置为高电平 (1)，点亮 LED 
    # (注：如果你的模块是低电平点亮的，请将此处的 1 改为 0)
    led.value(1)
    # 保持当前点亮状态 1 秒钟
    time.sleep(1)
    
    # 将 GPIO11 设置为低电平 (0)，熄灭 LED 
    # (注：如果是低电平点亮模块，请将此处的 0 改为 1)
    led.value(0)
    # 保持当前熄灭状态 1 秒钟
    time.sleep(1)