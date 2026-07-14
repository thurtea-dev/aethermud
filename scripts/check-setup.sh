#!/bin/bash
# check-setup.sh - verify checkout is ready to build/run AetherMUD
#
# Usage (from project root):
#   ./scripts/check-setup.sh

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
MUD_BASE="${MUD_BASE:-$ROOT/nightmare3_fluffos_v2}"
BIN_DIR="$MUD_BASE/bin"
LIB_DIR="$MUD_BASE/lib"
FLUFFOS_DIR="$MUD_BASE/fluffos-2.9-ds2.08"
CFG="$BIN_DIR/mudos.cfg"
GROUPS_CFG="$LIB_DIR/secure/cfg/groups.cfg"

errors=0
warnings=0

fail() {
    echo "[FAIL] $1"
    errors=$((errors + 1))
}

warn() {
    echo "[WARN] $1"
    warnings=$((warnings + 1))
}

ok() {
    echo "[ OK ] $1"
}

echo "AetherMUD setup check"
echo "  project root: $ROOT"
echo "  MUD_BASE:     $MUD_BASE"
echo ""

if [ ! -d "$MUD_BASE" ]; then
    fail "MUD_BASE directory missing: $MUD_BASE"
    echo ""
    echo "Result: $errors error(s), $warnings warning(s)"
    exit 1
fi

[ -d "$LIB_DIR" ] && ok "mudlib directory exists" || fail "missing $LIB_DIR"
[ -d "$FLUFFOS_DIR" ] && ok "FluffOS source exists" || fail "missing $FLUFFOS_DIR"
[ -f "$BIN_DIR/mudos.cfg.example" ] && ok "mudos.cfg.example present" || fail "missing mudos.cfg.example"

if [ -f "$CFG" ]; then
    ok "mudos.cfg present"
    if grep -q '@MUDLIB_DIR@\|@BINARY_DIR@\|/full/path/to/' "$CFG" 2>/dev/null; then
        fail "mudos.cfg still has placeholder paths. Run ./scripts/init-local-config.sh"
    fi
    mudlib_line=$(grep '^mudlib directory' "$CFG" | sed 's/^mudlib directory : //')
    bin_line=$(grep '^binary directory' "$CFG" | sed 's/^binary directory : //')
    if [ -n "$mudlib_line" ] && [ ! -d "$mudlib_line" ]; then
        fail "mudos.cfg mudlib directory does not exist: $mudlib_line"
    fi
    if [ -n "$bin_line" ] && [ ! -d "$bin_line" ]; then
        fail "mudos.cfg binary directory does not exist: $bin_line"
    fi
else
    fail "mudos.cfg missing. Run: ./scripts/init-local-config.sh"
fi

if [ -f "$GROUPS_CFG" ]; then
    ok "groups.cfg present"
    if grep -q 'YOUR_ADMIN_NAME' "$GROUPS_CFG" 2>/dev/null; then
        warn "groups.cfg still contains YOUR_ADMIN_NAME placeholder"
    fi
else
    fail "groups.cfg missing. Run: ./scripts/init-local-config.sh"
fi

if [ -x "$BIN_DIR/driver" ]; then
    ok "driver binary exists"
else
    warn "driver binary missing. Build with: cd fluffos-2.9-ds2.08 && ./configure nm3 && make && make install"
fi

if [ -x "$BIN_DIR/addr_server" ]; then
    ok "addr_server binary exists"
else
    warn "addr_server missing (same build step as driver)"
fi

if [ -d "$LIB_DIR/log" ]; then
    if [ -w "$LIB_DIR/log" ]; then
        ok "lib/log is writable"
    else
        fail "lib/log is not writable: $LIB_DIR/log"
    fi
else
    warn "lib/log missing (will be created on first start)"
fi

if [ -f "$ROOT/mud.sh" ] && [ -x "$ROOT/mud.sh" ]; then
    ok "mud.sh is executable"
else
    [ -f "$ROOT/mud.sh" ] && warn "mud.sh exists but is not executable. Run: chmod +x mud.sh"
fi

WS_BRIDGE="$ROOT/scripts/ws-bridge.py"
WS_PYTHON="${WS_PYTHON:-/usr/bin/python3.12}"
if [ -f "$WS_BRIDGE" ]; then
    if [ -x "$WS_PYTHON" ]; then
        ws_py_version=$("$WS_PYTHON" -c 'import sys; print("%d.%d.%d" % sys.version_info[:3])' 2>/dev/null)
        if [ -z "$ws_py_version" ]; then
            fail "ws-bridge interpreter $WS_PYTHON exists but would not run (import/exec failure)"
        elif "$WS_PYTHON" -c 'import sys; sys.exit(0 if sys.version_info >= (3, 7) else 1)' 2>/dev/null; then
            ok "ws-bridge interpreter $WS_PYTHON is Python $ws_py_version"
        else
            fail "ws-bridge interpreter $WS_PYTHON is Python $ws_py_version (need >= 3.7). Set WS_PYTHON to a newer interpreter or install python3.12 at that path."
        fi
    else
        fail "ws-bridge interpreter missing or not executable: $WS_PYTHON (set WS_PYTHON=/path/to/python3.7+ or install python3.12). Browser client will not connect."
    fi
else
    warn "ws-bridge script missing: $WS_BRIDGE (browser client will not connect)"
fi

echo ""
if [ "$errors" -gt 0 ]; then
    echo "Result: $errors error(s), $warnings warning(s) - fix errors before ./mud.sh start"
    exit 1
fi

if [ "$warnings" -gt 0 ]; then
    echo "Result: $errors error(s), $warnings warning(s) - review warnings, then try ./mud.sh start"
    exit 0
fi

echo "Result: ready. Next: ./mud.sh start  then  telnet localhost 1122"
exit 0
