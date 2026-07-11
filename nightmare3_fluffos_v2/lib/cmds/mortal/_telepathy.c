// /cmds/mortal/_telepathy.c
// Shortcut: telepathy <target> [message]

#include <std.h>

inherit DAEMON;

int cmd_telepathy(string str) {
    return (int)"/cmds/mortal/_psi"->do_telepathy(str);
}