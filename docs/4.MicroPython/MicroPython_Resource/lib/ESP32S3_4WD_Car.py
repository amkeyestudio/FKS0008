# ESP23S3_4Wd_Car.py - 小车库文件
from machine import Pin, PWM, I2C, time_pulse_us
from esp32 import RMT
import time

class Keyes_ESP32S3_4WD:
    
    # 音阶频率表（低音到高音）
    NOTES = [
        262, 294, 330, 349, 392, 440, 494,        # 低音 Do-Si
        523, 587, 659, 698, 784, 880, 988,        # 中音 Do-Si
        1047, 1175, 1319, 1397, 1568, 1760, 1967  # 高音 Do-Si
    ]
    
    """
    电机驱动代码
    """
    def Motor_init(self, MOTOR_AIN, MOTOR_AEN, MOTOR_BIN, MOTOR_BEN):
        # 创建方向控制引脚对象，设置为输出模式
        self.ain = Pin(MOTOR_AIN, Pin.OUT)
        self.bin = Pin(MOTOR_BIN, Pin.OUT)

        # 创建 PWM 对象，频率设置为 1000Hz
        self.aen = PWM(Pin(MOTOR_AEN), freq=20000)
        self.ben = PWM(Pin(MOTOR_BEN), freq=20000)

        # 初始占空比设为 0，确保电机停止
        self.aen.duty_u16(0)
        self.ben.duty_u16(0)
    
    def set_motor(self,left_speed, right_speed):
        """设置左右电机的 PWM 速度，范围 0-255"""
        # 限制速度在 0-255 之间
        left_speed = max(0, min(255, left_speed))
        right_speed = max(0, min(255, right_speed))
        # 将 0-255 线性映射到 duty_u16 的 0-65535
        self.aen.duty_u16(int(left_speed / 255 * 65535))
        self.ben.duty_u16(int(right_speed / 255 * 65535))

    def forward(self,left_speed, right_speed):
        """前进：左轮正转，右轮正转"""
        self.ain.value(1)                # 左轮方向引脚置高，正转
        self.bin.value(1)                # 右轮方向引脚置高，正转
        self.set_motor(left_speed, right_speed)

    def back(self,left_speed, right_speed):
        """后退：左轮反转，右轮反转"""
        self.ain.value(0)                # 左轮方向引脚置低，反转
        self.bin.value(0)                # 右轮方向引脚置低，反转
        self.set_motor(left_speed, right_speed)

    def left(self,left_speed, right_speed):
        """左转：左轮停止，右轮正转"""
        self.ain.value(0)                # 左轮方向引脚置低（停止/反转）
        self.bin.value(1)                # 右轮方向引脚置高，正转
        self.set_motor(left_speed, right_speed)

    def right(self,left_speed, right_speed):
        """右转：左轮正转，右轮停止"""
        self.ain.value(1)                # 左轮方向引脚置高，正转
        self.bin.value(0)                # 右轮方向引脚置低（停止/反转）
        self.set_motor(left_speed, right_speed)

    def stop_motor(self):
        """停止电机：速度设为 0"""
        self.set_motor(0, 0)
        
    """
    舵机驱动代码
    """
    def Servo_init(self, servo_pin):
        # 创建 PWM 对象，绑定到 GPIO42，频率设置为 50Hz（舵机标准控制频率）
        self.Servo_Pin = PWM(Pin(servo_pin), freq = 50)
    
    def Servo_set_angle(self, angle):
        """设置舵机角度，范围 0~180 度"""
        # 限制角度范围，防止超出舵机物理极限
        if angle < 0:
            angle = 0
        if angle > 180:
            angle = 180

        # 计算对应的脉冲宽度（单位：微秒）
        # 0 度对应 500us，180 度对应 2500us，线性映射
        pulse_us = 500 + (angle / 180) * 2000

        # 将脉冲宽度转换为 PWM 占空比
        # 周期为 20ms = 20000us，duty_u16 的范围是 0~65535
        duty = int(pulse_us / 20000 * 65535)

        # 输出 PWM 占空比，驱动舵机转到指定角度
        self.Servo_Pin.duty_u16(duty)
        
        
    """
    蜂鸣器驱动
    """
    def Buzzer_init(self, buzzerPin, rmt_id=0):
        self._buzzer_pin = buzzerPin
        self._rmt_id = rmt_id
        self._rmt = None
        Pin(buzzerPin, Pin.OUT, value=0)   # 上电静音

    def Buzzer_tone(self, frequency):
        """持续发出指定频率的方波"""
        self.Buzzer_off()

        if frequency <= 0:
            return

        # RMT 1MHz
        # 1 tick = 1us
        half_period = int(500000 / frequency)

        self._rmt = RMT(
            self._rmt_id,
            pin=Pin(self._buzzer_pin),
            clock_div=80,
            idle_level=0
        )

        # 高、低各 half_period us
        self._rmt.loop(True)

        self._rmt.write_pulses(
            (half_period, half_period),
            1
        )
        

    def Buzzer_off(self):
        if self._rmt is not None:
            self._rmt.loop(False)
            self._rmt.deinit()
            self._rmt = None
        Pin(self._buzzer_pin, Pin.OUT, value=0)

    def Buzzer_play(self, frequency, duration):
        self.Buzzer_tone(frequency)
        time.sleep_ms(duration)
        self.Buzzer_off()
        time.sleep_ms(10)
        
    def Buzzer_play_Do(self):
        self.Buzzer_play(262, 500)
        
    def Buzzer_play_Re(self):
        self.Buzzer_play(294, 500)
        
    def Buzzer_play_Mi(self):
        self.Buzzer_play(330, 500)
        
    def Buzzer_play_Fa(self):
        self.Buzzer_play(349, 500)
        
    def Buzzer_play_Sol(self):
        self.Buzzer_play(392, 500)
        
    def Buzzer_play_La(self):
        self.Buzzer_play(440, 500)
        
    def Buzzer_play_Si(self):
        self.Buzzer_play(494, 500)

    def Buzzer_play_Music(self, notes, meter):
        for i in range(len(notes)):          
            if 1 <= notes[i] <= len(self.NOTES):
                self.Buzzer_play(self.NOTES[notes[i] - 1], meter[i] * 200)
        
    
    """
    8*16点阵驱动代码
    """
    def Matrix_init(self, CLK_PIN, DIN_PIN):
        # 创建 CLK 引脚对象，设置为输出模式，初始电平为高
        self.clk = Pin(CLK_PIN, Pin.OUT, value=1)
        # 创建 DIN 引脚对象，设置为输出模式，初始电平为高
        self.din = Pin(DIN_PIN, Pin.OUT, value=1)

    def Matrix_iic_start(self):
        """模拟类 I2C 协议的起始条件：CLK 高电平时，DIN 由高变低"""
        self.clk.value(1)                # 拉高时钟线
        time.sleep_us(3)            # 延时 3 微秒
        self.din.value(1)                # 拉高数据线
        time.sleep_us(3)            # 延时 3 微秒
        self.din.value(0)                # 拉低数据线，产生下降沿
        time.sleep_us(3)            # 延时 3 微秒

    def Matrix_iic_send(self, data):
        """模拟类 I2C 协议的数据传输：CLK 低电平时改变数据，高电平时锁存"""
        for _ in range(8):          # 循环 8 次，发送一个字节
            self.clk.value(0)            # 拉低时钟，准备数据
            time.sleep_us(3)        # 延时 3 微秒
            if data & 0x01:         # 判断最低位是否为 1
                self.din.value(1)        # 若为 1，数据线拉高
            else:
                self.din.value(0)        # 若为 0，数据线拉低
            time.sleep_us(3)        # 延时 3 微秒
            self.clk.value(1)            # 拉高时钟，锁存数据
            time.sleep_us(3)        # 延时 3 微秒
            data >>= 1              # 数据右移一位，准备传输下一位

    def Matrix_iic_end(self):
        """模拟类 I2C 协议的停止条件：CLK 高电平时，DIN 由低变高"""
        self.clk.value(0)                # 拉低时钟线
        time.sleep_us(3)            # 延时 3 微秒
        self.din.value(0)                # 拉低数据线
        time.sleep_us(3)            # 延时 3 微秒
        self.clk.value(1)                # 拉高时钟线
        time.sleep_us(3)            # 延时 3 微秒
        self.din.value(1)                # 拉高数据线，产生上升沿
        time.sleep_us(3)            # 延时 3 微秒

    def Matrix_display(self, matrix_value):
        """核心函数：将 16 字节的图案数据写入 AiP1640 显存"""
        # 第一步：发送数据命令 (0x40)，设置为写数据模式，地址自动加 1
        self.Matrix_iic_start()                 # 发送起始条件
        self.Matrix_iic_send(0x40)              # 发送命令 0x40
        self.Matrix_iic_end()                   # 发送停止条件

        # 第二步：发送地址命令 (0xC0)，设置显存起始地址为 0
        self.Matrix_iic_start()                 # 发送起始条件
        self.Matrix_iic_send(0xC0)              # 发送命令 0xC0

        # 第三步：循环发送 16 字节的图案数据
        for b in matrix_value:      # 遍历传入的字节序列
            self.Matrix_iic_send(b)             # 逐个发送字节
        self.Matrix_iic_end()                   # 发送停止条件

        # 第四步：发送显示控制命令 (0x87)，开启显示，脉冲宽度设为 8/16 (最高亮度)
        self.Matrix_iic_start()                 # 发送起始条件
        self.Matrix_iic_send(0x8A)              # 发送命令 0x8A
        self.Matrix_iic_end()                   # 发送停止条件
        
    def Matrix_Scroll_display(self, arr):
        for shift in range(16):     # 循环 16 次，每次移动一个位置
            # 将 smile 数组循环左移，实现滚动效果
            scroll_data = arr[shift:] + arr[:shift]  # 切片拼接实现循环左移
            self.Matrix_display(scroll_data)                  # 显示滚动后的图案
            time.sleep_ms(150)                           # 滚动速度控制，数值越小滚动越快

        

    
    """
    巡线传感器读取代码
    """
    
    def Line_init(self, SCL, SDA):
        self.i2c = I2C(0, scl=Pin(SCL), sda=Pin(SDA), freq=100000)
        # PCF8574 的 I2C 地址（默认 0x20）
        self.PCF8574_ADDR = 0x20
        
        # 定义 5 路巡线传感器对应的位（对应 PCF8574 的 P0-P4）
        self.OUTA_BIT = 4  # 探头 A (最左侧) 对应 P4
        self.OUTB_BIT = 0  # 探头 B (左侧)   对应 P0
        self.OUTC_BIT = 1  # 探头 C (中间)   对应 P1
        self.OUTD_BIT = 2  # 探头 D (右侧)   对应 P2
        self.OUTE_BIT = 3  # 探头 E (最右侧) 对应 P3

        # 初始化 PCF8574：向端口写 0xFF，将所有引脚置为高电平（输入模式）
        self.i2c.writeto(self.PCF8574_ADDR, b'\xFF')
        
    def Line_get_data(self):
        data = self.i2c.readfrom(self.PCF8574_ADDR, 1)[0]
        
        # 提取各个探头的状态（1 表示高电平，0 表示低电平）
        outa = 1 - ((data >> self.OUTA_BIT) & 1)
        outb = 1 - ((data >> self.OUTB_BIT) & 1)
        outc = 1 - ((data >> self.OUTC_BIT) & 1)
        outd = 1 - ((data >> self.OUTD_BIT) & 1)
        oute = 1 - ((data >> self.OUTE_BIT) & 1)
        return outa, outb, outc, outd, oute

    
    
    """
    超声波传感器读取代码
    """
    def Ultrasonic_init(self, TRIG_PIN, ECHO_PIN):
        # 创建 Trig 引脚对象，设置为输出模式，初始低电平
        self.trig = Pin(TRIG_PIN, Pin.OUT, value=0)
        # 创建 Echo 引脚对象，设置为输入模式
        self.echo = Pin(ECHO_PIN, Pin.IN)
        
    def Ultrasonic_measure_distance(self):
        """测量一次距离，返回厘米，如果超时或超范围返回 0"""
        # 第一步：确保 Trig 引脚是低电平，准备发射
        self.trig.value(0)
        # 等待 2 微秒，确保信号稳定
        time.sleep_us(2)

        # 第二步：给 Trig 引脚一个 10 微秒的高电平脉冲，触发传感器发射超声波
        self.trig.value(1)
        time.sleep_us(10)
        self.trig.value(0)

        # 第三步：读取 Echo 引脚的高电平持续时间
        # time_pulse_us 会等待引脚变为指定电平，然后测量持续时间（微秒）
        # 参数：引脚对象、等待的电平（1 表示高电平）、超时时间（微秒）
        # 这里超时设为 30000 微秒（30ms），对应大约 5 米的距离
        duration = time_pulse_us(self.echo, 1, 30000)

        # 如果超时，duration 会返回 -2，直接返回 0
        if duration < 0:
            return 0

        # 第四步：计算距离
        # 声音在空气中的速度大约是 0.034 厘米/微秒
        # 距离 = (时间 × 速度) / 2，因为声音走了来回
        distance = duration * 0.034 / 2

        # 限制超声波测距范围，超过 3m 或者低于 2cm 就不准了
        if distance < 2 or distance > 300:
            distance = 0

        # 返回计算出的距离（厘米）
        return int(distance)
    
    """
    红外接收代码
    """
