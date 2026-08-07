// /cmds/mortal/_experience.c
// AetherMUD  -  Show current XP and progress to next level.
// Aliases: xp, exp

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_experience(string str) {
    string race, occ;
    int level, exp, next_exp, needed;
    mapping occ_data;
    int *xp_table;
    int *def;

    if(this_player()->query_ghost()) {
        notify_fail("The dead have no experience worth counting.\n");
        return 0;
    }

    level = (int)this_player()->query_level();
    exp   = (int)this_player()->query_exp();
    race  = (string)this_player()->query_race();

    if(!(int)RIFTS_D->is_rifts_race(race)) {
        // Non-Rifts: show NM3 XP using the standard advance daemon's table
        next_exp = (int)ADVANCE_D->get_exp(level + 1);
        needed   = next_exp - exp;
        if(needed < 0) needed = 0;
        message("status",
            sprintf("Level: %-3d  XP: %-8d  Next level: %-8d  Needed: %d",
                level, exp, next_exp, needed),
            this_player());
        return 1;
    }

    // Rifts character: use OCC XP table
    occ = (string)this_player()->getenv("rifts_occ");
    if(occ && occ != "none" && occ != "")
        occ_data = (mapping)OCC_D->query_occ(occ);

    if(occ_data && occ_data["xp_table"]) {
        xp_table = (int *)occ_data["xp_table"];
        if(level >= sizeof(xp_table)) {
            // At or past the end of the table
            next_exp = -1;
        } else {
            next_exp = xp_table[level];
        }
    } else {
        // Default table for no-OCC or unknown OCC /* approx */
        def = ({ 0, 2000, 4000, 8000, 16000, 24000, 48000, 80000,
                 120000, 160000, 230000, 330000, 430000, 530000, 630000 });
        next_exp = (level < sizeof(def)) ? def[level] : -1;
    }

    if(level >= 15) {
        message("status",
            sprintf("Level: %-3d  XP: %-8d  [Maximum level reached]", level, exp),
            this_player());
        return 1;
    }

    needed = (next_exp >= 0) ? next_exp - exp : 0;
    if(needed < 0) needed = 0;

    message("status",
        sprintf("Level: %-3d  XP: %-8d  Next level: %-8d  Needed: %d",
            level, exp, (next_exp >= 0 ? next_exp : 0), needed),
        this_player());
    return 1;
}

void help() {
    message("help",
        "Syntax: experience  (aliases: xp, exp)\n\n"
        "Shows your current experience level and how much more you need\n"
        "to advance to the next level.\n\n"
        "See also: advance, score",
        this_player());
}
