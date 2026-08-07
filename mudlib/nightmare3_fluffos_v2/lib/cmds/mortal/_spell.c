/* /cmds/mortal/_spell.c
   Alias for cast command: spell <name> [at <target>] */

#include <std.h>
#include <daemons.h>

inherit "/cmds/mortal/_cast";

int cmd_spell(string str) {
    return cmd_cast(str);
}

void help() {
    write("Syntax: spell <spell name> [at <target>]\n\n"
          "Alias for 'cast'.  See: help cast\n");
}
