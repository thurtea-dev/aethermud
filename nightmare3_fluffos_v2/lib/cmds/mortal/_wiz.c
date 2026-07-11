// /cmds/mortal/_wiz.c
// Wizard-only global channel.
// Usage: wiz <message>

#include <std.h>

inherit DAEMON;

int cmd_wiz(string str) {
    object *all_users;
    string msg;
    int i;

    if(!creatorp(this_player())) {
        write("You are not a wizard.\n");
        return 1;
    }
    if(!str || !strlen(str)) {
        write("Wiz what?\n");
        return 1;
    }
    msg = "%^BOLD%^[WIZ] " + (string)this_player()->query_cap_name() +
          ": %^RESET%^" + str + "\n";
    all_users = users();
    i = sizeof(all_users);
    while(i--) {
        if(!all_users[i]) continue;
        if(creatorp(all_users[i]))
            message("wiz", msg, all_users[i]);
    }
    return 1;
}

void help() {
    write(
        "Syntax: wiz <message>\n\n"
        "Sends a message to all online wizards.\n"
        "Wizards only.  Players receive: 'You are not a wizard.'\n"
        "See also: ooc, tell\n"
    );
}
