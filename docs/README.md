# docs/

Setup, operations, and planning documentation for AetherMUD. These are the
tracked, maintained docs referenced from the repo-root README.md.

- `INSTALL.md` - first-time setup on a fresh Linux box (clone, build the
  driver, configure, first boot).
- `RUNNING.md` - day-to-day operation: start/stop, ports, logs, reload vs.
  full reboot.
- `DEVELOPMENT.md` - contributor workflow and LPC coding rules (a
  narrower companion to the root `CLAUDE.md`).
- `STYLE.md` - tone and voice for player-facing strings, help text,
  and code comments (no casual slang; NPC dialect exceptions).
- `SHARING.md` - what's local-only config vs. what's tracked in git.
- `PUBLISHING.md` - pre-publication checklist before making the repo or
  server more widely visible.
- `ASSESSMENT.md` - measured state-of-the-project audit (content counts,
  known gaps).
- `zone-expansion-plan.md` - priorities for growing the game world.
- `mercenary-groups-plan.md` - design plan for a player-founded
  mercenary group system with emblem-gated secret bases. Planning only,
  not yet implemented.
- `legacy-riftsmud-reference.md` - design memories from the original
  RiftsMUD (items, Moxim rift travel, falconry pets, UI conventions,
  future feature brainstorm). Reference material, not a task list.
- `starting-equipment-audit.md` - OCC/race starting-gear gap audit
  against `daemon/rifts_start_d.c` and `daemon/occ.c`.
- `lib-cleanup-manifest.md` - dead-file and stale-directory tracking for
  the mudlib tree (what's safe to move/archive, what's still active).
- `admin-thurtea-checklist.md` - thurtea's personal admin/staff-tool
  regression checklist.
- `playtest-checklist.md` - pointer file only; the actual checklists are
  `playtest-checklist-players.md` and `playtest-checklist-wizards.md`,
  with `playtest-roster.md` for expected-value snapshots of rebuilt
  test characters.
- `reference/` - bulk Rifts reference material for developers (see its
  own README.md).
