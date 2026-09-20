# 从 machine 模块导入 Pin 和 PWM 类，Pin 用于指定引脚，PWM 用于输出可调亮度的脉冲
from machine import Pin, PWM
# 导入 time 模块，用于控制呼吸灯的延时速度
import time

# 创建 PWM 对象，使用 GPIO11，频率设置为 1000Hz（频率越高，LED 闪烁感越小）
led = PWM(Pin(11), freq=1000)
# 将占空比设置为 0，初始状态为熄灭（占空比范围 0~65535，0 最暗，65535 最亮）
led.duty_u16(0)

# 无限循环，让呼吸灯持续运行
while True:
    # 渐亮循环：占空比从 0 开始，每次增加 256，直到接近 65535
    for duty in range(0, 65536, 256):
        # 将当前占空比写入 PWM，控制 LED 亮度
        led.duty_u16(duty)
        # 延时 5 毫秒，数值越大呼吸越慢，数值越小呼吸越快
        time.sleep_ms(5)

    # 确保达到最大亮度 65535
    led.duty_u16(65535)
    # 全亮状态保持 200 毫秒
    time.sleep_ms(200)

    # 渐灭循环：占空比从 65535 开始，每次减少 256，直到接近 0
    for duty in range(65535, -1, -256):
        # 将当前占空比写入 PWM，控制 LED 亮度
        led.duty_u16(duty)
        # 延时 5 毫秒，与渐亮速度保持一致
        time.sleep_ms(5)

    # 确保达到最小亮度 0，完全熄灭
    led.duty_u16(0)
    # 全灭状态保持 200 毫秒
    time.sleep_ms(200)