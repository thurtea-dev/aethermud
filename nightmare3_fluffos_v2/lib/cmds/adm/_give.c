/* /cmds/adm/_give.c
   Clone an object by path and give it to an online player. */

#include <std.h>

inherit DAEMON;

int cmd_give(string str) {
    string name, path;
    object target, ob;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || sscanf(str, "%s to %s", path, name) != 2) {
        write("Syntax: give <object_path> to <player>\n"
              "Example: give /domains/Praxis/equipment/e_clip.c to bob\n");
        return 1;
    }
    path   = lower_case(path);
    name   = lower_case(name);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    ob = clone_object(path);
    if(!ob) {
        write("Could not clone: " + path + "\n");
        return 1;
    }
    if(ob->move(target)) {
        write("Could not give item to " + capitalize(name) + " (inventory full?).\n");
        ob->destruct();
        return 1;
    }
    write("Gave " + (string)ob->query_short() + " to " + capitalize(name) + ".\n");
    tell_object(target,
        "%^YELLOW%^" + (string)this_player()->query_cap_name() +
        " gives you " + (string)ob->query_short() + ".%^RESET%^\n");
    return 1;
}

void help() {
    write("Syntax: give <object_path> to <player>\n\n"
          "Clone an object by its full path and give it to an online player.\n"
          "Example: give /domains/Praxis/equipment/e_clip.c to bob\n"
          "Restricted to wizards.\n");
}
