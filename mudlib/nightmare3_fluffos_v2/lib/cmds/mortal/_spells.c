/* /cmds/mortal/_spells.c
   Lists a player's known spells with grouped presentation. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_spells(string str) {
    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(str && str != "" &&
       lower_case(str) != "all" && lower_case(str) != "verbose") {
        notify_fail("Syntax: spells [all|verbose]\n");
        return 0;
    }
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        notify_fail("You don't have the ability to cast spells.\n");
        return 0;
    }
    if(!(int)RIFTS_D->player_has_magic_access(this_player())) {
        write("You do not know how to cast spells.\n");
        return 1;
    }

    RIFTS_SPELLS_D->write_spell_list(this_player(),
        str ? lower_case(str) : "");
    return 1;
}

void help() {
    write(
        "Syntax: spells [all|verbose]\n\n"
        "Lists your known spells grouped by source (race, OCC, learned).\n"
        "Wizards see a summary unless you type 'spells all' or 'verbose'.\n"
    );
}
