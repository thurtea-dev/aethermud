# lib/secure/

The trusted core of the mudlib: everything the security model protects.
Code here can touch player saves, accounts, and driver-level hooks.

- `daemon/` - privileged daemons: `master.c` (the driver's master
  object: access control, groups.cfg parsing, error handling),
  `wiztools.c` (staff tool grant/removal), news, finger, network
  services.
- `SimulEfun/` - simulated efuns, one function per file (user_path,
  absolute_path, ...), globally callable from all LPC.
- `cfg/` - configuration: `preload.cfg` (daemons loaded at boot; a
  Rifts daemon missing here will not exist in-game) and `groups.cfg`
  (privilege groups; thurtea is in SECURE and ASSIST).
- `save/users/<letter>/<name>.o` - player save files. THE canonical
  path (never `/save/players/`).
- `cmds/` - privileged commands by rank (`adm/` rid, xmote, godling;
  `creator/` update, clone; `mortal/` tell, reply - historical
  placement).
- `std/`, `include/`, `etc/`, `tmp/` - secure-side bases (login
  object), shared headers (daemons.h, config.h), and scratch space.

Full reboot required after editing files here that are inherited or
under `cmds/` (see CLAUDE.md). See HOWTO.md for common tasks and
WHY.md for the security model rationale.
