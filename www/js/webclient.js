/* AetherMUD browser telnet client.
 * Connects via WebSocket to the telnet-to-WS bridge (default port 1129),
 * which relays to the MUD telnet port (1122).
 */
(function () {
  "use strict";

  var WS_PORT = 1129;
  var MAX_LINES = 4000;
  var history = [];
  var histIndex = -1;
  var draft = "";
  var socket = null;
  var ansiState = { fg: null, bg: null, bold: false };

  var statusEl = document.getElementById("webclient-status");
  var outputEl = document.getElementById("webclient-output");
  var inputEl = document.getElementById("webclient-input");
  var connectBtn = document.getElementById("webclient-connect");
  var disconnectBtn = document.getElementById("webclient-disconnect");
  var clearBtn = document.getElementById("webclient-clear");

  if (!statusEl || !outputEl || !inputEl) return;

  function wsUrl() {
    var host = window.location.hostname || "localhost";
    var proto = window.location.protocol === "https:" ? "wss:" : "ws:";
    var override = outputEl.getAttribute("data-ws-url");
    if (override) return override;
    var portAttr = outputEl.getAttribute("data-ws-port");
    var port = portAttr ? parseInt(portAttr, 10) : WS_PORT;
    /* Local file:// pages still need a real host. */
    if (!host || host === "") host = "localhost";
    return proto + "//" + host + ":" + port + "/";
  }

  function setStatus(kind, text) {
    statusEl.className = "webclient-status webclient-status--" + kind;
    statusEl.textContent = text;
  }

  function setConnected(on) {
    inputEl.disabled = !on;
    if (connectBtn) connectBtn.disabled = on;
    if (disconnectBtn) disconnectBtn.disabled = !on;
    if (on) inputEl.focus();
  }

  function scrollBottom() {
    outputEl.scrollTop = outputEl.scrollHeight;
  }

  function trimOutput() {
    while (outputEl.childNodes.length > MAX_LINES) {
      outputEl.removeChild(outputEl.firstChild);
    }
  }

  function appendNode(node) {
    outputEl.appendChild(node);
    trimOutput();
    scrollBottom();
  }

  function ansiColor(code) {
    var map = {
      30: "#6a7070",
      31: "#c07070",
      32: "#70c070",
      33: "#c0c070",
      34: "#7090c0",
      35: "#c070c0",
      36: "#70c0c0",
      37: "#c8cccc",
      90: "#8a9292",
      91: "#e09090",
      92: "#90e090",
      93: "#e0e090",
      94: "#90b0e0",
      95: "#e090e0",
      96: "#90e0e0",
      97: "#ffffff"
    };
    return map[code] || null;
  }

  function applySgr(params) {
    var i, n;
    if (!params.length) params = [0];
    for (i = 0; i < params.length; i++) {
      n = params[i];
      if (n === 0) {
        ansiState.fg = null;
        ansiState.bg = null;
        ansiState.bold = false;
      } else if (n === 1) {
        ansiState.bold = true;
      } else if (n === 22) {
        ansiState.bold = false;
      } else if (n >= 30 && n <= 37) {
        ansiState.fg = ansiColor(n);
      } else if (n >= 90 && n <= 97) {
        ansiState.fg = ansiColor(n);
      } else if (n === 39) {
        ansiState.fg = null;
      } else if (n >= 40 && n <= 47) {
        ansiState.bg = ansiColor(n - 10);
      } else if (n === 49) {
        ansiState.bg = null;
      }
    }
  }

  function styledSpan(text) {
    var span = document.createElement("span");
    span.textContent = text;
    if (ansiState.fg) span.style.color = ansiState.fg;
    if (ansiState.bg) span.style.backgroundColor = ansiState.bg;
    if (ansiState.bold) span.style.fontWeight = "bold";
    return span;
  }

  /* Strip telnet IAC negotiation; keep printable / ANSI text. */
  function stripTelnet(bytes) {
    var out = [];
    var i = 0;
    var b;
    while (i < bytes.length) {
      b = bytes[i];
      if (b === 255) {
        /* IAC */
        if (i + 1 >= bytes.length) break;
        var cmd = bytes[i + 1];
        if (cmd === 255) {
          out.push(255);
          i += 2;
        } else if (cmd === 250) {
          /* SB ... SE */
          i += 2;
          while (i < bytes.length) {
            if (bytes[i] === 255 && i + 1 < bytes.length && bytes[i + 1] === 240) {
              i += 2;
              break;
            }
            i += 1;
          }
        } else {
          /* WILL/WONT/DO/DONT + option */
          i += (cmd >= 251 && cmd <= 254) ? 3 : 2;
        }
      } else if (b === 0) {
        i += 1;
      } else {
        out.push(b);
        i += 1;
      }
    }
    return out;
  }

  function utf8FromBytes(arr) {
    try {
      return new TextDecoder("utf-8", { fatal: false }).decode(Uint8Array.from(arr));
    } catch (e) {
      var s = "";
      var i;
      for (i = 0; i < arr.length; i++) s += String.fromCharCode(arr[i]);
      return s;
    }
  }

  function renderAnsiChunk(text) {
    var frag = document.createDocumentFragment();
    var re = /\u001b\[([0-9;]*)m/g;
    var last = 0;
    var m;
    var chunk;
    while ((m = re.exec(text)) !== null) {
      if (m.index > last) {
        chunk = text.slice(last, m.index);
        if (chunk) frag.appendChild(styledSpan(chunk));
      }
      applySgr(
        m[1]
          ? m[1].split(";").map(function (x) {
              return parseInt(x, 10) || 0;
            })
          : [0]
      );
      last = m.index + m[0].length;
    }
    if (last < text.length) frag.appendChild(styledSpan(text.slice(last)));
    return frag;
  }

  function writeIncoming(rawText) {
    /* Normalize CR LF and lone CR. */
    var text = rawText.replace(/\r\n/g, "\n").replace(/\r/g, "\n");
    var lines = text.split("\n");
    var i;
    var line;
    var row;
    for (i = 0; i < lines.length; i++) {
      line = lines[i];
      if (i < lines.length - 1) {
        row = document.createElement("div");
        if (line.length) row.appendChild(renderAnsiChunk(line));
        else row.appendChild(document.createTextNode(""));
        appendNode(row);
      } else if (line.length) {
        /* Trailing partial line without newline: append to last div or new. */
        var last = outputEl.lastChild;
        if (!last || last.nodeName !== "DIV" || last.getAttribute("data-partial") !== "1") {
          last = document.createElement("div");
          last.setAttribute("data-partial", "1");
          appendNode(last);
        }
        last.appendChild(renderAnsiChunk(line));
      }
    }
    /* Completed lines clear partial marker on previous. */
    if (lines.length > 1) {
      var nodes = outputEl.querySelectorAll('[data-partial="1"]');
      for (i = 0; i < nodes.length - (lines[lines.length - 1].length ? 1 : 0); i++) {
        nodes[i].removeAttribute("data-partial");
      }
    }
  }

  function handleMessage(ev) {
    var data = ev.data;
    if (typeof data === "string") {
      writeIncoming(data);
      return;
    }
    var buf;
    if (data instanceof ArrayBuffer) {
      buf = new Uint8Array(data);
      writeIncoming(utf8FromBytes(stripTelnet(buf)));
      return;
    }
    /* Blob fallback */
    var reader = new FileReader();
    reader.onload = function () {
      buf = new Uint8Array(reader.result);
      writeIncoming(utf8FromBytes(stripTelnet(buf)));
    };
    reader.readAsArrayBuffer(data);
  }

  function connect() {
    if (socket && (socket.readyState === WebSocket.OPEN || socket.readyState === WebSocket.CONNECTING)) {
      return;
    }
    var url = wsUrl();
    setStatus("connecting", "Connecting to " + url + " ...");
    setConnected(false);
    try {
      socket = new WebSocket(url);
    } catch (err) {
      setStatus("disconnected", "Failed to open WebSocket");
      writeIncoming("\n[client] " + err + "\n");
      return;
    }
    socket.binaryType = "arraybuffer";
    socket.onopen = function () {
      setStatus("connected", "Connected");
      setConnected(true);
      writeIncoming("\n[client] Connected to AetherMUD.\n");
    };
    socket.onmessage = handleMessage;
    socket.onerror = function () {
      setStatus("disconnected", "Connection error");
    };
    socket.onclose = function () {
      setStatus("disconnected", "Disconnected");
      setConnected(false);
      socket = null;
    };
  }

  function disconnect() {
    if (socket) {
      try {
        socket.close();
      } catch (e) {}
      socket = null;
    }
    setStatus("disconnected", "Disconnected");
    setConnected(false);
  }

  function sendLine(line) {
    if (!socket || socket.readyState !== WebSocket.OPEN) return;
    socket.send(line + "\n");
    var echo = document.createElement("div");
    echo.style.color = "#8a9292";
    echo.textContent = "> " + line;
    appendNode(echo);
  }

  inputEl.addEventListener("keydown", function (ev) {
    if (ev.key === "Enter") {
      ev.preventDefault();
      var line = inputEl.value;
      inputEl.value = "";
      if (line.length) {
        history.push(line);
        if (history.length > 200) history.shift();
      }
      histIndex = -1;
      draft = "";
      sendLine(line);
      return;
    }
    if (ev.key === "ArrowUp") {
      ev.preventDefault();
      if (!history.length) return;
      if (histIndex === -1) {
        draft = inputEl.value;
        histIndex = history.length - 1;
      } else if (histIndex > 0) {
        histIndex -= 1;
      }
      inputEl.value = history[histIndex];
      return;
    }
    if (ev.key === "ArrowDown") {
      ev.preventDefault();
      if (histIndex === -1) return;
      if (histIndex < history.length - 1) {
        histIndex += 1;
        inputEl.value = history[histIndex];
      } else {
        histIndex = -1;
        inputEl.value = draft;
      }
    }
  });

  if (connectBtn) connectBtn.addEventListener("click", connect);
  if (disconnectBtn) disconnectBtn.addEventListener("click", disconnect);
  if (clearBtn)
    clearBtn.addEventListener("click", function () {
      outputEl.innerHTML = "";
    });

  setStatus("disconnected", "Disconnected");
  setConnected(false);

  /* Auto-connect when the play page loads. */
  if (outputEl.getAttribute("data-autoconnect") !== "0") {
    connect();
  }
})();
