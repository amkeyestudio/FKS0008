#include <IRremote.hpp> // 引入 IRremote 红外遥控库

// 定义红外接收模块信号引脚连接的 GPIO 编号
#define IR_RECEIVE_PIN 48 

void setup()
{
    // 初始化串口通信，设置波特率为 115200
    Serial.begin(115200);
    delay(1000); // 延时 1 秒，等待串口稳定

    Serial.println("ESP32-S3 IR Receiver Start");
    // 初始化红外接收器，指定引脚并禁用板载 LED 反馈闪烁
    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
    Serial.println("Waiting IR...");
}

void loop()
{
    // 检查是否成功解码到红外信号
    if (IrReceiver.decode())
    {
        Serial.println("------ IR DATA ------");
        Serial.print("IR Value:");
        // 以十进制格式打印解码后的命令码
        Serial.println(IrReceiver.decodedIRData.command);
        Serial.println("---------------------");
        
        // 恢复接收状态，清空缓存，准备接收下一个红外信号
        IrReceiver.resume(); 
    }
}