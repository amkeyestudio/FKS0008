# 从我们提前准备好的“工具箱”（ESP32S3_4WD_Car库）里，拿出专门控制这块开发板的“魔法书”（Keyes_ESP32S3_4WD类）
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD

# 导入“时间”工具箱，这样我们就能让程序“等一等”、“睡一会儿”（延时）
import time

# 创建一个名为 matrix（矩阵）的变量，让它拥有控制开发板的超能力
matrix = Keyes_ESP32S3_4WD()

# 定义通信引脚，与我们的物理接线表保持完全一致
CLK_PIN = 9   # 时钟引脚，连接模块上的 CLK 引脚（指挥棒）
DIN_PIN = 8   # 数据引脚，连接模块上的 DIN 引脚（传话筒）

# 告诉“魔法书”，我们要用哪两个引脚来和点阵模块沟通
matrix.Matrix_init(CLK_PIN, DIN_PIN)


# 定义“微笑”图案的数据 (16字节，对应16列，每字节8位对应8行)
# 这些 0x 开头的数字，就是我们在取模工具里生成的“图案密码”
smile = bytes([
    0x00, 0x00, 0x1C, 0x02, 0x02, 0x02, 0x5C, 0x40,
    0x40, 0x5C, 0x02, 0x02, 0x02, 0x1C, 0x00, 0x00
])

# 定义“爱心”图案的数据
heart = bytes([
    0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x7c,
    0x7c, 0x3e, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00
])


# 初始化清屏操作，发送 16 个 0 字节，把所有灯都关掉，防止刚通电时显示乱码
matrix.Matrix_display(bytes(16))       

# 主循环：让代码像跑圈一样，一直重复执行下面的内容
while True:
    # 1. 显示爱心图案，保持 3 秒
    matrix.Matrix_display(heart)       # 把“爱心密码”发送给点阵模块，让它显示爱心
    time.sleep(3)               # 让程序“睡”3秒钟（期间爱心一直亮着）

    # 2. 显示微笑图案，保持 2 秒
    matrix.Matrix_display(smile)       # 把“微笑密码”发送给点阵模块，替换掉爱心
    time.sleep(2)               # 让程序“睡”2秒钟（期间微笑一直亮着）

    # 3. 简单的滚动效果演示 (调用内置的滚动函数，将微笑数据循环左移)
    matrix.Matrix_Scroll_display(smile)
