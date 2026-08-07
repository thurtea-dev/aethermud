// /cmds/mortal/_take.c
// Alias for get - players expect both "take" and "get" to work.

#include <std.h>

inherit "/cmds/mortal/_get";

int cmd_take(string str) {
    return cmd_get(str);
}
