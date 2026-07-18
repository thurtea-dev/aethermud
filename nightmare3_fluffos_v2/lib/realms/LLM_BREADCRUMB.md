# LLM breadcrumb - lib/realms/

1. Read `/CLAUDE.md` at the repo root first.
2. Read `README.md` here for the per-wizard layout.
3. Check the parent (`nightmare3_fluffos_v2/lib/`) and repo-root
   breadcrumbs for wider context.
4. Related docs: `HOWTO.md` / `WHY.md` here; `help build` in-game
   (source: `/doc/help/creator/build`); `www/coding.html`.

## Gotchas specific to this directory

- Everything here is generated or personal. Do not assume realm files
  follow current lib conventions; QCS templates evolve and old
  generated files may predate fixes.
- The directory pair `/realms/<name>` + workroom.c is created by
  `wiz_setup_workroom()` in `std/user.c` during set_position, and QCS
  subdirectories by `qcs_ensure_realm_dirs()` in
  `/cmds/creator/_qcs.c`. Do not hand-create realm skeletons; promote
  properly instead.
- Realm content is live game code once loaded: a broken generated file
  here shows up in `log/errors/realms/` like any other compile error.
- Player-facing content should graduate OUT of realms into a domain;
  rooms under /realms/ are not part of the world map and nothing
  should link to them from domain rooms.
