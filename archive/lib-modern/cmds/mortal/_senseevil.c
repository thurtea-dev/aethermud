// /cmds/mortal/_senseevil.c
// Shortcut: senseevil

#include <std.h>

inherit DAEMON;

int cmd_senseevil(string str) {
    return (int)"/cmds/mortal/_psi"->do_psi_power("sense evil", str);
}
