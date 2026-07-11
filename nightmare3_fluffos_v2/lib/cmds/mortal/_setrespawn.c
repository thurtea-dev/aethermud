/* /cmds/mortal/_setrespawn.c
   Sets the player's respawn point to the current room.
   Only works in rooms with rest_allowed property set. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_setrespawn(string str) {
    object env;
    string fname;

    env = environment(this_player());
    if(!env) {
        write("You are not in a valid room.\n");
        return 1;
    }
    if(!(int)env->query_property("rest_allowed")) {
        write("You cannot set your respawn point here.\n");
        return 1;
    }
    fname = file_name(env);
    this_player()->set_property("respawn_room", fname);
    this_player()->save_player((string)this_player()->query_name());
    write("Respawn point set here.\n");
    return 1;
}

void help() {
    write(
        "Syntax: setrespawn\n\n"
        "Sets your respawn point to the current room.\n"
        "Only works in inns and rest areas (rooms where 'rest' is allowed).\n"
        "When you die, you will wake here instead of the default location.\n"
    );
}
