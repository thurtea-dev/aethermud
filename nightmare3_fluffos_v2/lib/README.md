# lib/

The LPC mudlib - this is AetherMUD itself: the Rifts game content, rules,
and logic running on top of the FluffOS driver in
`../fluffos-2.9-ds2.08/`. Almost all gameplay work happens somewhere
under this directory.

Top-level layout (each has its own README.md/LLM_BREADCRUMB.md):

- `std/` - core inherited base classes (user, living, room, armour,
  combat, container, body, etc.) that everything else builds on.
- `secure/` - privileged code: security-critical daemons, the master
  object, save data, config, and the login/registration flow.
- `cmds/` - player-facing commands, organized by access tier (mortal,
  adm, creator, ambassador, etc.).
- `daemon/` - game-system daemons (races, OCCs, skills, spells,
  psionics, combat rules, and more).
- `domains/` - the actual game world: zones/areas, monsters, NPCs,
  equipment, organized per-domain (Praxis, Chi-Town, Splynn, etc.).
- `doc/` - in-mudlib documentation: help files, LPC language reference,
  build guides.
- `realms/` - personal wizard workrooms/realms.
- `obj/`, `estates/`, `ftp/`, `include/`, `news/`, `www/` - smaller
  supporting subsystems (generic objects, player estates, in-game FTP,
  shared headers, MOTD/news content, and driver-served web assets
  respectively).
- `adm/` - miscellaneous admin-tier support files.

Runtime-only directories (gitignored, not documented individually):
`save/`, `log/`, `tmp/`.
