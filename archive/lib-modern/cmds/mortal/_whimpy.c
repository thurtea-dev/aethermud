/* /cmds/mortal/_whimpy.c
   Set auto-flee HP threshold. Accepts numeric percentage or on/off.
   Syntax: whimpy              -- show current setting
           whimpy <1-90>       -- auto-flee below this % of max HP
           whimpy off          -- disable auto-flee */

#include <std.h>

inherit DAEMON;

int cmd_whimpy(string str) {
    int cur, pct;
    string s;

    cur = (int)this_player()->query_wimpy();

    if(!str || !strlen(str)) {
        if(cur > 0)
            write("Auto-flee is ON at " + cur + "% of maximum HP.\n");
        else
            write("Auto-flee is OFF. (Brave mode)\n");
        return 1;
    }

    s = lower_case(str);

    if(s == "off" || s == "0") {
        this_player()->set_wimpy(0);
        write("Auto-flee disabled. Brave mode.\n");
        return 1;
    }
    if(s == "on") {
        this_player()->set_wimpy(25);
        write("Auto-flee enabled at 25% HP.\n");
        return 1;
    }

    pct = to_int(str);
    if(pct < 1 || pct > 90) {
        write("Threshold must be between 1 and 90.\n");
        return 1;
    }
    this_player()->set_wimpy(pct);
    write("Auto-flee set to " + pct + "% of maximum HP.\n");
    return 1;
}

void help() {
    message("help",
        "Syntax: whimpy              -- show current setting\n"
        "        whimpy <1-90>       -- auto-flee below this % of max HP\n"
        "        whimpy off          -- disable auto-flee\n\n"
        "When auto-flee is active, you will attempt to flee combat\n"
        "automatically when your HP drops below the given percentage.\n"
        "Note: a single hit can take you below the threshold before\n"
        "auto-flee triggers, so it is not a guarantee of survival.\n\n"
        "See also: wimpy, flee, score",
        this_player());
}
