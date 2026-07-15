# LLM breadcrumb - repo root

1. Read `/CLAUDE.md` first. It is the mandatory rules file for this repo
   and overrides default behavior.
2. Read `README.md` in this directory - the project overview (what
   AetherMUD is, quick start, repo layout, doc index).
3. This is the top of the tree - there is no parent breadcrumb to check.
4. Related docs: everything under `docs/` (start with `docs/INSTALL.md`
   and `docs/RUNNING.md`), `staff-handbook/` for in-game staff mechanics,
   `internal/` for personal/machine-specific working notes (not
   authoritative project docs).

## Gotchas specific to this directory

- Only `README.md` and `CLAUDE.md` belong at the repo root as tracked
  docs (see CLAUDE.md's "Repo doc layout" section). Don't add new
  top-level `.md` files here - put setup/ops/planning docs under
  `docs/`, bulk reference material under `docs/reference/`, and
  personal notes under `internal/`.
- `zip-files/` exists on disk but is gitignored (reference archives,
  e.g. a Dead Souls mudlib snapshot used for comparison research) - it
  is never pushed and should not be treated as part of the tracked repo.
- The actual game code lives two levels down, in
  `nightmare3_fluffos_v2/lib/` (LPC mudlib) and
  `nightmare3_fluffos_v2/fluffos-2.9-ds2.08/` (C driver source). Don't
  assume repo-root files are where gameplay logic lives.
- Every directory in this workspace (except
  `nightmare3_fluffos_v2/fluffos-2.9-ds2.08/`'s internal subdirectories,
  which are vendored FluffOS driver source) carries this same
  README.md + LLM_BREADCRUMB.md pair. If you're working several levels
  deep, the nearest one up the tree is usually more specific than this
  one.
