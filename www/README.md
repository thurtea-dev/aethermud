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
  sync with the actual chargen flow in `domains/Praxis/setter.c` /
  `daemon/chargen_guide.c`).
- `classes.html`, `races.html`, `world.html` - reference pages for Classes,
  races, and the game world.
- `play.html` - how to connect and play.
- `webclient.html` - in-browser terminal client (WebSocket bridge on
  port 1129; HTTPS pages need wss, which is a known infra gap).
- `roadmap.html` - public-facing development roadmap.
- `css/`, `js/` - static assets for these pages.

See HOWTO.md for editing/publishing these pages, and WHY.md for why the
site is static HTML rather than served from the mudlib.
