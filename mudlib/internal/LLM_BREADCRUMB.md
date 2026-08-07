# LLM breadcrumb - internal/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the repo root's `LLM_BREADCRUMB.md` for workspace-wide
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related: `docs/ASSESSMENT.md` and `docs/playtest-checklist.md` are
   the current, authoritative versions of material some of these files
   superseded.

## Gotchas specific to this directory

- Nothing here is guaranteed current. Files here may mark themselves
  superseded (`report.md`) - check for a "SUPERSEDED" banner at the top
  before treating a file's content as fact, and prefer the doc it
  points to.
- A "SUPERSEDED, merged into X" banner is not proof the merge survived.
  `Playtest-list.md` carried one, and its target had since been split
  with the merged section dropped (see `internal/README.md`). Verify
  the content actually exists at the destination before deleting
  anything on the strength of its own banner.
- `tomorrow.md` is a live scratch file the user overwrites between
  sessions - don't treat its content as a stable spec, and don't be
  surprised if it doesn't match the current conversation's actual task.
- These are machine/personal notes, not repo policy. If something here
  conflicts with `CLAUDE.md` or `docs/`, those win.
