# LLM breadcrumb - docs/reference/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` in this directory (above).
3. Check the parent `docs/LLM_BREADCRUMB.md` (and the repo root's) for
   conventions before assuming this directory's gotchas are
   self-contained.
4. Related: CLAUDE.md's "Reference files" section lists which files to
   read before making content decisions, and explicitly calls out two
   filenames that no longer exist in the repo - don't cite them.

## Gotchas specific to this directory

- This is lookup/reference material, not setup instructions - if you're
  looking for how to install or run the MUD, go to `docs/` instead.
- `reference-chart.md` is explicitly called out in CLAUDE.md as one of
  the only two remaining sources for the OCC target list (the other is
  `daemon/occ.c` itself). Treat edits here as content-authoritative,
  not casual.
- CLAUDE.md marks `RiftsMUD-AetherMUD Helpfiles.txt` and
  `RiftsMUD-AetherMUD RCC's OCC's List.txt` as GONE - do not cite them
  even if you find references to them elsewhere (old commit messages,
  stale comments); they no longer exist in this repo.
