# LLM breadcrumb

Check the repo root `CLAUDE.md` first, then this directory's
`README.md`, then the parent directory's `LLM_BREADCRUMB.md`, then
`docs/lib-cleanup-manifest.md` for the full move manifest.

Gotchas specific to this directory:

- Everything in here is INTENTIONALLY dead. Do not "fix" references
  to point here, do not add these paths to any config, and do not
  compile these files. They are outside the driver's `lib/` root and
  must stay unreachable.
- Paths mirror their old `lib/`-relative locations. Restoring a file
  means moving it back to the same relative path under `lib/`, then
  re-checking the references listed in the manifest.
- `domains/Praxis/standardOld/` was throwing runtime errors on reset
  before the move (see docs/ASSESSMENT.md section 7). Restoring it
  restores the error spam.
