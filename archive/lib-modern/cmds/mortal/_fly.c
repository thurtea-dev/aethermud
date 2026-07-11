// /cmds/mortal/_fly.c
// Racial flight toggle (distinct from the Fly spell).

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_fly(string str) {
    object pl;

    pl = this_player();
    if(!pl) return 0;
    if(!(int)RIFTS_D->has_race_flag(pl, "flight")) {
        return notify_fail("You cannot fly.\n");
    }
    if(str && (strsrch(lower_case(str), "off") != -1 ||
       strsrch(lower_case(str), "land") != -1)) {
        if(!(int)pl->query_property("racial_flying")) {
            write("You are not flying.\n");
            return 1;
        }
        pl->remove_property("racial_flying");
        write("You fold your wings and land.\n");
        pl->tell_room_living(environment(pl), pl, 0, " lands.\n");
        return 1;
    }
    if((int)pl->query_property("racial_flying")) {
        write("You are already airborne. Type 'fly off' to land.\n");
        return 1;
    }
    pl->set_property("racial_flying", 1);
    write("You spread your wings and take to the air.\n");
    pl->tell_room_living(environment(pl), pl, 0, " takes flight.\n");
    return 1;
}

void help() {
    write("Syntax: fly [off|land]\n\n"
        "Racial flight for winged beings. Does not move rooms automatically;\n"
        "use normal exits while airborne. Type 'fly off' to land.\n");
}
