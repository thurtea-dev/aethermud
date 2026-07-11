#!/bin/bash
# mud.sh - Nightmare 3 MUD management script
# Usage: ./mud.sh {start|stop|restart|status|log|boot}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MUD_BASE="${MUD_BASE:-$SCRIPT_DIR/nightmare3_fluffos_v2}"
BIN_DIR="$MUD_BASE/bin"
DRIVER="$BIN_DIR/driver"
ADDR_SERVER="$BIN_DIR/addr_server"
CFG="${MUDOS_CFG:-$BIN_DIR/mudos.cfg}"
LOG="$MUD_BASE/lib/log/runtime"
PIDFILE="$BIN_DIR/mud.pid"
ADDR_PIDFILE="$BIN_DIR/addr.pid"
PORT=1122
ADDR_PORT=1199
WS_PORT=1129

get_pid() {
    if [ -f "$PIDFILE" ]; then
        local pid
        pid=$(cat "$PIDFILE")
        if kill -0 "$pid" 2>/dev/null; then
            echo "$pid"
            return
        fi
        rm -f "$PIDFILE"
    fi
    pgrep -f "$DRIVER $CFG" 2>/dev/null | head -1
}

get_addr_pid() {
    if [ -f "$ADDR_PIDFILE" ]; then
        local pid
        pid=$(cat "$ADDR_PIDFILE")
        if kill -0 "$pid" 2>/dev/null; then
            echo "$pid"
            return
        fi
        rm -f "$ADDR_PIDFILE"
    fi
    pgrep -f "$ADDR_SERVER $ADDR_PORT" 2>/dev/null | head -1
}

is_running() {
    [ -n "$(get_pid)" ]
}

verify_ports() {
    local pid="$1"
    local tcp_line udp_line found_extra=0

    if [ -z "$pid" ]; then
        return 0
    fi

    while IFS= read -r tcp_line; do
        if echo "$tcp_line" | grep -qE ":($PORT|$WS_PORT)\b"; then
            continue
        fi
        echo "[mud] ERROR: Driver PID $pid is listening on a forbidden port."
        echo "       $tcp_line"
        echo "[mud] Allowed TCP ports: $PORT (telnet), $WS_PORT (websocket if needed)."
        echo "[mud] Check /daemon/http.c and /secure/daemon/mcp_d.c - their setup()"
        echo "[mud] must not call socket_bind(). A full restart is required to close"
        echo "[mud] LPC-opened sockets."
        found_extra=1
    done < <(ss -tlnp 2>/dev/null | grep "pid=$pid," || true)

    while IFS= read -r udp_line; do
        if echo "$udp_line" | grep -q ":$ADDR_PORT\b"; then
            continue
        fi
        echo "[mud] WARNING: Unexpected UDP listener from driver PID $pid:"
        echo "       $udp_line"
    done < <(ss -ulnp 2>/dev/null | grep "pid=$pid," || true)

    return "$found_extra"
}

kill_port_squatters() {
    local port
    for port in "$@"; do
        local pids
        pids=$(ss -tlnp 2>/dev/null | awk -v p=":$port" '$0 ~ p {match($0,/pid=([0-9]+)/,a); if(a[1]) print a[1]}' | sort -u)
        for p in $pids; do
            if [ "$p" != "$(get_pid)" ] && [ "$p" != "$(get_addr_pid)" ]; then
                echo "[mud] Killing PID $p which is squatting on port $port..."
                kill "$p" 2>/dev/null
            fi
        done
    done
}

do_start() {
    if is_running; then
        echo "[mud] Already running (PID $(get_pid))."
        return 1
    fi
    echo "[mud] Clearing any processes squatting on port $PORT..."
    kill_port_squatters "$PORT"
    echo "[mud] Starting Nightmare 3 on port $PORT..."
    cd "$BIN_DIR" || { echo "[mud] ERROR: cannot cd to $BIN_DIR"; exit 1; }

    # Start addr_server if not already running
    if [ -z "$(get_addr_pid)" ]; then
        echo "[mud] Starting addr_server on port $ADDR_PORT..."
        nohup "$ADDR_SERVER" "$ADDR_PORT" >> "$LOG" 2>&1 </dev/null &
        local apid=$!
        echo "$apid" > "$ADDR_PIDFILE"
        sleep 1
        if kill -0 "$apid" 2>/dev/null; then
            echo "[mud] addr_server running (PID $apid)."
        else
            echo "[mud] WARNING: addr_server exited immediately. Continuing anyway."
            rm -f "$ADDR_PIDFILE"
        fi
    else
        echo "[mud] addr_server already running (PID $(get_addr_pid))."
    fi

    nohup "$DRIVER" "$CFG" >> "$LOG" 2>&1 </dev/null &
    local pid=$!
    echo "$pid" > "$PIDFILE"
    echo "[mud] Waiting for port $PORT to open (mudlib preload can take 1-3 min)..."
    local i=0
    while [ $i -lt 180 ]; do
        if ! kill -0 "$pid" 2>/dev/null; then
            echo "[mud] ERROR: Driver exited during startup. Check $LOG"
            rm -f "$PIDFILE"
            return 1
        fi
        if ss -tlnp 2>/dev/null | grep -q ":$PORT"; then
            echo "[mud] Started (PID $pid). Listening on port $PORT."
            verify_ports "$pid"
            return 0
        fi
        sleep 1
        ((i++))
    done
    echo "[mud] WARNING: Driver running (PID $pid) but port $PORT not open after 3 min. Check $LOG"
}

