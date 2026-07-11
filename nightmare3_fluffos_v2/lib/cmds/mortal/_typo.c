// /cmds/mortal/_typo.c
// Report a typo. Appends to /log/typos.

#include <std.h>

inherit DAEMON;

int cmd_typo(string str) {
    string loc;
    string entry;
    object env;

    if(!str || str == "") {
        write("Usage: typo <description of the typo>\n");
        return 1;
    }

    env = environment(this_player());
    loc = env ? base_name(env) : "unknown";

    entry = sprintf("[%s] %s (at %s): %s\n",
        ctime(time()), (string)this_player()->query_cap_name(), loc, str);

    log_file("typos", entry);
    write("Typo reported. Thank you!\n");
    return 1;
}

void help() {
    write(
        "Syntax: typo <description>\n\n"
        "Report a typo or text error to the staff.\n\n"
        "See also: bug, idea\n"
    );
}
