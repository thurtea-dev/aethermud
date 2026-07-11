// /cmds/mortal/_retrieve.c
// Retrieve items from an active dimensional pocket.

#include <std.h>

inherit DAEMON;

private int pocket_active(object pl) {
    int until;

    if(!pl) return 0;
    if((string)pl->getenv("has_dim_pocket") != "1") return 0;
    until = (int)pl->query_property("dim_pocket_until");
    if(until > 0 && until < time()) return 0;
    return 1;
}

int cmd_retrieve(string str) {
    object pl;
    object bag;
    object ob;

    pl = this_player();
    if(!pl) return 0;
    if(!pocket_active(pl))
        return notify_fail("You do not have an active dimensional pocket.\n");
    if(!str || !strlen(str))
        return notify_fail("Syntax: retrieve <item>\n");
    bag = pl->query_property("dim_pocket_bag");
    if(!bag || !objectp(bag))
        return notify_fail("Your dimensional pocket is empty.\n");
    ob = present(lower_case(str), bag);
    if(!ob)
        return notify_fail("That is not in your dimensional pocket.\n");
    if(ob->move(pl)) {
        write("You cannot carry that right now.\n");
        return 1;
    }
    write("You retrieve " + (string)ob->query_short() +
        " from the dimensional pocket.\n");
    return 1;
}

void help() {
    write("Syntax: retrieve <item>\n\n"
        "Take an item out of your dimensional pocket.\n");
}
