// /cmds/mortal/_workroom.c
// Teleports a wizard to their personal workroom.

#include <std.h>

inherit DAEMON;

int cmd_workroom(string str) {
    string pname;
    string wroom;

    if(!creatorp(this_player())) {
        write("You do not have a workroom.\n");
        return 1;
    }
    pname = (string)this_player()->query_name();
    wroom = (string)this_player()->query_property("home_room");
    if(!wroom || file_size(wroom + ".c") <= 0) {
        wroom = "/realms/" + pname + "/workroom";
        if(file_size(wroom + ".c") <= 0) {
            wroom = "/domains/wizards/" + pname + "/workroom";
            if(file_size(wroom + ".c") <= 0) {
                write("Your workroom has not been set up yet.  Contact an arch wizard.\n");
                return 1;
            }
        }
    }
    write("You vanish in a shimmer of magical energy.\n");
    say((string)this_player()->query_cap_name() + " vanishes in a shimmer of magical energy.");
    this_player()->move_player(wroom);
    return 1;
}

void help() {
    write(
        "Syntax: workroom\n\n"
        "Teleports you to your personal workroom instantly.\n"
        "Wizard command only.\n"
    );
}
