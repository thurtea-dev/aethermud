// /cmds/mortal/_store.c
// Store items in an active dimensional pocket.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private int pocket_active(object pl) {
    int until;

    if(!pl) return 0;
    if((string)pl->getenv("has_dim_pocket") != "1") return 0;
    until = (int)pl->query_property("dim_pocket_until");
    if(until > 0 && until < time()) return 0;
    return 1;
}

private object query_pocket_bag(object pl) {
    object bag;

    bag = pl->query_property("dim_pocket_bag");
    if(bag && objectp(bag)) return bag;
    bag = new("/std/obj/dim_pocket_bag");
    if(!bag) return 0;
    bag->move(pl);
    pl->set_property("dim_pocket_bag", bag);
    return bag;
}

int cmd_store(string str) {
    object pl;
    object ob;
    object bag;

    pl = this_player();
    if(!pl) return 0;
    if(!pocket_active(pl))
        return notify_fail("You do not have an active dimensional pocket.\n");
    if(!str || !strlen(str))
        return notify_fail("Syntax: store <item>\n");
    ob = present(lower_case(str), pl);
    if(!ob)
        return notify_fail("You are not carrying that.\n");
    if((int)ob->query_property("dim_pocket_bag"))
        return notify_fail("You cannot store the pocket in itself.\n");
    if((int)ob->query_property("no_drop") || (int)ob->query_property("no_store"))
        return notify_fail("You cannot store that.\n");
    bag = query_pocket_bag(pl);
    if(!bag)
        return notify_fail("The pocket fails to open.\n");
    if(ob->move(bag)) {
        write("It will not fit in the pocket.\n");
        return 1;
    }
    write("You tuck " + (string)ob->query_short() + " into the dimensional pocket.\n");
    return 1;
}

void help() {
    write("Syntax: store <item>\n\n"
        "Place an item into your dimensional pocket (requires active pocket spell).\n");
}
