// =====================================================
// index_html.h
// ESP32-S3 智能小车 网页界面
// =====================================================

#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Smart Car</title>
<style>
* { box-sizing: border-box; margin: 0; padding: 0; }

body {
  background: #111;
  color: white;
  font-family: Arial, "Microsoft YaHei", sans-serif;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 15px;
  user-select: none;
  -webkit-user-select: none;
}

.title {
  width: 100%;
  max-width: 640px;
  text-align: center;
  font-size: 24px;
  font-weight: bold;
  margin-bottom: 12px;
}

.video-container {
  width: 100%;
  max-width: 480px;
  aspect-ratio: 4 / 3;
  background: black;
  border-radius: 12px;
  overflow: hidden;
  display: flex;
  justify-content: center;
  align-items: center;
  box-shadow: 0 4px 15px rgba(0,0,0,0.5);
}

.video-container img {
  width: 100%;
  height: 100%;
  object-fit: contain;
  display: block;
  background: black;
}

.status {
  width: 100%;
  max-width: 640px;
  text-align: center;
  margin-top: 10px;
  font-size: 15px;
  color: #aaa;
}

.control-panel {
  width: 100%;
  max-width: 420px;
  margin-top: 18px;
}

.control-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  grid-template-rows: repeat(3, 1fr);
  gap: 10px;
  width: 100%;
  aspect-ratio: 1 / 1;
}

.control-btn {
  border: none;
  border-radius: 18px;
  background: #292929;
  color: white;
  font-size: 30px;
  font-weight: bold;
  cursor: pointer;
  user-select: none;
  -webkit-user-select: none;
  touch-action: manipulation;
  transition: transform 0.08s, background 0.08s;
  box-shadow: 0 4px 8px rgba(0,0,0,0.4);
}

.control-btn:active {
  transform: scale(0.94);
  background: #555;
}

.direction { background: #333; }
.stop      { background: #8b1e1e; font-size: 20px; }
.servo     { background: #234d6f; font-size: 17px; }
.extension { background: #3a3a3a; color: #888; font-size: 16px; }

.button-text {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  height: 100%;
  gap: 3px;
}

.button-small {
  font-size: 12px;
  font-weight: normal;
  opacity: 0.8;
}
</style>
</head>

<body>

<div class="title">Smart Car</div>

<!-- 视频 -->
<div class="video-container">
  <img id="cameraStream" src="/stream" alt="Camera video">
</div>

<!-- 状态 -->
<div class="status">Current Status:<span id="statusText">Stop</span></div>

<!-- 九宫格控制 -->
<div class="control-panel">
<div class="control-grid">

  <!-- 功能1 -->
  <button class="control-btn extension" onclick="sendCommand('1')">
    <div class="button-text"><span>A</span><span class="button-small">A</span></div>
  </button>

  <!-- 前进 -->
  <button class="control-btn direction"
    onpointerdown="startMove(event,'F')"
    onpointerup="stopMove(event)"
    onpointercancel="stopMove(event)"
    onpointerleave="stopMove(event)">&#8593;</button>

  <!-- 功能2 -->
  <button class="control-btn extension" onclick="sendCommand('2')">
    <div class="button-text"><span>a</span><span class="button-small">a</span></div>
  </button>

  <!-- 左转 -->
  <button class="control-btn direction"
    onpointerdown="startMove(event,'L')"
    onpointerup="stopMove(event)"
    onpointercancel="stopMove(event)"
    onpointerleave="stopMove(event)">&#8592;</button>

  <!-- STOP -->
  <button class="control-btn stop" onclick="sendCommand('S')">STOP</button>

  <!-- 右转 -->
  <button class="control-btn direction"
    onpointerdown="startMove(event,'R')"
    onpointerup="stopMove(event)"
    onpointercancel="stopMove(event)"
    onpointerleave="stopMove(event)">&#8594;</button>

  <!-- 舵机开 -->
  <button class="control-btn servo" onclick="sendCommand('O')">
    <div class="button-text"><span>Open</span><span class="button-small">OPEN</span></div>
  </button>

  <!-- 后退 -->
  <button class="control-btn direction"
    onpointerdown="startMove(event,'B')"
    onpointerup="stopMove(event)"
    onpointercancel="stopMove(event)"
    onpointerleave="stopMove(event)">&#8595;</button>

  <!-- 舵机关 -->
  <button class="control-btn servo" onclick="sendCommand('C')">
    <div class="button-text"><span>Close</span><span class="button-small">CLOSE</span></div>
  </button>

</div>
</div>

<script>
let currentMove = null;

// 命令 → 状态文字 映射表
const STATUS_MAP = {
  F: "F",  B: "B",
  L: "L",     R: "R",
  S: "S",
  O: "O", C: "C",
  1: "A", 2: "a"
};

// 键盘 → 命令 映射表
const KEY_MAP = {
  ArrowUp: "F",  ArrowDown: "B",
  ArrowLeft: "L",   ArrowRight: "R"
};

function updateStatus(cmd) {
  document.getElementById("statusText").innerText =
    STATUS_MAP[cmd] || "Unknown command";
}

function sendCommand(command) {
  fetch("/control?cmd=" + encodeURIComponent(command),
        { method: "GET", cache: "no-store" })
    .then(r => {
      if (!r.ok) throw new Error("HTTP " + r.status);
      updateStatus(command);
    })
    .catch(e => {
      console.error("Control error:", e);
      document.getElementById("statusText").innerText = "Connection failed";
    });
}

function startMove(event, command) {
  event.preventDefault();
  currentMove = command;
  sendCommand(command);
}

function stopMove(event) {
  if (event) event.preventDefault();
  if (currentMove !== null) {
    currentMove = null;
    sendCommand("S");
  }
}

// 键盘按下
document.addEventListener("keydown", function(event) {
  if (event.repeat) return;

  if (event.key === " ") {
    event.preventDefault();
    sendCommand("S");
    return;
  }

  const command = KEY_MAP[event.key];
  if (command) {
    event.preventDefault();
    currentMove = command;
    sendCommand(command);
  }
});

// 键盘松开
document.addEventListener("keyup", function(event) {
  if (KEY_MAP[event.key]) {
    event.preventDefault();
    stopMove(null);
  }
});

// 失去焦点自动停止（切窗口 / 手机切后台）
window.addEventListener("blur", () => stopMove(null));
</script>

</body>
</html>
)rawliteral";

#endif // INDEX_HTML_H