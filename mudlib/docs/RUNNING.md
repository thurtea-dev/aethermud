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
