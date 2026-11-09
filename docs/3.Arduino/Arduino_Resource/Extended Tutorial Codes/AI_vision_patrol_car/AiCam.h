#ifndef AICAM_H
#define AICAM_H

#include <Arduino.h>

// ---------- 平台类型别名 ----------
#ifdef ESP32
  // ESP32 用硬件串口指针
  #define AiCamSerial HardwareSerial*
  #define DEFAULT_ESP_RX 16
  #define DEFAULT_ESP_TX 17
#elif defined(ARDUINO_AVR_UNO)
  // Uno 用软件串口对象
  #include <SoftwareSerial.h>
  #define AiCamSerial SoftwareSerial
  #define DEFAULT_ESP_RX 2
  #define DEFAULT_ESP_TX 3
#else
  #error "Unsupported board. Only ESP32 and Arduino Uno are supported."
#endif

// ---------- 默认波特率（仍允许用户覆盖） ----------
// #ifndef USB_BAUD
//   #define USB_BAUD 115200
// #endif
#ifndef ESP_BAUD
  #define ESP_BAUD 9600
#endif

// ---------- 数据存储结构 ----------
struct EspData {
  int faceX = 0;
  int faceY = 0;
  bool faceValid = false;

  String color;
  bool colorValid = false;

  String qr;
  bool qrValid = false;

  String card;
  bool cardValid = false;

  int lineOffset = 0;
  int lineAngle = 0;
  int lineValid = false;
};

class AiCam {
public:
  // 构造函数
  AiCam();                                 // 使用默认引脚
  AiCam(uint8_t rxPin, uint8_t txPin);    // 自定义引脚

  // 初始化（必须在 setup() 中调用）
  void begin();

  // 发送命令
  void sendToEsp(const String &cmd);

  // 设置AI Cam 功能模式
  void setAiCamMode(String cmd);

  // 处理串口数据（需在 loop 中反复调用）
  void readEspSerial();
  void readUsbSerial();

  // 数据访问
  int getFaceX();
  int getFaceY();
  String getColor();
  String getQrCode();
  String getCard();
  bool isFaceValid();
  int getLineValid();
  int getLineOffset();
  int getLineAngle();

private:
  uint8_t _rxPin, _txPin;

  // 统一的串口成员（类型根据平台不同）
  AiCamSerial _espSerial;

  // 内部数据
  EspData _data;
  String _espLine;   // 接收缓冲区
  String _usbLine;   // USB 命令缓冲区

  // 内部辅助函数
  bool parseEspResponse(const String &strData, String &mode, String &data);
  void updateEspData(const String &mode, const String &data);
};

#endif