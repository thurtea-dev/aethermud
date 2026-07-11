// /cmds/mortal/_bionics.c
// Display the player's installed cybernetic modifications.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_bionics(string str) {
    mapping bionics, bdef;
    string *slots;
    int i;

    bionics = (mapping)BIONIC_D->query_bionics(this_player());
    if(!mapp(bionics) || !sizeof(bionics)) {
        write("You have no cybernetic modifications installed.\n");
        return 1;
    }
    write("Your installed cybernetics:\n");
    slots = keys(bionics);
    i = sizeof(slots);
    while(i--) {
        bdef = (mapping)BIONIC_D->query_bionic_stats((string)bionics[slots[i]]);
        if(mapp(bdef) && sizeof(bdef))
            write("  " + slots[i] + ": " + bionics[slots[i]] + ", " +
                  (string)bdef["desc"] + "\n");
        else
            write("  " + slots[i] + ": " + bionics[slots[i]] + "\n");
    }
    return 1;
}

void help() {
    message("help",
        "Syntax: bionics\n\n"
        "Lists your installed cybernetic modifications and their bonuses.\n\n"
        "See also: score",
        this_player());
}
