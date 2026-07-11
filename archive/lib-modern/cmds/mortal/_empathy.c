// /cmds/mortal/_empathy.c
// Shortcut: empathy [at <target>]

#include <std.h>

inherit DAEMON;

int cmd_empathy(string str) {
    return (int)"/cmds/mortal/_psi"->do_psi_power("empathy", str);
}
