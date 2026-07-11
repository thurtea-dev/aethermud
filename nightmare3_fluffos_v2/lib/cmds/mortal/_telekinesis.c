// /cmds/mortal/_telekinesis.c
// Shortcut: telekinesis [at <target>]

#include <std.h>

inherit DAEMON;

int cmd_telekinesis(string str) {
    return (int)"/cmds/mortal/_psi"->do_psi_power("telekinesis", str);
}
