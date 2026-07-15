# Install

Fresh-machine setup for AetherMUD on Linux (Fedora, Ubuntu, or WSL2).
For copying an existing live tree to a new PC via HDD, prefer
[install-aethermud.md](../internal/install-aethermud.md). Local config details:
[SHARING.md](SHARING.md).

## Prerequisites

**Debian / Ubuntu / WSL:**

```bash
sudo apt update
sudo apt install -y build-essential git bison flex libssl-dev libmysqlclient-dev \
  libsqlite3-dev libpq-dev libpcre3-dev libevent-dev libjemalloc-dev telnet
```

**Fedora:**

```bash
sudo dnf groupinstall "Development Tools" -y
sudo dnf install gcc gcc-c++ make bison flex openssl-devel pcre-devel \
  libevent-devel jemalloc-devel sqlite-devel libpq-devel \
  mariadb-connector-c-devel telnet -y
```

You need a C compiler, make, bison, flex, and common dev headers.

## 1. Get the project

From a transfer tarball (recommended when moving machines):

```bash
cd ~
tar xzf /path/to/aethermud-full-YYYYMMDD.tar.gz
cd aethermud
chmod +x mud.sh scripts/*.sh
```

Or from git:

```bash
git clone <your-repo-url> aethermud
cd aethermud
chmod +x mud.sh scripts/*.sh
```

All steps below assume the project root (directory containing `mud.sh`).

## 2. Local config (required on first checkout)

```bash
./mud.sh init-config
```

This creates (if missing):

- `nightmare3_fluffos_v2/bin/mudos.cfg` from `mudos.cfg.example`
- `lib/secure/cfg/groups.cfg` from `groups.cfg.example`

You will be prompted for your intended admin login name.

If you extracted a full transfer tarball and your home path is still
`/home/thurtea/aethermud`, existing `mudos.cfg` / `groups.cfg` usually
need no change. Otherwise edit the two absolute paths in `mudos.cfg`, or
delete it and re-run `init-config`.

Manual alternative:

```bash
cp nightmare3_fluffos_v2/bin/mudos.cfg.example nightmare3_fluffos_v2/bin/mudos.cfg
cp nightmare3_fluffos_v2/lib/secure/cfg/groups.cfg.example \
   nightmare3_fluffos_v2/lib/secure/cfg/groups.cfg
# edit both files: absolute paths in mudos.cfg, your name in groups.cfg
```

## 3. Build the FluffOS driver

Always rebuild on the new host (do not reuse a WSL binary):

```bash
cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
./configure nm3
make
make install
cd ../..
```

- `./configure nm3` prepares the NM3 build; it does **not** compile.
- `make install` copies `driver`, `addr_server`, and `portbind` into `nightmare3_fluffos_v2/bin/`.

## 4. Verify

```bash
./mud.sh doctor
```

Fix any `[FAIL]` lines before starting.

## 5. Launch and connect

```bash
./mud.sh start
./mud.sh status
telnet localhost 1122
```

Preload can take 1-3 minutes. Watch boot: `./mud.sh log 50`.

Mudlet: new connection, telnet, host `localhost`, port `1122`. Then `colorize on`.

## 6. First admin

The first account to complete normal registration, while no admin exists
yet on this install, is offered admin rank automatically - no manual
step required.

1. Connect and register a new character as usual (see step 5).
2. After you enter your (optional) real name, you'll see:
   `No admin exists on this MUD yet. Become the admin? (y/n)`
3. Answer `y` to be granted `head arch` rank immediately, before you
   even enter the game world.
4. This only fires once, for whoever registers first. It will not
   appear again once an admin exists.
5. Optional, for promoting someone else later: an existing arch runs
   `makewiz <name>` then `setrole <name> admin` (makewiz creates
   Apprentice; setrole assigns the full role).

`groups.cfg` SECURE/ASSIST membership does **not** grant in-game admin
command access by itself - it only grants raw filesystem read/write
privileges. Every in-game admin check (`archp()`/`creatorp()`, and
every `setrole`/`makewiz`/wizard-tool command) is gated purely by the
`position` field on your save file, which is what the prompt above
sets directly.

## Environment overrides

```bash
export MUD_BASE=/path/to/nightmare3_fluffos_v2
export MUDOS_CFG=/path/to/custom/mudos.cfg
./mud.sh start
```

`mudos.cfg` mudlib and binary paths must still match `MUD_BASE`.

## Common failures

| Symptom | Fix |
|---------|-----|
| `driver` not found | Run `make install` in fluffos directory |
| Port 1122 never opens | `./mud.sh log`; check LPC compile errors |
| Wrong mudlib | Re-run `init-config` or fix paths in `mudos.cfg` |
| Connection refused | Wait for preload; `./mud.sh status` |
| `groups.cfg missing` | `./mud.sh init-config` |

## Rebuild driver when

Any change under `fluffos-2.9-ds2.08/` C source:

```bash
cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
./configure nm3 && make && make install
cd ../..
./mud.sh restart
```

LPC mudlib edits do not require driver rebuild. Use in-game `update` /
`warmboot` for most LPC; use `./mud.sh restart` for `/std/` or new
preload daemons.

## Next

Daily operations: [RUNNING.md](RUNNING.md). Transfer to another PC:
[install-aethermud.md](../internal/install-aethermud.md). Contributor workflow:
[DEVELOPMENT.md](DEVELOPMENT.md).