# webpage.py - 网页 HTML 内容（对应 Arduino 的 webpage.h）
# 所有 HTML、CSS、JavaScript 都放在这里，主程序只需 import 即可

INDEX_HTML = """<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<style>
body{font-family:Arial;text-align:center;background:#f4f4f4;margin:10px}
.card{background:#fff;border-radius:12px;padding:10px;max-width:500px;margin:auto}
.grid{display:grid;grid-template-columns:repeat(3,1fr);gap:10px}
button{height:80px;font-size:22px;border:none;border-radius:12px;color:#fff}
.f{background:#4CAF50}.b{background:#F44336}.l{background:#FF9800}
.r{background:#2196F3}.s{background:#666}.x{background:#009688}.v{background:#9C27B0}
</style>
</head>
<body>
<div class="card">
<h2>ESP32 智能小车</h2>
<p>超声波：<span id="d">0</span> cm</p>
<p id="st">等待指令</p>
<div class="grid">
<button class="v" onclick="cmd('servo_plus')">舵机+</button>
<button class="f" onmousedown="cmd('forward')" onmouseup="cmd('stop')" ontouchstart="cmd('forward')" ontouchend="cmd('stop')">↑</button>
<button class="v" onclick="cmd('servo_minus')">舵机-</button>

<button class="l" onmousedown="cmd('left')" onmouseup="cmd('stop')" ontouchstart="cmd('left')" ontouchend="cmd('stop')">←</button>
<button class="s" onclick="cmd('stop')">■</button>
<button class="r" onmousedown="cmd('right')" onmouseup="cmd('stop')" ontouchstart="cmd('right')" ontouchend="cmd('stop')">→</button>

<button class="x" onclick="cmd('claw_open')">夹开</button>
<button class="b" onmousedown="cmd('backward')" onmouseup="cmd('stop')" ontouchstart="cmd('backward')" ontouchend="cmd('stop')">↓</button>
<button class="x" onclick="cmd('claw_close')">夹关</button>
</div>
</div>
<script>
function cmd(c){
    document.getElementById('st').innerText=c;
    fetch('/cmd?move='+c);
}
setInterval(()=>{
    fetch('/distance').then(r=>r.text()).then(t=>d.innerText=t);
},1000);
</script>
</body>
</html>
"""