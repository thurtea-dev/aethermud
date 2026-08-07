/* /cmds/mortal/_position.c
    Sets a player's visible position string in room descriptions.
    RP flavor command. Combat stance is handled by _stance.c. */

#include <std.h>

inherit DAEMON;

int cmd_position(string str) {
    string trimmed;

    if(!str || str == "") {
        this_player()->set_property("position_str", 0);
        message("info", "You resume your usual stance.\n",
                this_player());
        return 1;
    }
    trimmed = str;
    if(strlen(trimmed) > 60) {
        message("info",
            "Position string too long (max 60 characters).\n",
            this_player());
        return 1;
    }
    this_player()->set_property("position_str", trimmed);
    message("info", "You are now " + trimmed + ".\n",
            this_player());
    return 1;
}

void init() {
    add_action("cmd_position", "position");
}