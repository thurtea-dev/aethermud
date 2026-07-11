// /cmds/mortal/_color.c
// Alias: color off -> colorize off, color on -> colorize on

#include <std.h>

inherit DAEMON;

int cmd_color(string str) {
    string lc;

    if(!str || str == "") {
        notify_fail("Usage: color on|off\n");
        return 0;
    }
    lc = lower_case(str);
    if(lc == "off") {
        this_player()->setenv("TERM", "unknown");
        this_player()->setenv("colorize_set", "1");
        this_player()->reset_terminal();
        this_player()->save_player((string)this_player()->query_name());
        write("Color disabled.\n");
        return 1;
    }
    if(lc == "on") {
        this_player()->setenv("TERM", "ansi");
        this_player()->setenv("colorize_set", "1");
        this_player()->reset_terminal();
        this_player()->save_player((string)this_player()->query_name());
        write("Color enabled. Type 'look' to see the difference.\n");
        return 1;
    }
    notify_fail("Usage: color on|off\n");
    return 0;
}

void help() {
    write("Syntax: color on|off\n\nAlias for colorize on/off.\n");
}
