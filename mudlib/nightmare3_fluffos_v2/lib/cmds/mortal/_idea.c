// /cmds/mortal/_idea.c
// Submit a game idea/suggestion. Appends to /log/ideas.

#include <std.h>

inherit DAEMON;

int cmd_idea(string str) {
    string loc;
    string entry;
    object env;

    if(!str || str == "") {
        write("Usage: idea <your suggestion>\n");
        return 1;
    }

    env = environment(this_player());
    loc = env ? base_name(env) : "unknown";

    entry = sprintf("[%s] %s (at %s): %s\n",
        ctime(time()), (string)this_player()->query_cap_name(), loc, str);

    log_file("ideas", entry);
    write("Idea submitted. Thank you for the suggestion!\n");
    return 1;
}

void help() {
    write(
        "Syntax: idea <suggestion>\n\n"
        "Submit a gameplay idea or suggestion to the staff.\n\n"
        "See also: bug, typo\n"
    );
}
