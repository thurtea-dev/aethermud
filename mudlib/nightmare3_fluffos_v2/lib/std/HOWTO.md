# HOWTO: lib/std/

## Change base-object behavior safely

1. Find the right layer. Player-only behavior goes in `user.c`;
   anything shared with NPCs goes in `living.c` or `living/*.c`;
   NPC-only in `monster.c`. Grep for the function name first: many
   "user" behaviors are actually implemented in an inherited file and
   only prototyped in user.c.
2. Make the edit C89-clean: new variables at the top of the function,
   `/* */` comments, prototypes for any function used before its
   definition (see the prototype blocks at the top of user.c and
   monster.c).
3. Full reboot: `./mud.sh stop && ./mud.sh start` from the shell.
   There is no shortcut; `warmboot std` itself warns that connected
   players keep old inherited code until relog.
4. After boot, check `log/runtime` AND `log/errors/<dir>`; a compile
   error in an inherited file surfaces as mysteriously missing objects
   (no NPCs anywhere), not as a visible error message.

## Add a new inheritable

Create the file here, give it a `create()` calling `::create()` if it
inherits another base, and add a define for it in the std header if
callers should use a macro path. New inheritables only take effect for
objects compiled after the reboot, which for a new file is everything.

## Add a property vs. a stat vs. an env

- `set_property()/query_property()` - per-object runtime state, saved
  with the player; use for flags like `is_sneaking`.
- `set_stats()/query_stats()` - numeric character sheet values
  (IQ..Spd, MDC, PPE, ISP, rifts_hp).
- `setenv()/getenv()` - string-valued player settings that show in
  `setenv` listings (TITLE, wiz_role, rifts_occ). Wizard tooling
  reads/writes these; keep values strings.

## Message classes

`user.c receive_message()` is the single choke point for coloring and
blocking typed messages (tell = bold blue, room_exits = bold green,
radio = green, ooc = red). To color a new category, add a case there
and send with `message("<class>", ...)`; the class name is also what
players block with the block command.
