# 导入专门为这块小车开发板写好的“控制工具箱”，里面封装好了控制蜂鸣器等硬件的现成命令
from ESP32S3_4WD_Car import Keyes_ESP32S3_4WD
# 导入“时间”工具箱，用来让程序等待（延时），从而控制声音的节奏
import time

# 创建一个名为 car 的对象，相当于唤醒了我们的开发板小车，准备接受指令
car = Keyes_ESP32S3_4WD()

# 定义一个变量 buzzer_pin，告诉程序蜂鸣器连接在第 10 号引脚（IO10）上
buzzer_pin = 10

# 初始化板载蜂鸣器，让开发板准备好通过 10 号引脚给蜂鸣器发送控制信号
car.Buzzer_init(buzzer_pin)

# 开启一个无限循环，让下面的代码一直重复执行，直到你手动停止程序
while True:
    
    # 在电脑上打印一行提示文字“Playing Alarm...”，方便你知道程序运行到哪一步了
    print("Playing Alarm...")
    # 1. 警报阶段：发出短促、尖锐的“滴-滴-滴”声
    # 使用 for 循环，让 i 从 0 变到 2，总共执行 3 次循环
    for i in range(3):
        # 调用播放命令：频率设为 1000Hz（声音尖锐），持续发声 200毫秒（0.2秒）
        car.Buzzer_play(1000, 200)
        # 让程序暂停 200毫秒，形成“滴”和“滴”之间的停顿间隔
        time.sleep_ms(200)

    # 2. 休息阶段
    # 在电脑上打印提示文字“Resting...”，表示警报结束，开始休息
    print("Resting...")
    # 让程序安静地暂停 1000毫秒（也就是 1 秒钟），这段时间蜂鸣器不响
    time.sleep_ms(1000)

    # 3. 音乐阶段：演奏 Do-Re-Mi-Fa-Sol-La-Si 音阶
    # 在电脑上打印提示文字“Playing Scale...”，表示开始演奏音阶
    print("Playing Scale...")
    # 调用现成的函数，播放低音 Do（频率约 262Hz）
    car.Buzzer_play_Do()  # 低 Do
    # 调用现成的函数，播放低音 Re（频率约 294Hz）
    car.Buzzer_play_Re()  # 低 Re
    # 调用现成的函数，播放低音 Mi（频率约 330Hz）
    car.Buzzer_play_Mi()  # 低 Mi
    # 调用现成的函数，播放低音 Fa（频率约 349Hz）
    car.Buzzer_play_Fa()  # 低 Fa
    # 调用现成的函数，播放低音 Sol（频率约 392Hz）
    car.Buzzer_play_Sol()  # 低 Sol
    # 调用现成的函数，播放低音 La（频率约 440Hz）
    car.Buzzer_play_La()  # 低 La
    # 调用现成的函数，播放低音 Si（频率约 494Hz）
    car.Buzzer_play_Si()  # 低 Si

    # 4. 循环前的长休息
    # 让程序安静地暂停 2000毫秒（也就是 2 秒），准备重新开始下一轮循环
    time.sleep_ms(2000)
