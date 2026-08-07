# LLM breadcrumb - lib/domains/

1. Read `/CLAUDE.md` at the repo root first. Several of its "Known
   Mistakes" rules exist specifically because of files in this tree.
2. Read `README.md` in this directory for the zone map.
3. Check the parent (`nightmare3_fluffos_v2/lib/`) and repo-root
   breadcrumbs if you need wider context.
4. Related docs: `HOWTO.md` / `WHY.md` here, `staff-handbook/`
   ch16-builder, `www/domain.html`, and
   `/domains/Praxis/adm/master_gap_report.txt` for what is missing.

## Gotchas specific to this directory

- Room spawns belong in `reset()` guarded by `present()`, never in
  `create()` (CLAUDE.md rule 11). Never describe spawned NPCs/objects
  in `set_long()` (rule 8).
- `set_smell()` / `set_listen()` text must not be echoed by the room
  display (rule 3). No ambient message calls in rooms (rule 7).
- Legacy paths are load-bearing: `equipment/hover_cycle.c` and several
  magic items live at the equipment root and must not be moved; rooms
  reference those exact paths.
- `Praxis/setter.c` is chargen, not a room, despite living here.
- Single room files are update-safe in-game (`update <path>` moves
  players to the void and back). NPCs already spawned keep old code
  until they respawn.
