# LLM breadcrumb - docs/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the repo root's `LLM_BREADCRUMB.md` for workspace-wide
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related: `docs/reference/` for bulk lookup material,
   `staff-handbook/` for in-game staff mechanics (a different kind of
   doc - player/staff-facing, not setup/ops).

## Gotchas specific to this directory

- Per CLAUDE.md's "Repo doc layout" section, this is where
  setup/ops/planning docs live (moved here from the repo root on
  2026-07-13). If you're about to create a new top-level `.md` file at
  the repo root, it almost certainly belongs here instead.
- `ASSESSMENT.md` contains measured counts (races, OCCs, spells, etc.)
  that go stale - treat it as a snapshot from its last-updated date, not
  a live source of truth. Prefer grepping the actual daemon files
  (`daemon/rifts.c`, `daemon/occ.c`, etc.) for current counts.
- These docs describe the *current* setup/ops story. If you change how
  setup or admin bootstrap works (e.g. `scripts/init-local-config.sh`,
  `scripts/check-setup.sh`, or the login/registration flow in
  `secure/std/login.c`), update `INSTALL.md` and `RUNNING.md` in the
  same change - they are the docs a fresh clone actually depends on.
