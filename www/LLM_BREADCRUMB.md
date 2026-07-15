# LLM breadcrumb - www/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the repo root's `LLM_BREADCRUMB.md` for workspace-wide
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related: `staff-handbook/` covers similar staff-mechanics ground in
   Markdown - the two are not guaranteed to be in sync, cross-check both
   if researching a staff-facing topic.

## Gotchas specific to this directory

- This is plain static HTML, not templated/generated - edits are direct
  file edits, no build step.
- Content here should track the actual mudlib behavior (chargen flow,
  staff commands, OCC/race lists). When you change chargen, staff ranks,
  races, or OCCs in the LPC code, check whether the matching page here
  needs updating too - it will not update itself.
- `chargen.html` in particular was rewritten to match the 2026-07-14
  plain-string chargen input model (see CLAUDE.md's "Recent session work
  (2026-07-14)" section) - don't let it drift back to describing the old
  verb-prefixed/numbered-menu style.
- CLAUDE.md cites `www/index.html` by its repo-root-relative path
  (`www/index.html`) even though CLAUDE.md itself lives at the actual
  repo root - this directory is one level down from where CLAUDE.md's
  own references assume you're standing, so don't mis-resolve the path.
