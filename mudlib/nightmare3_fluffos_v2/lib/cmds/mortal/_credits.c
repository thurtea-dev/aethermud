// /cmds/mortal/_credits.c
// Show current credits balance.

#include <std.h>

inherit DAEMON;

int cmd_credits(string str) {
    int bal;

    bal = (int)this_player()->query_money("credits");
    write("Credits: "+bal+"\n");
    return 1;
}
