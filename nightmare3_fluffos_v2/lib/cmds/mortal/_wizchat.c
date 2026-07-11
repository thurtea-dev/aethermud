// /cmds/mortal/_wizchat.c
// Alias for the wiz channel command.

#include <std.h>

inherit "/cmds/mortal/_wiz";

int cmd_wizchat(string str) {
    return cmd_wiz(str);
}
