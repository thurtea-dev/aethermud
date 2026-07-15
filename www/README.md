# www/

The AetherMUD website: offline staff guides and public-facing game
information, served as static HTML. Not part of the running MUD server -
these are standalone pages, not LPC.

- `index.html` - main page; also referenced from CLAUDE.md as the
  canonical entry point for offline staff guides (QCS, coding, domain,
  roleplay, admin, chargen).
- `admin.html`, `apprentice.html`, `coding.html`, `domain.html`,
  `qcs.html`, `roleplay.html` - staff role guides.
- `chargen.html` - player-facing character generation guide (kept in
  sync with the actual chargen flow in `secure/std/setter.c` /
  `daemon/chargen_guide.c`).
- `occs.html`, `races.html`, `world.html` - reference pages for OCCs,
  races, and the game world.
- `play.html` - how to connect and play.
- `roadmap.html` - public-facing development roadmap.
- `css/`, `js/` - static assets for these pages.
