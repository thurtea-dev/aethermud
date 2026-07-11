/* /cmds/adm/_force.c
   Force an online player to execute a command. Restricted to archp/admin. */

#include <std.h>

inherit DAEMON;

int cmd_force(string str) {
    string name, cmd;
    object target;

    if(!archp(this_player()) &&
       (string)this_player()->getenv("wiz_role") != "admin") {
        write("Access denied.\n");
        return 1;
    }
    if(!str || sscanf(str, "%s %s", name, cmd) != 2) {
        write("Syntax: force <player> <command>\n");
        return 1;
    }
    name   = lower_case(name);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    if(creatorp(target) && !archp(this_player())) {
        write("You cannot force another wizard.\n");
        return 1;
    }
    target->force_me(cmd);
    write("You force " + capitalize(name) + " to: " + cmd + "\n");
    log_file("/log/adm/force",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " forced " + name + " to: " + cmd + "\n");
    return 1;
}

void help() {
    write("Syntax: force <player> <command>\n\n"
          "Force an online player to execute a command.\n"
          "All uses are logged. Restricted to admin-level wizards.\n");
}
