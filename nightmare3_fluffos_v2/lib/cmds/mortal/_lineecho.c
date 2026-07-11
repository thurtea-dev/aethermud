/* /cmds/mortal/_lineecho.c
   Harmless server-side toggle for clients that honor lineecho mode. */

#include <std.h>

inherit DAEMON;

int cmd_lineecho(string str) {
    if(!str || !sizeof(str)) {
        if((string)this_player()->getenv("lineecho") == "1")
            write("Lineecho is on.\n");
        else
            write("Lineecho is off.\n");
        return 1;
    }
    if(lower_case(str) == "on") {
        this_player()->setenv("lineecho", "1");
        write("Lineecho on. Most clients ignore this flag; configure echo\n"
            "in your MUD client if lines do not display as you expect.\n");
        return 1;
    }
    if(lower_case(str) == "off") {
        this_player()->remove_env("lineecho");
        write("Lineecho off.\n");
        return 1;
    }
    write("Usage: lineecho [on|off]\n");
    return 1;
}

void help() {
    write(
        "Syntax: lineecho [on|off]\n\n"
        "Lineecho is primarily a client-side setting. Some MUD clients\n"
        "can echo each input line locally before sending it to the game.\n"
        "This command stores an on/off preference on your character in\n"
        "case a client reads it; the stock server does not change how\n"
        "lines display.\n\n"
        "See also: triggers\n"
    );
}
