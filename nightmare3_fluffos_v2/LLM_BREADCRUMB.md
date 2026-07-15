# LLM breadcrumb - nightmare3_fluffos_v2/

1. Read `/CLAUDE.md` at the repo root first - it governs everything
   under this directory and is written specifically for this mudlib
   (do not treat this as a standard/generic NM3 lib).
2. Read `README.md` in this directory (above).
3. Check the repo root's `LLM_BREADCRUMB.md` for workspace-wide
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related: `docs/INSTALL.md` and `docs/RUNNING.md` for build/run
   instructions; CLAUDE.md's "Running the MUD" and "UPDATE VS FULL
   REBOOT" sections for the update/reload model.

## Gotchas specific to this directory

- `fluffos-2.9-ds2.08/` is vendored third-party driver source. It has
  its own README.md/LLM_BREADCRUMB.md only at its root (per an earlier
  scoping decision) - its internal subdirectories (testsuite/, packages/,
  compat/, etc.) do not get the workspace-wide doc treatment, since
  that's upstream C code, not AetherMUD content.
- `lib/` is where essentially all actual work happens - see its own
  README.md/LLM_BREADCRUMB.md for the full breakdown.
- Never use `kill` directly on the driver process - always
  `./mud.sh stop`/`start`/`restart` from the repo root (see CLAUDE.md).
- Editing anything under `/std/` (not just user.c/living.c/room.c/
  armour.c/combat.c) or `/secure/cmds/` requires a full
  `./mud.sh stop && ./mud.sh start`, not just `update` or `warmboot` -
  see CLAUDE.md's "UPDATE VS FULL REBOOT" section for why.
