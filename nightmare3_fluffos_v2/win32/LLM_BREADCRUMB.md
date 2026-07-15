# LLM breadcrumb - win32/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the parent `nightmare3_fluffos_v2/LLM_BREADCRUMB.md` (and the
   repo root's) before assuming this directory's gotchas are
   self-contained.
4. Related: `../bin/mudos.cfg.win32` is the matching Windows config
   template; `../fluffos-2.9-ds2.08/` is where `driver.exe` would be
   rebuilt from if it ever needs updating.

## Gotchas specific to this directory

- `driver.exe` is a prebuilt binary, not source - the project's primary
  development and deployment target is Linux (this Fedora dev box
  locally, a Linux VPS in production per CLAUDE.md). Don't assume
  Windows parity for new features unless explicitly asked; this
  directory may lag behind the Linux build.
- If Linux-side driver behavior changes in a way that would affect
  Windows players, this binary needs a separate rebuild - editing LPC
  under `../lib/` does not touch `driver.exe` at all (the LPC mudlib is
  driver-agnostic; only the C driver itself is platform-specific).
