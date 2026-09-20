#include <Wire.h>       // 引入 Arduino 内置的 I2C (Wire) 通信库
#include <PCF8574.h>    // 引入 PCF8574 I2C 扩展芯片的驱动库

// 实例化 PCF8574 对象，设置其 I2C 地址为 0x20（该模块的默认地址）
PCF8574 pcf8574(0x20);

// 定义 5 路巡线传感器的输出引脚，对应 PCF8574 的 P0-P4 引脚
#define OUTA P4  // 探头 A (最左侧)
#define OUTB P0  // 探头 B (左侧)
#define OUTC P1  // 探头 C (中间)
#define OUTD P2  // 探头 D (右侧)
#define OUTE P3  // 探头 E (最右侧)


void setup()
{
    // 初始化串口通信，波特率设置为 115200
    Serial.begin(115200);
    Serial.println("5路巡线传感器初始化完成!");

    // 将 PCF8574 的 5 个引脚配置为输入模式，用于读取传感器状态
    pcf8574.pinMode(OUTA, INPUT);
    pcf8574.pinMode(OUTB, INPUT);
    pcf8574.pinMode(OUTC, INPUT);
    pcf8574.pinMode(OUTD, INPUT);
    pcf8574.pinMode(OUTE, INPUT);

    // 启动 I2C 通信
    pcf8574.begin();
}

void loop()
{
    // 依次读取 5 个探头的状态，并通过串口打印，中间用空格隔开
    Serial.print(pcf8574.digitalRead(OUTA));
    Serial.print(" ");
    Serial.print(pcf8574.digitalRead(OUTB));
    Serial.print(" ");
    Serial.print(pcf8574.digitalRead(OUTC));
    Serial.print(" ");
    Serial.print(pcf8574.digitalRead(OUTD));
    Serial.print(" ");
    Serial.println(pcf8574.digitalRead(OUTE)); // 最后一个探头使用 println 打印并换行

    // 延时 500 毫秒，控制数据刷新频率，便于肉眼观察
    delay(500);
}