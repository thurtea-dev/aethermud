# LLM breadcrumb - lib/std/

1. Read `/CLAUDE.md` at the repo root first, especially "UPDATE VS
   FULL REBOOT" and the Known Mistakes list; half of those rules are
   about this directory.
2. Read `README.md` here for the file map.
3. Check the parent (`nightmare3_fluffos_v2/lib/`) and repo-root
   breadcrumbs for wider context.
4. Related docs: `HOWTO.md` / `WHY.md` here; `docs/DEVELOPMENT.md`.

## Gotchas specific to this directory

- ANY edit under /std/ needs a full reboot
  (`./mud.sh stop && ./mud.sh start`) to reach already-loaded rooms,
  connected players, and spawned NPCs. `update` and `warmboot` only
  affect objects compiled afterward. This is not limited to user.c and
  living.c; it includes container.c, body.c, storage.c, everything.
- Functions must be declared before use (FluffOS 2.9). A missing
  prototype here can silently prevent /std/monster from loading for a
  whole boot; the error appears only in `log/errors/<dir>`.
- Do not rename armour.c or its `*_armour_*` functions or the ARMOUR
  macro; do not revert the room display order in
  `user.c describe_current_room()` (description, exits, NPCs, objects).
- `user.c` is huge and load-bearing: prefer surgical edits, keep new
  variables C89-declared at the top of the enclosing function.
