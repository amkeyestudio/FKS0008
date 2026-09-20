#ifndef WEBPAGE_H
#define WEBPAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<style>
body{font-family:Arial;text-align:center;background:#f4f4f4;margin:10px}
.card{background:#fff;border-radius:12px;padding:10px;max-width:500px;margin:auto}
.grid{display:grid;grid-template-columns:repeat(3,1fr);gap:10px}
button{height:80px;font-size:22px;border:none;border-radius:12px;color:#fff}
.f{background:#4CAF50}.b{background:#F44336}.l{background:#FF9800}
.r{background:#2196F3}.s{background:#666}.x{background:#009688}.v{background:#9C27B0}
</style></head><body>
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
</div></div>
<script>
function cmd(c){document.getElementById('st').innerText=c;fetch('/cmd?move='+c);}
setInterval(()=>{fetch('/distance').then(r=>r.text()).then(t=>d.innerText=t);},500);
</script></body></html>
)rawliteral";

#endif
