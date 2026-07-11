// /cmds/mortal/_assassinate.c
// Alias for assassination command.

#include <std.h>

inherit "/cmds/mortal/_assassination";

int cmd_assassinate(string str) {
    return cmd_assassination(str);
}

void help() {
    ::help();
}
