// /cmds/mortal/_mindblock.c
// Shortcut: mindblock

#include <std.h>

inherit DAEMON;

int cmd_mindblock(string str) {
    return (int)"/cmds/mortal/_psi"->do_psi_power("mind block", str);
}