do_stop() {
    local pid
    pid=$(get_pid)
    if [ -z "$pid" ]; then
        echo "[mud] Not running."
    else
        echo "[mud] Stopping driver (PID $pid)..."
        kill "$pid"
        local i=0
        while kill -0 "$pid" 2>/dev/null && [ $i -lt 15 ]; do
            sleep 1
            ((i++))
        done
        if kill -0 "$pid" 2>/dev/null; then
            echo "[mud] Still running after 15s - sending SIGKILL."
            kill -9 "$pid"
        fi
        rm -f "$PIDFILE"
        echo "[mud] Stopped."
    fi

    local apid
    apid=$(get_addr_pid)
    if [ -n "$apid" ]; then
        echo "[mud] Stopping addr_server (PID $apid)..."
        kill "$apid" 2>/dev/null
        rm -f "$ADDR_PIDFILE"
    fi
}

do_restart() {
    echo "[mud] Restarting..."
    do_stop
    sleep 1
    do_start
}

do_status() {
    local pid
    pid=$(get_pid)
    if [ -n "$pid" ]; then
        echo "[mud] RUNNING - PID $pid, port $PORT"
        echo "[mud] Uptime: $(ps -o etime= -p "$pid" 2>/dev/null | tr -d ' ')"
        echo "[mud] Connections: $(ss -tnp 2>/dev/null | grep ":$PORT" | grep -c ESTAB || echo "unknown")"
        verify_ports "$pid"
    else
        echo "[mud] STOPPED"
    fi
    local apid
    apid=$(get_addr_pid)
    if [ -n "$apid" ]; then
        echo "[mud] addr_server RUNNING (PID $apid, port $ADDR_PORT)"
    else
        echo "[mud] addr_server STOPPED"
    fi
}

do_log() {
    local lines="${2:-40}"
    echo "[mud] Last $lines lines of $LOG:"
    echo "────────────────────────────────────────"
    tail -n "$lines" "$LOG"
}

do_boot() {
    echo "[mud] Live boot log (Ctrl+C to exit):"
    echo "────────────────────────────────────────"
    tail -f "$LOG"
}

do_doctor() {
    if [ -x "$SCRIPT_DIR/scripts/check-setup.sh" ]; then
        MUD_BASE="$MUD_BASE" "$SCRIPT_DIR/scripts/check-setup.sh"
    else
        echo "[mud] ERROR: scripts/check-setup.sh not found or not executable"
        exit 1
    fi
}

do_init_config() {
    if [ -x "$SCRIPT_DIR/scripts/init-local-config.sh" ]; then
        MUD_BASE="$MUD_BASE" "$SCRIPT_DIR/scripts/init-local-config.sh"
    else
        echo "[mud] ERROR: scripts/init-local-config.sh not found or not executable"
        exit 1
    fi
}

case "$1" in
    start)       do_start ;;
    stop)        do_stop ;;
    restart)     do_restart ;;
    status)      do_status ;;
    log)         do_log "$@" ;;
    boot)        do_boot ;;
    doctor)      do_doctor ;;
    init-config) do_init_config ;;
    *)
        echo "Usage: $0 {start|stop|restart|status|log [lines]|boot|doctor|init-config}"
        echo "  start        - Start the MUD driver"
        echo "  stop         - Gracefully stop the MUD driver"
        echo "  restart      - Stop then start"
        echo "  status       - Show running status, PID, uptime, connections"
        echo "  log [N]      - Print last N lines of runtime log (default 40)"
        echo "  boot         - Tail -f the runtime log (live feed)"
        echo "  doctor       - Verify checkout paths, config, and binaries"
        echo "  init-config  - Create mudos.cfg and groups.cfg from .example templates"
        echo ""
        echo "Environment: MUD_BASE, MUDOS_CFG (optional config file path)"
        exit 1
        ;;
esac
