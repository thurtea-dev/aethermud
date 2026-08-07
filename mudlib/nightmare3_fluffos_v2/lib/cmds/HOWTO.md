# HOWTO: lib/cmds/

## Add a new player command

1. Create `/cmds/mortal/_mycommand.c` (underscore prefix, lowercase):

   ```c
   /* /cmds/mortal/_mycommand.c
      One-line description of what it does. */

   #include <std.h>

   inherit DAEMON;

   int cmd_mycommand(string str) {
       object player;

       player = this_player();
       if(!str || !sizeof(str)) {
           write("Syntax: mycommand <thing>\n");
           return 1;
       }
       write("You mycommand the " + str + ".\n");
       return 1;
   }

   void help() {
       write("Syntax: mycommand <thing>\n\n"
             "What the command does, for the help fallback.\n");
   }
   ```

2. Rules that will bite you if skipped (all from CLAUDE.md):
   - C89: every variable declared at the top of the function block.
   - No `//` comments, only `/* */`.
   - Any helper function called before its definition needs a prototype
     at the top of the file.
   - No em dashes in any player-visible string.
   - American spelling in strings and comments (armor, color).

3. Return conventions: `return 1` means "I handled it". `return 0`
   (often with `notify_fail(...)`) lets the parser try other commands
   with the same verb, and shows the notify_fail text if nothing takes
   it.

4. Reload: for an EDITED file, `update /cmds/mortal/_mycommand.c`
   in-game is enough. For a NEW file, the command daemon needs to
   rehash its paths; a full reboot (`./mud.sh stop && ./mud.sh start`)
   always covers it.

5. Add a help topic: create an extensionless file
   `/doc/help/user/mycommand` matching the format of neighbors (Syntax
   line first, prose after, "See also:" last). Category listings in
   `/daemon/help.c` are separate; `help mycommand` works from the file
   alone.

## Add an admin command

Same pattern in `/cmds/adm/`, but gate the body:

```c
    if(!archp(this_player()) &&
       (string)this_player()->getenv("wiz_role") != "admin") {
        write("Access denied.\n");
        return 1;
    }
```

Log anything that changes another player: `log_file("adm/mycmd", ...)`
(the path is relative to `/log`; never pass a leading `/log/`).

## Useful in-file idioms

- `this_player()` - the acting player object.
- `environment(this_player())` - their room.
- `present("id", room)` - find an object by id in a container.
- `find_player("name")` - online player by lowercase name.
- `message("class", "text", who)` - typed message (classes such as
  my_action, tell, talk get colors/filtering in `std/user.c`).
