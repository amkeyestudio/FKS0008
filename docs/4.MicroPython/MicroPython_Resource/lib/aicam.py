from machine import UART, Pin
import time

class AiCam:

    # AI模式
    FACE = 0
    COLOR = 1
    QR = 2
    CARD = 3
    LINE = 4

    def __init__(self):
        self.uart = None

        # 数据缓存
        self.faceXValue = 0
        self.faceYValue = 0
        self.faceValid = False

        self.colorValue = ""
        self.qrValue = ""
        self.cardValue = ""
        
        self.lineOffsetValue = 0
        self.lineAngleValue = 0
        self.lineValidValue = 0

    # =====================
    # 初始化串口（传入引脚编号或 Pin 对象，波特率）
    # =====================
    def begin(self, tx, rx, baud):
        # 如果传入的是数字，转为 Pin 对象
        if isinstance(tx, int):
            tx = Pin(tx, Pin.OUT)
        if isinstance(rx, int):
            rx = Pin(rx, Pin.IN)

        # ESP32 常用 UART2（或 UART1），这里以 2 为例，可根据实际修改
        self.uart = UART(2, baudrate=baud, tx=tx, rx=rx, timeout=0)
        time.sleep_ms(200)
        
    def flush(self):
        while self.uart.any():
            self.uart.read()

    # =====================
    # 读取数据
    # =====================
    def readData(self):
        if self.uart is None:
            return
        if self.uart.any():
            line = self.uart.readline()
            if line:
                try:
                    line = line.decode("utf-8").strip()
                    self.parseLine(line)
                except:
                    pass

    # =====================
    # 数据解析
    # =====================
    def parseLine(self, line):
        if len(line) == 0:
            return
        parts = line.split(":")
        if len(parts) < 3:
            return
        mode = parts[1]
        # 防止二维码中出现冒号
        data = ":".join(parts[2:])

        if mode == "FACE":
            xy = data.split(",")
            if len(xy) >= 2:
                try:
                    self.faceXValue = int(xy[0])
                    self.faceYValue = int(xy[1])
                    if self.faceXValue != 0 and self.faceYValue != 0:
                        self.faceValid = True
                    else:
                        self.faceValid = False
                except:
                    self.faceValid = False

        elif mode == "COLOR":
            self.colorValue = data

        elif mode == "QR":
            self.qrValue = data

        elif mode == "CARD":
            self.cardValue = data
        elif mode == "LINE":
            lineVal = data.split(",")
            if len(lineVal) == 3:
                self.lineOffsetValue = int(lineVal[0])
                self.lineAngleValue = int(lineVal[1])
                self.lineValidValue = int(lineVal[2])                    

    # =====================
    # 设置模式
    # =====================
    def setMode(self, mode):
        if self.uart is None:
            return
        if mode == self.FACE:
            cmd = "MODE:FACE\r\n"
        elif mode == self.COLOR:
            cmd = "MODE:COLOR\r\n"
        elif mode == self.QR:
            cmd = "MODE:QR\r\n"
        elif mode == self.CARD:
            cmd = "MODE:CARD\r\n"
        elif mode == self.LINE:
            cmd = "MODE:LINE\r\n"
        else:
            return

        for i in range(3):
            self.uart.write(cmd)
            time.sleep_ms(50)

    # =====================
    # 人脸接口
    # =====================
    def faceDetected(self):
        return self.faceValid

    def faceX(self):
        return self.faceXValue

    def faceY(self):
        return self.faceYValue

    # =====================
    # 颜色接口
    # =====================
    def color(self):
        return self.colorValue

    # =====================
    # 二维码接口
    # =====================
    def qr(self):
        return self.qrValue

    # =====================
    # 卡片接口
    # =====================
    def card(self):
        return self.cardValue
    # =====================
    # 线识别接口
    # =====================
    def lineOffset(self):
        return self.lineOffsetValue
    
    def lineAngle(self):
        return self.lineAngleValue
    
    def lineValid(self):
        return self.lineValidValue
    
    