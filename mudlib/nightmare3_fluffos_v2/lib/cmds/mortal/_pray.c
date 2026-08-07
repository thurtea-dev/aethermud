// /cmds/mortal/_pray.c
// Pray for faster resurrection in the Rifts death chamber.
// Reduces auto-resurrect timer from 2 minutes to 30 seconds.

#include <std.h>

inherit DAEMON;

int cmd_pray(string str) {
    if(!(int)this_player()->query_ghost()) {
        notify_fail("You are not dead.  Save the prayers for when you need them.\n");
        return 0;
    }
    this_player()->pray_resurrect();
    return 1;
}

void help() {
    write(
        "Syntax: pray\n\n"
        "When you have been killed and are waiting in the grey void,\n"
        "typing 'pray' calls upon the forces of the Rifts to hasten\n"
        "your return to the living world.\n\n"
        "  - Normal resurrection: automatic after 2 minutes\n"
        "  - After praying:       30 seconds\n\n"
        "You will return to the Rift arrival zone with 1 HP.\n"
        "Your equipment remains in a corpse at the site of your death.\n"
        "The corpse dissolves after 10 minutes, dropping items to the floor.\n\n"
        "See also: death\n"
    );
}
