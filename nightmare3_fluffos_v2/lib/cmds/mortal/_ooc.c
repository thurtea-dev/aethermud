// /cmds/mortal/_ooc.c
// OOC: global out-of-character channel.
// Bypasses language and location. Type "ooc off" to stop receiving.

#include <std.h>

inherit DAEMON;

int cmd_ooc(string str) {
    object *all_players;
    string msg;
    int i;

    if(!str || str == "") {
        notify_fail("Usage: ooc <message>   (or: ooc off / ooc on)\n");
        return 0;
    }
    if(lower_case(str) == "off") {
        this_player()->setenv("ooc_deaf", "1");
        write("OOC channel disabled. Type 'ooc on' to re-enable.\n");
        return 1;
    }
    if(lower_case(str) == "on") {
        this_player()->remove_env("ooc_deaf");
        write("OOC channel enabled.\n");
        return 1;
    }
    all_players = users();
    for(i = 0; i < sizeof(all_players); i++) {
        if(!all_players[i]) continue;
        if((string)all_players[i]->getenv("ooc_deaf") == "1") continue;
        msg = sprintf("[OOC] %s: %s\n",
            (string)this_player()->query_display_name(all_players[i]), str);
        message("ooc", msg, all_players[i]);
    }
    return 1;
}

void help() {
    write(
        "Syntax: ooc <message>\n"
        "        ooc off    stop receiving OOC messages\n"
        "        ooc on     resume receiving OOC messages\n\n"
        "Sends a message to all players on the global out-of-character channel.\n"
        "OOC bypasses language barriers and in-game location.\n\n"
        "See also: say, tell, shout\n"
    );
}
