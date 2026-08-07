/* /cmds/mortal/_right.c
   Designate the right hand as the active hand for dual-wielding or
   directional actions.
   Syntax: right       -- set right hand active
           right hand  -- same */

#include <std.h>

inherit DAEMON;

int cmd_right(string str) {
    string cur;

    cur = (string)this_player()->query_property("active_hand");

    if(!str || !strlen(str) || lower_case(str) == "hand") {
        this_player()->set_property("active_hand", "right");
        if(cur != "right")
            write("You shift your weapon to your right hand.\n");
        else
            write("Your right hand is already active.\n");
        return 1;
    }
    return 0;
}

void help() {
    message("help",
        "Syntax: right\n"
        "        right hand\n\n"
        "Designates your right hand as the active hand.\n"
        "Primarily relevant for dual-wielders choosing which weapon\n"
        "to lead with.\n\n"
        "See also: left, wield, skills",
        this_player());
}
