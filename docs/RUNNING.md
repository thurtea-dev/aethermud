# Running AetherMUD

Day-to-day operations. Setup: [INSTALL.md](INSTALL.md). Local config: [SHARING.md](SHARING.md).

## Lifecycle

From project root:

```bash
./mud.sh start
./mud.sh stop
./mud.sh restart
./mud.sh status
./mud.sh log          # last 40 lines
./mud.sh log 100
./mud.sh boot         # tail -f runtime log
./mud.sh doctor       # verify config and binaries
./mud.sh init-config  # create local config from templates
```

Never `kill` the driver directly. Use `./mud.sh stop`.

## Connect

| Client | Host | Port |
|--------|------|------|
| telnet | localhost | 1122 |
| Mudlet | localhost | 1122 (telnet) |

In-game: `colorize on` (requires `TERM ansi`).

## Config files

| File | Purpose |
|------|---------|
| `nightmare3_fluffos_v2/bin/mudos.cfg` | Driver paths and ports (local, from `.example`) |
| `lib/secure/cfg/groups.cfg` | Admin group membership (local, from `.example`) |
| `lib/secure/cfg/preload.cfg` | Daemon boot list |

After editing `groups.cfg` or `mudos.cfg`: `./mud.sh restart`.

## Ports

| Service | Port |
|---------|------|
| Player telnet | 1122 |
| addr_server | 1199 |

HTTP and MCP/intermud are disabled in this build.

## Modern driver trial (porting only, archived)

Experimental FluffOS v2025 build for a future upgrade. Does not affect the live game.
Paused and moved out of the project root to keep it clean; not needed for AetherMUD
to run on the live 2.9 driver.

```bash
cd archive/
./mud-modern.sh start    # telnet port in bin-modern/fluffos.cfg (currently 1124)
./mud-modern.sh stop
./mud-modern.sh status
./mud-modern.sh log
```

Uses `archive/lib-modern/` (porting copy) and `archive/bin-modern/fluffos.cfg`. See
`archive/driver-upgrade-notes.md` and `internal/report.md` section 10.

## Logs

```
nightmare3_fluffos_v2/lib/log/runtime
nightmare3_fluffos_v2/lib/log/adm/setrole
```

## Reload vs reboot

| Change | Action |
|--------|--------|
| Daemon, command, domain LPC | `update /path/to/file` (no `.c` suffix) |
| Bulk reload | `warmboot` (arch only; notifies online admins) |
| One domain | `warmboot tolkeen` (or Praxis, Horton, …) |
| Wiz tools under `/domains/adm` | included in plain `warmboot` |
| Any file under `/std/` | `./mud.sh restart` |
| `preload.cfg` or new daemon | `./mud.sh restart` |
| FluffOS C source | rebuild driver + `./mud.sh restart` |

Warmboot ensures runtime save/log directories (including apprentice queues),
rehashes commands, and logs to `lib/log/adm/warmboot`. It does not reload
std inheritance on connected players until relog. There is no in-game
warmboot via a `reboot` command.

## Player saves

```
lib/secure/save/users/<first_letter>/<name>.o
```

Not `/save/players/` (legacy NM3).

## Environment

```bash
export MUD_BASE=/custom/path/to/nightmare3_fluffos_v2
export MUDOS_CFG=/custom/path/mudos.cfg
./mud.sh start
```

## Scheduled shutdown (in-game, arch)

`setreboot`, `nextreboot`, `end [minutes]`, `shutdown <reason>`
