#!/bin/bash
# setup.sh - one-command fresh-machine setup for AetherMUD on Linux/WSL2.
#
# Wraps the manual steps in docs/INSTALL.md: install build dependencies,
# create local config from templates, build the FluffOS driver, then
# verify the checkout with ./mud.sh doctor.
#
# Usage:
#   ./setup.sh              # full setup
#   ./setup.sh --skip-deps  # skip the sudo package-manager step
#   ./setup.sh --skip-build # skip compiling the driver (e.g. rerunning
#                            # after a driver is already built)
#
# Safe to re-run: every step it wraps is already idempotent
# (scripts/init-local-config.sh never overwrites existing config; the
# build step is skipped with --skip-build; check-setup.sh is read-only).

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

SKIP_DEPS=0
SKIP_BUILD=0
for arg in "$@"; do
    case "$arg" in
        --skip-deps) SKIP_DEPS=1 ;;
        --skip-build) SKIP_BUILD=1 ;;
        -h|--help)
            echo "Usage: $0 [--skip-deps] [--skip-build]"
            exit 0
            ;;
        *)
            echo "[setup] Unknown option: $arg" >&2
            exit 1
            ;;
    esac
done

echo "[setup] AetherMUD Linux setup"
echo "[setup] Project root: $SCRIPT_DIR"
echo ""

chmod +x mud.sh scripts/*.sh 2>/dev/null || true

# --- 1. Build dependencies ---------------------------------------------
if [ "$SKIP_DEPS" -eq 1 ]; then
    echo "[setup] --skip-deps set; not installing packages."
elif command -v dnf >/dev/null 2>&1; then
    echo "[setup] Fedora/dnf detected. Installing build dependencies (sudo required)..."
    sudo dnf groupinstall "Development Tools" -y
    sudo dnf install -y gcc gcc-c++ make bison flex openssl-devel pcre-devel \
        libevent-devel jemalloc-devel sqlite-devel libpq-devel \
        mariadb-connector-c-devel telnet
elif command -v apt-get >/dev/null 2>&1; then
    echo "[setup] Debian/Ubuntu/WSL apt detected. Installing build dependencies (sudo required)..."
    sudo apt-get update
    sudo apt-get install -y build-essential git bison flex libssl-dev libmysqlclient-dev \
        libsqlite3-dev libpq-dev libpcre3-dev libevent-dev libjemalloc-dev telnet
else
    echo "[setup] WARNING: no dnf or apt-get found; skipping automatic dependency install."
    echo "[setup]          Install a C toolchain (gcc, make, bison, flex) and dev headers"
    echo "[setup]          yourself, then rerun with --skip-deps."
fi
echo ""

# --- 2. Local config -----------------------------------------------------
echo "[setup] Creating local config (mudos.cfg, groups.cfg) if missing..."
./mud.sh init-config
echo ""

# --- 3. Build the driver ---------------------------------------------------
if [ "$SKIP_BUILD" -eq 1 ]; then
    echo "[setup] --skip-build set; not compiling the driver."
else
    echo "[setup] Building the FluffOS driver..."
    (
        cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
        ./configure nm3
        make
        make install
    )
fi
echo ""

# --- 4. Verify -------------------------------------------------------------
echo "[setup] Verifying checkout..."
set +e
./mud.sh doctor
DOCTOR_STATUS=$?
set -e
echo ""

if [ "$DOCTOR_STATUS" -eq 0 ]; then
    echo "[setup] Done. Start with: ./mud.sh start"
    echo "[setup] Then connect:    telnet localhost 1122"
else
    echo "[setup] Setup finished with warnings/errors above - fix them, then ./mud.sh doctor again."
fi
exit "$DOCTOR_STATUS"
