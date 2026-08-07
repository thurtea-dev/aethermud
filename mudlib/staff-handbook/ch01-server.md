# Chapter 01 - Server Operations

Part of the AetherMUD Staff Handbook. See [index.md](index.md) for the full chapter list.

---

## Starting and Stopping the MUD

All server control goes through mud.sh. Never use kill directly on the driver process.

```bash
cd /home/thurtea/aethermud

./mud.sh start
./mud.sh stop
./mud.sh restart
```

`restart` runs stop, waits, then start. That is a full driver reboot.

---

## Warmboot vs Full Reboot

These are not interchangeable. Knowing the difference prevents breaking live sessions.

**Warmboot** (in-game command from Mudlet: `warmboot`):
- Ensures runtime save/log directories exist, reloads daemon/cmd/obj/adm-tool
  trees, rehashes commands, and notifies online admins.
- Does NOT reload core std files on connected players until relog.
- Appropriate for most day-to-day LPC changes.
- Logged to `lib/log/adm/warmboot`.

**Full reboot** (`./mud.sh restart` or `./mud.sh stop` then `./mud.sh start`):
- Required after any edit to any file under /std/, not just the well known
  ones (std/user.c, std/living.c, std/room.c, std/armour.c, std/combat.c).
  This also includes files like std/container.c, std/living/body.c, and
  std/storage.c: anything that rooms, players, or monsters inherit.
- Also required when changes to preload.cfg are made.
- Never assume warmboot is sufficient for std/ files.

Why the whole /std/ tree, not just those five files: `inherit` is resolved
at compile time. An object that inherits a std/ file gets that file's
compiled code baked in at the moment it was compiled. Reloading the
parent blueprint later, via `update` or `warmboot std`, does not change
any object that already inherited it. Every already-loaded room, every
connected player, and every already-spawned NPC keeps running the old
code until a full reboot. If you edited a std/ file and warmbooted, the
old compiled version is still running for anything already in the game.

---

## In-Game Update Command

After editing a daemon, command, or domain file, reload it from Mudlet
using the update command. The admin runs reloads; do not attempt them
yourself unless you have admin-level access and are specifically asked to.

```
update /daemon/rifts_skills
update /cmds/mortal/_score
update /domains/Praxis/areas/splynn/splynn_market
```

Provide the path from the lib root **without** the `.c` extension (per `/doc/help/wiz/update`).

One update per file. Do not batch them in a single line. If a file fails
to compile on update, the error appears in the runtime log immediately.

---

## Log Paths

Runtime errors (compile failures, runtime errors, driver warnings):

```
nightmare3_fluffos_v2/lib/log/runtime
```

(relative to project root, or use `./mud.sh log`)

Admin log (setrole changes):

```
/log/adm/setrole
```

General driver log output goes to the terminal if mud.sh is run in
foreground. For background runs, check the log/runtime file.

After any update command, check log/runtime immediately to confirm the
file compiled cleanly. A missing brace or C89 violation will show there.

---

## preload.cfg

Location: `/secure/cfg/preload.cfg`

This file controls which daemons are loaded at boot. If a new daemon
file is added to /daemon/ but not listed in preload.cfg, it will not
be available when the mud starts. The driver does not auto-discover
daemon files.

After adding a new daemon:
1. Add it to preload.cfg.
2. Do a full reboot (mud.sh stop + start).
3. Warmboot is not sufficient to load a daemon that was not present
   at the previous boot.

---

## Player Save Path

Player saves are stored at:

```
/secure/save/users/<first_letter_of_name>/<playername>.o
```

Example: a player named "Xelos" saves to `/secure/save/users/x/xelos.o`

Do not use /save/players/ - that is the old NM3 path and does not exist
in this codebase.

---

## When to Update vs When to Reboot

| Change type | Action needed |
|-------------|--------------|
| Daemon file (/daemon/) | update <path> |
| Mortal command (/cmds/mortal/) | update <path> |
| Admin command (/cmds/adm/) | update <path> |
| Domain file (/domains/) | update <path> |
| A single room file (/domains/.../room.c) | update <path>, safe for that room only |
| Any file under /std/ (user.c, living.c, room.c, armour.c, combat.c, container.c, storage.c, everything) | Full reboot |
| New daemon added | Edit preload.cfg + full reboot |
| preload.cfg changed | Full reboot |
| Multiple std files in one session | One full reboot covers all |

---

## Modern FluffOS port track (experimental, archived)

A parallel driver build for a future upgrade lives beside the live game.
It does **not** replace port 1122. Moved into `archive/` to keep the project
root clean; not needed for AetherMUD to run on the live 2.9 driver.

| Item | Path |
|------|------|
| Live game | `./mud.sh` on port **1122** |
| Trial modern instance | `archive/mud-modern.sh` (see `archive/bin-modern/fluffos.cfg` for port) |
| Porting mudlib copy | `archive/lib-modern/` |
| Driver source | `archive/fluffos-modern/` (tag v2025.1205.0) |
| Upgrade notes | `archive/driver-upgrade-notes.md` |

Use the modern instance only for porting work. Player-facing changes belong
in `nightmare3_fluffos_v2/lib/` until cutover is approved.
