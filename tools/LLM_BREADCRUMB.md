# LLM breadcrumb - tools/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the repo root's `LLM_BREADCRUMB.md` for workspace-wide
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related: the `verify` skill and `run` skill (project-level agent
   tooling) - these scripts serve the same purpose (prove a change works
   against the real running game) but are hand-written and predate any
   automated skill wiring.

## Gotchas specific to this directory

- Every script here requires a **live driver already running** on port
  1122 (or whatever port the target instance uses) - they are telnet
  clients, not standalone tests. Start the MUD first.
- Character names created by these scripts must be alphabetic only
  (matches `BANISH_D->valid_name()`'s rules in the actual mudlib).
- `playtest_create_chars.py` explicitly does not touch existing
  accounts - preserve that guarantee if extending it. Never point one of
  these scripts at a production instance with real player data unless
  you are certain of what it does end to end.
- `section8_verify*.py` scripts are numbered passes, each covering
  different regressions/features - don't assume `section8_verify4.py`
  supersedes the earlier ones; read each docstring to see what it
  actually exercises before assuming overlap.
- `__pycache__/` regenerates automatically; never hand-edit or commit
  meaningful content there.
