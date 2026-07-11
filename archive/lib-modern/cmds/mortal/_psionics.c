/* /cmds/mortal/_psionics.c
   Lists a player's known psionic powers with grouped presentation. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_psionics(string str) {
    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(str && str != "" &&
       lower_case(str) != "all" && lower_case(str) != "verbose") {
        notify_fail("Syntax: psionics [all|verbose]\n");
        return 0;
    }
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        notify_fail("You don't have psionic abilities.\n");
        return 0;
    }
    if(!(int)RIFTS_D->player_has_psi_access(this_player())) {
        write("You do not have psionic abilities.\n");
        return 1;
    }

    RIFTS_PSIONICS_D->write_psionic_list(this_player(),
        str ? lower_case(str) : "");
    return 1;
}

void help() {
    write(
        "Syntax: psionics [all|verbose]\n\n"
        "Lists your known psionic powers grouped by source.\n"
        "Godlings see a summary unless you type 'psionics all' or 'verbose'.\n"
    );
}
