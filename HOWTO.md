# HOWTO: AetherMUD repo root

Practical entry points for working in this checkout without guessing paths.

## Run the MUD locally

1. First-time setup: follow [docs/INSTALL.md](docs/INSTALL.md).
2. Day-to-day:

   ```bash
   ./mud.sh doctor    # config and binary sanity check
   ./mud.sh start
   # connect: Mudlet or telnet to localhost:1122
   ./mud.sh stop      # never kill the driver by hand
   ```

3. After editing LPC under `nightmare3_fluffos_v2/lib/`:
   - Daemons / cmds / domain rooms: in-game `update /path/to/file.c`
   - Anything under `/std/` or `secure/cmds/`: full reboot
     (`./mud.sh stop && ./mud.sh start`). Warmboot is not enough.

## Where to put work

| You want to... | Go here |
|---|---|
| Add a player command | `nightmare3_fluffos_v2/lib/cmds/mortal/` (see that dir's HOWTO.md) |
| Add a room / NPC / zone content | `nightmare3_fluffos_v2/lib/domains/<zone>/` |
| Change core player/room/combat inheritables | `nightmare3_fluffos_v2/lib/std/` (full reboot) |
| Change login, master, security | `nightmare3_fluffos_v2/lib/secure/` |
| Build as a coding wizard (QCS) | `/realms/<you>/` in-game; docs in `lib/realms/HOWTO.md` |
| Staff mechanics reference | `staff-handbook/` |
| Public / offline HTML guides | `www/` |
| Setup, ops, planning docs | `docs/` |

## Rules that apply everywhere

Read [CLAUDE.md](CLAUDE.md) before changing the mudlib. The non-negotiables
that bite most often:

- C89 locals (declare at top of each function).
- No em dashes in player-facing text or repo docs.
- Spawns in `reset()`, never `create()`.
- Do not describe spawned NPCs in `set_long()`.
- Player saves live under `/secure/save/users/<letter>/<name>.o`.
- You do not live-test the MUD; the human does.

## Staff promotion / demotion (quick)

- Mortal to Apprentice: `makewiz <player>`
- Apprentice to role: `setrole <player> <admin|coding|domain|rp>`
- Demote wizard to mortal: Staff of Demotion menu option 2 (`demote`)
- Delete a player file: `rid <name>` (arch + LAW; not the same as demote)

## Docs map

- Per-directory `README.md` = what is here
- Per-directory `HOWTO.md` = how to make a thing here
- Per-directory `WHY.md` = why it is shaped this way
- Per-directory `LLM_BREADCRUMB.md` = navigation for assistants
