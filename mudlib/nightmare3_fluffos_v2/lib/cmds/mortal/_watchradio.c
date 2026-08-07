// /cmds/mortal/_watchradio.c
// Toggle whether this player receives radio transmissions.
// Default: ON (receiving). Off suppresses all radio output.

#include <std.h>

inherit DAEMON;

int cmd_watchradio(string str) {
    if(!str || str == "") {
        if((string)this_player()->getenv("radio_off") == "1")
            write("Radio channel: OFF. Type 'watchradio on' to re-enable.\n");
        else
            write("Radio channel: ON. Type 'watchradio off' to stop receiving.\n");
        return 1;
    }
    str = lower_case(str);
    if(str == "off") {
        this_player()->setenv("radio_off", "1");
        write("Radio channel disabled. You will no longer receive transmissions.\n");
        return 1;
    }
    if(str == "on") {
        this_player()->remove_env("radio_off");
        write("Radio channel enabled.\n");
        return 1;
    }
    notify_fail("Usage: watchradio [on|off]\n");
    return 0;
}

void help() {
    write(
        "Syntax: watchradio [on|off]\n\n"
        "Controls whether you receive global radio transmissions.\n"
        "  watchradio off  -- stop receiving radio messages\n"
        "  watchradio on   -- resume receiving radio messages\n"
        "  watchradio      -- show current status\n\n"
        "See also: radio, ooc\n"
    );
}
