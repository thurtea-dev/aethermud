/* /cmds/adm/_trans.c
   Bring an online player to the wizard's current location. */

#include <std.h>

inherit DAEMON;

int cmd_trans(string str) {
    object target;
    string name, dest;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || !strlen(str)) {
        write("Syntax: trans <player>\n");
        return 1;
    }
    name   = lower_case(str);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    if(!environment(this_player())) {
        write("You are not in a room.\n");
        return 1;
    }
    dest = base_name(environment(this_player()));
    target->move_player(dest);
    write("You transport " + capitalize(name) + " to your location.\n");
    tell_object(target,
        "%^YELLOW%^" + (string)this_player()->query_cap_name() +
        " transports you to their location.%^RESET%^\n");
    return 1;
}

void help() {
    write("Syntax: trans <player>\n\nBring a player to your current location.\n"
          "Restricted to wizards.\n");
}
