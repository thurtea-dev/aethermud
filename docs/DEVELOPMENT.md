# Development Guide

Contributors working on the AetherMUD LPC mudlib and zones.

## Read first

1. [CLAUDE.md](../CLAUDE.md) - mandatory rules
2. [RUNNING.md](RUNNING.md) - start/stop and reload matrix
3. [SHARING.md](SHARING.md) - local config and gitignore
4. `nightmare3_fluffos_v2/lib/domains/Praxis/adm/master_gap_report.txt` - feature gaps

## Mudlib architecture

```
lib/
  std/              Core objects (user, living, room, armour) - full reboot after edits
  secure/           Login, master, security configs, admin saves
  daemon/           Game daemons (rifts, combat, spells, psionics, quests)
  cmds/mortal/      Player commands
  cmds/adm/         Admin commands
  domains/          World zones (Praxis hub, chitown, newcamelot, Lazlo, etc.)
  doc/help/         Player and wizard help files (no extension)
  include/          Shared LPC headers
```

### Rifts-specific daemons

| Daemon | Role |
|--------|------|
| `daemon/rifts.c` | Races, MDC/SDC, stat rolls |
| `daemon/occ.c` | OCC definitions |
| `daemon/rifts_start_d.c` | Starting packages (gear, skills, spells) |
| `daemon/rifts_skills.c` | Skill grants and checks |
| `daemon/rifts_spells_d.c` | Spell effects |
| `daemon/rifts_psionics_d.c` | Psionic powers |
| `daemon/rifts_combat.c` | Rifts combat math |
| `daemon/quest_d.c` | Quest registry |
| `daemon/faction_d.c` | Faction standing |

All Rifts daemons must appear in `lib/secure/cfg/preload.cfg`.

## LPC coding rules

- **C89 only:** declare all variables at the top of each function block
- Use `/* */` comments, not `//` (most files use `//` in newer code; match the surrounding file)
- Player saves: `/secure/save/users/<letter>/<name>.o`
- Room display order: description, exits, NPCs, objects (in `std/user.c`)
- Do not put smell/listen text in `set_long()`; use `set_smell()` / `set_listen()` only
- Do not describe spawned NPCs in `set_long()`
- No em dashes in player-facing strings
- American spelling in player-facing text: armor, color, defense

## Edit workflow

1. Edit the `.c` file
2. If not core std: in-game `update /path/to/file` (arch)
3. Check `lib/log/runtime` for compile errors
4. If std/ or preload.cfg: `./mud.sh restart`

## Testing checklist (manual, in-game)

After your change, verify in Mudlet:

- Relevant command still parses
- No errors in runtime log after `update`
- For combat/spell changes: target a test mob in a wizard workroom
- For zone changes: `@goto` the room and check exits, reset spawns

Do not automate login or read player credential saves.

## Content conventions

- Equipment: `lib/domains/Praxis/equipment/` (and subdirs)
- Admin tools: `lib/domains/adm/wiz_tools/`
- Chi-Town: `lib/domains/chitown/`
- Help files: `lib/doc/help/user/<topic>` (no `.hlp` extension)

## Color conventions (match the original game)

- Exits: bold green
- Radio: green prefix
- OOC: red
- Tells: blue
- Score status (thirst quenched, stoned): blue
- Room descriptions: no color

## Psionics compatibility

Powers must work via direct name (`telepathy guard`) and via `psi <power> at <target>`. Do not rename working power commands.

## Staff handbook

See `staff-handbook/` for server operations, gotchas, and reload procedures.

## Gap report

Update `lib/domains/Praxis/adm/master_gap_report.txt` when you close a documented gap (optional but helpful).
