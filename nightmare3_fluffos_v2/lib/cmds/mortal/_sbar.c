/* /cmds/mortal/_sbar.c
   AetherMUD status bar -- toggle and display HP/SDC/MDC/PPE/ISP after each move. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_sbar(string str) {
    int cur;

    if(!str || str == "") {
        cur = (int)this_player()->query_property("sbar_active");
        if(cur) {
            this_player()->set_property("sbar_active", 0);
            this_player()->save_player((string)this_player()->query_name());
            write("Status bar disabled.\n");
        } else {
            this_player()->set_property("sbar_active", 1);
            this_player()->save_player((string)this_player()->query_name());
            write("Status bar enabled. It will appear after each move.\n");
        }
        return 1;
    }
    str = lower_case(str);
    if(str == "on") {
        this_player()->set_property("sbar_active", 1);
        this_player()->save_player((string)this_player()->query_name());
        write("Status bar enabled.\n");
        return 1;
    }
    if(str == "off") {
        this_player()->set_property("sbar_active", 0);
        this_player()->save_player((string)this_player()->query_name());
        write("Status bar disabled.\n");
        return 1;
    }
    notify_fail("Usage: sbar [on|off]\n");
    return 0;
}

void help() {
    write(
        "Syntax: sbar [on|off]\n\n"
        "Toggles the status bar displayed after each room move.\n"
        "  sbar     -- toggles on/off\n"
        "  sbar on  -- enables the status bar\n"
        "  sbar off -- disables the status bar\n\n"
        "The bar shows your current HP, SDC, MDC, PPE, and ISP.\n"
        "Omits PPE and ISP sections if your max is 0.\n"
        "Use 'colorize' to see the bar in green.\n"
    );
}
