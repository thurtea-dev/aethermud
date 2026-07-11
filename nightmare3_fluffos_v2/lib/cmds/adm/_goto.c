/* /cmds/adm/_goto.c
   Teleport wizard to a player's location or a room path. */

#include <std.h>

inherit DAEMON;

int cmd_goto(string str) {
    object target, dest_room;
    string name;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || !strlen(str)) {
        write("Syntax: goto <player>\n"
              "        goto <room_path>\n");
        return 1;
    }

    /* If starts with '/', treat as room path. */
    if(str[0] == '/') {
        dest_room = find_object(str);
        if(!dest_room) {
            dest_room = load_object(str);
        }
        if(!dest_room) {
            write("Could not load room: " + str + "\n");
            return 1;
        }
        this_player()->move_player(str);
        write("You teleport to " + str + ".\n");
        return 1;
    }

    name   = lower_case(str);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    if(!environment(target)) {
        write(capitalize(name) + " has no location.\n");
        return 1;
    }
    this_player()->move_player(base_name(environment(target)));
    write("You teleport to " + capitalize(name) + "'s location.\n");
    return 1;
}

void help() {
    write("Syntax: goto <player>     -- teleport to player's location\n"
          "        goto <room_path>  -- teleport to an absolute room path\n\n"
          "Restricted to wizards.\n");
}
