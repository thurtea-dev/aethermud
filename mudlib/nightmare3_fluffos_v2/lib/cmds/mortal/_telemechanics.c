// /cmds/mortal/_telemechanics.c
// Racial telemechanics: interface with machines and vehicles.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_telemechanics(string str) {
    object pl;
    object env;

    pl = this_player();
    if(!pl) return 0;
    if(!(int)RIFTS_D->has_race_flag(pl, "telemechanics"))
        return notify_fail("You do not have telemechanics.\n");
    env = environment(pl);
    if(!env) {
        write("Nothing here to interface with.\n");
        return 1;
    }
    if((int)env->query_property("vehicle_interior") ||
       (int)env->query_property("vehicle_daemon")) {
        write("You mentally link with the vehicle systems.\n"
              "Pilot commands work from the cockpit as normal.\n");
        return 1;
    }
    if((int)env->query_property("vehicle_accessible")) {
        write("You sense heavy machinery nearby. Board a vehicle to interface fully.\n");
        return 1;
    }
    write("No complex machine interface detected here.\n");
    return 1;
}

void help() {
    write("Syntax: telemechanics\n\n"
        "Racial ability for dragons and similar beings to mentally\n"
        "interface with vehicles and machinery.\n");
}
