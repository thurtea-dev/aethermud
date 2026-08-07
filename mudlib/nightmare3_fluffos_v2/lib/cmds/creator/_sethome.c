/* /cmds/creator/_sethome.c
   Sets the wizard's home room to their current location.
   The 'workroom' command will use this value instead of the default path. */

#include <std.h>

inherit DAEMON;

int cmd_sethome(string str) {
    object env;
    string fname;

    if(!creatorp(this_player())) {
        write("You do not have a home room to set.\n");
        return 1;
    }
    env = environment(this_player());
    if(!env) {
        write("You are not in a valid room.\n");
        return 1;
    }
    fname = file_name(env);
    if(strsrch(fname, "#") != -1)
        fname = fname[0..strsrch(fname, "#") - 1];
    this_player()->set_property("home_room", fname);
    this_player()->save_player((string)this_player()->query_name());
    write("Home room set to " + fname + ".\n");
    write("Type 'workroom' to return here from anywhere.\n");
    return 1;
}

void help() {
    write(
        "Syntax: sethome\n\n"
        "Sets your home room to your current location.\n"
        "After setting it, 'workroom' will teleport you here instead of\n"
        "the default workroom path.\n"
        "Wizard command only.\n"
    );
}
