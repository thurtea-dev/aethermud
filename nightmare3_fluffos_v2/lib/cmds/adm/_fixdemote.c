// /cmds/adm/_fixdemote.c
// One-time repair command for stale demotions. Re-runs the full
// demotion restore (USERS_D->offline_demote, which restores the save
// into the users daemon, applies staff_of_demotion's
// apply_demote_restore, and writes the save back) on a named OFFLINE
// player. Use for saves demoted before the offline restore path
// existed: they may still carry Wizard race, its 98% skill package,
// or /domains/Praxis/rifts_welcome as their start room.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_fixdemote(string str) {
    string pname;

    if(!admin_wizp(this_player())) {
        notify_fail("You don't have permission to fix demotions.\n");
        return 0;
    }
    if(!str || !sizeof(str)) {
        write("Usage: fixdemote <player>\n");
        return 1;
    }
    pname = lower_case(str);
    if(find_player(pname)) {
        write(capitalize(pname) + " is online. Use the staff of demotion's "
            "demote option instead; fixdemote only repairs offline saves.\n");
        return 1;
    }
    if(!user_exists(pname)) {
        write("No such player: " + capitalize(pname) + ".\n");
        return 1;
    }
    if((int)USERS_D->offline_demote(pname)) {
        write("Restored " + capitalize(pname) + " to mortal: race, class, "
            "level, stats, skills, and start room rebuilt from their "
            "pre-wizard snapshot (or rerolled as human if none), and the "
            "save file rewritten.\n");
        write("Details were logged to /log/adm/staff_promotions.\n");
    } else {
        write("Unable to restore " + capitalize(pname) + "'s save. Check "
            "that the save file exists and the player is offline.\n");
    }
    return 1;
}

void help() {
    write(
        "Syntax: fixdemote <player>\n\n"
        "Repairs the save file of an offline player whose demotion "
        "predates the offline restore path. Such saves can still carry "
        "Wizard race, its full 98% skill package, or the wizard "
        "welcome room as their start room.\n\n"
        "Runs the same restore as the staff of demotion's demote option "
        "for offline players: position set to player, race/class/level/"
        "stats restored from the premote snapshot (or rerolled as human "
        "when no snapshot exists), Wizard race skills stripped, mortal race "
        "and OCC packages rebuilt, and a safe mortal start room chosen. "
        "The save file is rewritten immediately.\n\n"
        "The target must be offline. For online players use the staff "
        "of demotion instead.\n\n"
        "Restricted to admin wizards.\n\n"
        "Example: fixdemote macguyver\n"
        "See also: makewiz, setrole\n"
    );
}
