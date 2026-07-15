# nightmare3_fluffos_v2/

The actual running game: the FluffOS 2.9 C driver and the LPC mudlib
built on top of it. This is where all gameplay code lives, as opposed to
the repo root's docs/tooling.

- `fluffos-2.9-ds2.08/` - the FluffOS driver source (vendored C code,
  builds to `bin/driver` and `bin/addr_server`).
- `lib/` - the LPC mudlib itself: this is the actual Rifts game content
  and logic (races, OCCs, spells, rooms, commands, daemons, everything).
- `bin/` - build output and runtime config: the compiled `driver` and
  `addr_server` binaries, plus `mudos.cfg` (local, machine-specific,
  gitignored - generated from `mudos.cfg.example`).
- `win32/` - prebuilt Windows driver binary and batch launcher, kept for
  players/developers on Windows.
- `RiftsMUD Memories.txt` - historical archive of player memories of the
  original game; keep its filename and content as-is (see CLAUDE.md).
- `nm3-faq.html`, `INSTALL` - upstream Nightmare III / install reference
  material carried over from the base lib this project descends from.
