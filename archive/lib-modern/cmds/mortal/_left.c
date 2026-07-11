/* /cmds/mortal/_left.c
   Designate the left hand as the active hand for dual-wielding or
   directional actions.
   Syntax: left        -- set left hand active
           left hand   -- same */

#include <std.h>

inherit DAEMON;

int cmd_left(string str) {
    string cur;

    cur = (string)this_player()->query_property("active_hand");

    if(!str || !strlen(str) || lower_case(str) == "hand") {
        this_player()->set_property("active_hand", "left");
        if(cur != "left")
            write("You shift your weapon to your left hand.\n");
        else
            write("Your left hand is already active.\n");
        return 1;
    }
    return 0;
}

void help() {
    message("help",
        "Syntax: left\n"
        "        left hand\n\n"
        "Designates your left hand as the active hand.\n"
        "Primarily relevant for dual-wielders choosing which weapon\n"
        "to lead with.\n\n"
        "See also: right, wield, skills",
        this_player());
}
