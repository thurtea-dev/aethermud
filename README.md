# AetherMUD (a RiftsMUD recreation)

FluffOS 2.9 / Nightmare III mudlib recreation of Palladium Rifts MUD.
Brand is **AetherMUD**; the game system remains Rifts.

| Doc | Purpose |
|-----|---------|
| [RUNNING.md](RUNNING.md) | Start/stop, ports, logs, reload vs reboot |
| [INSTALL.md](INSTALL.md) | First-time setup on a fresh Linux box |
| [install-aethermud.md](install-aethermud.md) | HDD transfer to a new PC (Fedora mini) |
| [report.md](report.md) | Remaining work checklist |
| [staff-handbook/](staff-handbook/index.md) | Internal staff reference |
| [CLAUDE.md](CLAUDE.md) | Coding rules for the lib |
| [zone-expansion-plan.md](zone-expansion-plan.md) | Zone growth priorities |
| [playtest-checklist.md](playtest-checklist.md) | Playtest roster and checks |

## Quick start

```bash
./mud.sh doctor
./mud.sh start
./mud.sh status
telnet localhost 1122
```

Live game: `./mud.sh` on port **1122** (addr_server **1199**).

## Moving to another machine

Use the transfer package under `/home/thurtea/production-aethermud/`
(tarball + checksum + this guide set). Full steps:
[install-aethermud.md](install-aethermud.md).

Rebuild the FluffOS driver on the new host after extract (`configure nm3 &&
make && make install`). Do not rely on a WSL-built `driver` binary.

## Staff notes

- Mortal → Apprentice: `makewiz <player>` (gives apprentice kit: `kit`)
- Apprentice → full role: domain/coding review, then `setrole <player> <role>`
- Bulk LPC reload: in-game `warmboot` (notifies online admins)
- `/std/` or simul-efun changes: `./mud.sh restart`
