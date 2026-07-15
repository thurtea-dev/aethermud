# LLM breadcrumb - scripts/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the repo root's `LLM_BREADCRUMB.md` for workspace-wide
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related docs: `docs/INSTALL.md` (first-time setup walkthrough that calls
   these scripts), `docs/RUNNING.md` (start/stop/doctor).

## Gotchas specific to this directory

- These are **deploy/setup** shell scripts, not game code. Don't confuse
  them with LPC daemons of a similar name.
- Both scripts must stay idempotent: safe to run on every `./mud.sh start`
  or `./mud.sh doctor` with no side effects if setup is already complete.
  Never make them overwrite an existing `mudos.cfg` or `groups.cfg`.
- `check-setup.sh` uses `fail`/`warn`/`ok` helpers with distinct exit-code
  semantics - `fail` sets a nonzero exit and should be used for anything
  that would leave a fresh clone with no working admin path, not just
  something suboptimal.
- The first-admin bootstrap gap that used to live here (a proposed
  `bootstrap-admin.sh` that sed-patched save files) was dropped in favor
  of an LPC-level fix in `std/user.c` and `secure/std/login.c` - the
  first person to register while no admin exists is offered admin rank
  directly at login. Nothing in this directory needs to know about that
  anymore; don't reintroduce a shell-level admin bootstrap script.
- Portability: these run on whatever box `mud.sh` runs on (a Fedora dev
  box locally, a to-be-deployed VPS in production) - avoid bashisms that
  don't exist in older bash, and don't assume GNU-only sed flags without
  checking.
