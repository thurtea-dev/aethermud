# lib/realms/

Per-wizard home directories: `/realms/<name>/` is created automatically
when a player is promoted to staff (makewiz), along with a generated
`workroom.c` the wizard can teleport to with the `workroom` command.

Layout per wizard:

- `workroom.c` - personal workroom (auto-generated on promotion, safe
  to customize).
- `area/` - Quick Creation System output: `room/`, `npc/`, `weap/`,
  `armor/` subdirectories where the `build` command writes generated
  LPC files.
- `adm/` - personal admin/notes space (varies per wizard).

A wizard's shell (`cd`, `ls`, `pwd`) starts in their own realm
directory at login. QCS build verbs only operate while the wizard is
standing inside their own realm.

Current realms: whoever has been promoted on this checkout (e.g.
thurtea, splynncryth). Realm directories of demoted or departed staff
are left in place; content worth keeping should be migrated into a
domain by an admin.

See HOWTO.md for the workflow from empty realm to playable content, and
WHY.md for why realms are separated from domains.
