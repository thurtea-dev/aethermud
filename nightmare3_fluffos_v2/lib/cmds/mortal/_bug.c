// /cmds/mortal/_bug.c
// Report a bug. Appends to /log/bugs.

#include <std.h>

inherit DAEMON;

int cmd_bug(string str) {
    string loc;
    string entry;
    object env;

    if(!str || str == "") {
        write("Usage: bug <description of the bug>\n");
        return 1;
    }

    env = environment(this_player());
    loc = env ? base_name(env) : "unknown";

    entry = sprintf("[%s] %s (at %s): %s\n",
        ctime(time()), (string)this_player()->query_cap_name(), loc, str);

    log_file("bugs", entry);
    write("Bug report filed. Thank you!\n");
    return 1;
}

void help() {
    write(
        "Syntax: bug <description>\n\n"
        "Report a bug to the staff. Please include as much detail as possible.\n\n"
        "See also: idea, typo\n"
    );
}
