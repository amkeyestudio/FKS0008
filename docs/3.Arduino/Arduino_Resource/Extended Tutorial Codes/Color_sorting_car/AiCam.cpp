#include "AiCam.h"

// ---------- 构造函数 ----------
AiCam::AiCam() : AiCam(DEFAULT_ESP_RX, DEFAULT_ESP_TX) {}

AiCam::AiCam(uint8_t rxPin, uint8_t txPin) :
  _rxPin(rxPin),
  _txPin(txPin)
  #ifdef ARDUINO_AVR_UNO
    , _espSerial(rxPin, txPin)   // SoftwareSerial 需要在此构造
  #endif
{
  #ifdef ESP32
    _espSerial = &Serial2;       // 指向 ESP32 硬件串口 2
  #endif
}

// ---------- 初始化 ----------
void AiCam::begin() {
  // USB 监视串口
  // Serial.begin(USB_BAUD);
  // delay(200);

  // 启动 ESP 串口
  #ifdef ESP32
    _espSerial->begin(ESP_BAUD, SERIAL_8N1, _rxPin, _txPin);
  #elif defined(ARDUINO_AVR_UNO)
    _espSerial.begin(ESP_BAUD);
  #endif

  delay(500);
  Serial.println();
  Serial.println(F("UART Controller Start"));
  Serial.print(F("RX pin = "));
  Serial.println(_rxPin);
  Serial.print(F("TX pin = "));
  Serial.println(_txPin);

  // 上电查询主机状态（可选）
  sendToEsp(F("GET:MODE"));
  delay(50);
  sendToEsp(F("GET:OUTPUT"));
  delay(50);
}

// ---------- 发送命令 ----------
void AiCam::sendToEsp(const String &cmd) {
  if (cmd.length() == 0) return;
  #ifdef ESP32
    _espSerial->print(cmd);
    _espSerial->print("\r\n");
  #else
    _espSerial.print(cmd);
    _espSerial.print("\r\n");
  #endif
  Serial.print(F("SEND -> "));
  Serial.println(cmd);
}

// ---------- USB 命令处理 ----------
void AiCam::setAiCamMode(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;
  String lower = cmd;
  lower.toLowerCase();

  if (lower == "face") {
    sendToEsp(F("MODE:FACE"));
  } else if (lower == "color") {
    sendToEsp(F("MODE:COLOR"));
  } else if (lower == "qr") {
    sendToEsp(F("MODE:QR"));
  } else if (lower == "card") {
    sendToEsp(F("MODE:CARD"));
  } else if (lower == "line") {
    sendToEsp(F("MODE:LINE"));
  } else {
    Serial.print(F("Unknown local command: "));
    Serial.println(cmd);
  }
}

// ---------- 解析响应 ----------
bool AiCam::parseEspResponse(const String &strData, String &mode, String &data) {
  int firstColon = strData.indexOf(':');
  int secondColon = strData.indexOf(':', firstColon + 1);
  if (firstColon != -1 && secondColon != -1) {
    mode = strData.substring(firstColon + 1, secondColon);
    data = strData.substring(secondColon + 1);
    return true;
  }
  return false;
}

// ---------- 更新数据缓存 ----------
void AiCam::updateEspData(const String &mode, const String &data) {
  if (mode.equalsIgnoreCase("FACE")) {
    int comma = data.indexOf(',');
    if (comma != -1) {
      _data.faceX = data.substring(0, comma).toInt();
      _data.faceY = data.substring(comma + 1).toInt();
	  if (_data.faceX > 1){
		  _data.faceValid = true;
	  }else _data.faceValid = false;
    }
  } else if (mode.equalsIgnoreCase("COLOR")) {
    _data.color = data;
    _data.colorValid = true;
  } else if (mode.equalsIgnoreCase("QR")) {
    _data.qr = data;
    _data.qrValid = true;
  } else if (mode.equalsIgnoreCase("CARD")) {
    _data.card = data;
    _data.cardValid = true;
  } else if (mode.equalsIgnoreCase("LINE")) {
    int firstComma = data.indexOf(',');
    if (firstComma != -1) {
        int secondComma = data.indexOf(',', firstComma + 1);
        if (secondComma != -1) {
            _data.lineOffset = data.substring(0, firstComma).toInt();
            _data.lineAngle  = data.substring(firstComma + 1, secondComma).toInt();
            _data.lineValid  = data.substring(secondComma + 1).toInt();
        } else {
            // 如果只有两个字段，可以按需处理，或打印错误
            Serial.println(F("LINE data missing third field"));
        }
    }
  } else {
    Serial.print(F("Unknown local command:"));
    Serial.println(mode);
  }
}

// ---------- 读取 ESP 串口 ----------
void AiCam::readEspSerial() {
  #ifdef ESP32
    while (_espSerial->available()) {
      char c = (char)_espSerial->read();
  #else
    while (_espSerial.available()) {
      char c = (char)_espSerial.read();
  #endif
      if (c == '\r' || c == '\n') {
        if (_espLine.length() > 0) {
          String mode, data;
          if (parseEspResponse(_espLine, mode, data)) {
            updateEspData(mode, data);
          }
          _espLine = "";
        }
      } else {
        _espLine += c;
      }
    }
}

// ---------- 读取 USB 串口 ----------
void AiCam::readUsbSerial() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') {
      if (_usbLine.length() > 0) {
        setAiCamMode(_usbLine);
        _usbLine = "";
      }
    } else {
      _usbLine += c;
    }
  }
}

// ---------- Getter 实现 ----------
int AiCam::getFaceX()     { return _data.faceX; }
int AiCam::getFaceY()     { return _data.faceY; }
String AiCam::getColor()  { return _data.color; }
String AiCam::getQrCode() { return _data.qr; }
String AiCam::getCard()   { return _data.card; }
bool AiCam::isFaceValid() { return _data.faceValid; }
int AiCam::getLineValid() { return _data.lineValid; }
int AiCam::getLineOffset() { return _data.lineOffset; }
int AiCam::getLineAngle() { return _data.lineAngle; }

