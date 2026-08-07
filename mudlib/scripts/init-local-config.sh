#!/bin/bash
# init-local-config.sh - create local mudos.cfg and groups.cfg from templates
#
# Usage (from project root):
#   ./scripts/init-local-config.sh
#   ADMIN_NAME=mylogin ./scripts/init-local-config.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
MUD_BASE="${MUD_BASE:-$ROOT/nightmare3_fluffos_v2}"
BIN_DIR="$MUD_BASE/bin"
LIB_DIR="$MUD_BASE/lib"
CFG_DIR="$LIB_DIR/secure/cfg"
ADMIN_NAME="${ADMIN_NAME:-}"

created=0

if [ ! -d "$MUD_BASE" ]; then
    echo "ERROR: MUD_BASE not found: $MUD_BASE"
    exit 1
fi

if [ ! -f "$BIN_DIR/mudos.cfg.example" ]; then
    echo "ERROR: missing $BIN_DIR/mudos.cfg.example"
    exit 1
fi

if [ -f "$BIN_DIR/mudos.cfg" ]; then
    echo "[init] mudos.cfg already exists: $BIN_DIR/mudos.cfg (unchanged)"
else
    sed "s|@MUDLIB_DIR@|$LIB_DIR|g; s|@BINARY_DIR@|$BIN_DIR|g" \
        "$BIN_DIR/mudos.cfg.example" > "$BIN_DIR/mudos.cfg"
    echo "[init] Created $BIN_DIR/mudos.cfg"
    created=1
fi

if [ ! -f "$CFG_DIR/groups.cfg.example" ]; then
    echo "ERROR: missing $CFG_DIR/groups.cfg.example"
    exit 1
fi

if [ -f "$CFG_DIR/groups.cfg" ]; then
    echo "[init] groups.cfg already exists: $CFG_DIR/groups.cfg (unchanged)"
else
    if [ -z "$ADMIN_NAME" ]; then
        echo -n "[init] Enter your intended admin login name for groups.cfg: "
        read -r ADMIN_NAME
    fi
    if [ -z "$ADMIN_NAME" ]; then
        ADMIN_NAME="YOUR_ADMIN_NAME"
        echo "[init] WARNING: using placeholder YOUR_ADMIN_NAME in groups.cfg"
        echo "[init]          edit $CFG_DIR/groups.cfg before first boot"
    fi
    sed "s/YOUR_ADMIN_NAME/$ADMIN_NAME/g" \
        "$CFG_DIR/groups.cfg.example" > "$CFG_DIR/groups.cfg"
    echo "[init] Created $CFG_DIR/groups.cfg"
    created=1
fi

mkdir -p "$LIB_DIR/log" "$LIB_DIR/tmp" "$LIB_DIR/secure/save/users"
touch "$LIB_DIR/log/.gitkeep" 2>/dev/null || true
touch "$LIB_DIR/tmp/.gitkeep" 2>/dev/null || true

if [ "$created" -eq 1 ]; then
    echo "[init] Done. Run ./scripts/check-setup.sh then ./mud.sh start"
else
    echo "[init] Nothing to create. Run ./scripts/check-setup.sh to verify."
fi
