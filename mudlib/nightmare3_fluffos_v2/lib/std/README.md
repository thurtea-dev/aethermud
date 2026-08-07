# lib/std/

The inheritable object library: the base classes everything in the
game is built from. Rooms inherit `room.c`, players are `user.c`,
NPCs inherit `monster.c`, items inherit `Object.c` and its
specializations.

Key files:

- `user.c` - the player object: room display (exits before NPCs),
  movement (`move_player`, including prowl stealth rolls), message
  coloring (`receive_message`), positions/promotion plumbing.
- `living.c` + `living/` - shared living-being code (stats, skills,
  body, movement messages) for players and NPCs.
- `monster.c` - NPC base. `rifts_npc.c`, `rifts_pet.c`, corpses,
  vendors, radios extend the Rifts layer.
- `room.c` + `room/` - room base.
- `armour.c`, `weapon.c`, `container.c`, `storage.c` - item bases
  (armour.c keeps its British filename and API by rule).
- `user/nmsh.c` - the wizard shell (cwd, aliases, history, prompt).
- `obj/templates/` - QCS generation templates.

The critical operational fact: `inherit` is resolved at compile time,
so editing anything here requires a FULL reboot to affect
already-loaded objects. See HOWTO.md and CLAUDE.md's "UPDATE VS FULL
REBOOT" section.
