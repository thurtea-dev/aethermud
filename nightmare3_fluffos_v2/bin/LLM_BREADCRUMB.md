# LLM breadcrumb - nightmare3_fluffos_v2/bin/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the parent `nightmare3_fluffos_v2/LLM_BREADCRUMB.md` (and the
   repo root's) before assuming this directory's gotchas are
   self-contained.
4. Related: `scripts/init-local-config.sh` and `scripts/check-setup.sh`
   generate/verify `mudos.cfg`; `docs/RUNNING.md` covers day-to-day
   start/stop.

## Gotchas specific to this directory

- `mudos.cfg` is local and gitignored - never assume it's identical
  across machines (dev box vs. VPS), and never hardcode assumptions
  about its absolute paths elsewhere in the codebase.
- `driver` and `addr_server` are build artifacts, not source - to change
  their behavior, edit `fluffos-2.9-ds2.08/` and rebuild; editing a
  binary here directly is never correct.
- Testing a code change against a temporary/isolated copy of the MUD
  (e.g. to verify a `/std/` change without touching real player saves)
  means copying this whole `nightmare3_fluffos_v2/` tree elsewhere,
  generating a fresh `mudos.cfg` pointed at the copy's own paths and a
  different port, and clearing `lib/secure/save/users/` in the copy
  before testing - never test destructive changes against the real
  `bin/mudos.cfg` and its live save data directly.
