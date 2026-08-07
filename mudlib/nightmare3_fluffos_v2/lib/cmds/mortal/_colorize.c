// /cmds/mortal/_colorize.c
// Opt-in ANSI color control.
// colorize     -- toggle: if off turn on, if on turn off
// colorize on  -- force enable ANSI color
// colorize off -- force disable ANSI color

#include <std.h>

inherit DAEMON;

int cmd_colorize(string str) {
    string current;

    if(!str || str == "") {
        current = (string)this_player()->getenv("TERM");
        if(!current || current == "" || current == "unknown" || current == "plain") {
            this_player()->setenv("TERM", "ansi");
            this_player()->setenv("colorize_set", "1");
            this_player()->reset_terminal();
            this_player()->save_player((string)this_player()->query_name());
            write("%^BOLD%^%^GREEN%^Color enabled.%^RESET%^\n");
        } else {
            this_player()->setenv("TERM", "unknown");
            this_player()->setenv("colorize_set", "1");
            this_player()->reset_terminal();
            this_player()->save_player((string)this_player()->query_name());
            write("Color disabled.\n");
        }
        return 1;
    }

    str = lower_case(str);

    if(str == "on") {
        this_player()->setenv("TERM", "ansi");
        this_player()->setenv("colorize_set", "1");
        this_player()->reset_terminal();
        this_player()->save_player((string)this_player()->query_name());
        write("%^BOLD%^%^GREEN%^Color enabled.%^RESET%^\n");
        return 1;
    }

    if(str == "off") {
        this_player()->setenv("TERM", "unknown");
        this_player()->setenv("colorize_set", "1");
        this_player()->reset_terminal();
        this_player()->save_player((string)this_player()->query_name());
        write("Color disabled.\n");
        return 1;
    }

    notify_fail("Usage: colorize [on|off]\n");
    return 0;
}

void help() {
    write(
        "Syntax: colorize [off]\n\n"
        "Controls ANSI color output. Color is off by default.\n"
        "  colorize      -- turns color on (bold green exits, colored chat, etc.)\n"
        "  colorize off  -- turns color off (plain text output)\n\n"
        "Your preference is saved and applied automatically on future logins.\n"
    );
}
