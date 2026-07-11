// /cmds/mortal/_objectread.c
// Shortcut: objectread [at <target>]

#include <std.h>

inherit DAEMON;

int cmd_objectread(string str) {
    return (int)"/cmds/mortal/_psi"->do_psi_power("object read", str);
}
