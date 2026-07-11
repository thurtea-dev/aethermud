#!/usr/bin/bash
# Start/stop modern FluffOS porting instance. Does not touch live game on 1122.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DRIVER="$SCRIPT_DIR/bin-modern/driver"
CFG="$SCRIPT_DIR/bin-modern/fluffos.cfg"
LOG="$SCRIPT_DIR/lib-modern/log/runtime"
PIDFILE="$SCRIPT_DIR/bin-modern/modern.pid"
PORT=1124

case "$1" in
start)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then
        echo "[modern] Already running PID $(cat "$PIDFILE")"
        exit 0
    fi
    cd "$SCRIPT_DIR/bin-modern" || exit 1
    nohup "$DRIVER" "$CFG" >> "$LOG" 2>&1 &
    echo $! > "$PIDFILE"
    echo "[modern] Started PID $(cat "$PIDFILE") on port $PORT"
    ;;
stop)
    if [ -f "$PIDFILE" ]; then
        kill "$(cat "$PIDFILE")" 2>/dev/null
        rm -f "$PIDFILE"
    fi
    pkill -f "$CFG" 2>/dev/null
    echo "[modern] Stopped"
    ;;
status)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then
        echo "[modern] Running PID $(cat "$PIDFILE")"
        ss -tlnp 2>/dev/null | grep "$PORT" || true
    else
        echo "[modern] Not running"
    fi
    ;;
log)
    tail -f "$LOG"
    ;;
*)
    echo "Usage: $0 {start|stop|status|log}"
    exit 1
    ;;
esac
