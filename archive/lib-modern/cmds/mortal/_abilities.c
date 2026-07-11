/* /cmds/mortal/_abilities.c
   RiftsMUD -- shows racial and OCC special abilities for the player. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_abilities(string str) {
    string race, occ_name, desc, tag_line;
    string *race_abilities, *race_penalties, *occ_flags, *bskills;
    mapping occ_data;
    int i, apm, ppe, isp, max_ppe, max_isp;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }

    race = (string)this_player()->query_race();

    if(!(int)RIFTS_D->is_rifts_race(race)) {
        write("No special abilities.");
        return 1;
    }

    race_abilities = (string *)RIFTS_D->query_race_abilities(race);
    race_penalties = (string *)RIFTS_D->query_race_penalties(race);

    write("=== Racial Abilities (" + capitalize(race) + ") ===");
    if(!race_abilities || !sizeof(race_abilities)) {
        write("  None.");
    } else {
        for(i = 0; i < sizeof(race_abilities); i++)
            write("  - " + race_abilities[i]);
    }
    if(race_penalties && sizeof(race_penalties)) {
        write("Racial Penalties:");
        for(i = 0; i < sizeof(race_penalties); i++)
            write("  - " + race_penalties[i]);
    }

    write("");

    occ_name = (string)this_player()->getenv("rifts_occ");
    if(!occ_name || occ_name == "" || occ_name == "none") {
        write("=== OCC Abilities ===");
        write("  Racial character with no OCC chosen.");
    } else {
        occ_data = (mapping)OCC_D->query_occ(occ_name);
        write("=== OCC Abilities (" + capitalize(occ_name) + ") ===");
        if(!occ_data) {
            write("  No data available for this OCC.");
        } else {
            desc = (string)occ_data["description"];
            if(desc && sizeof(desc)) write("  " + desc);

            apm = (int)RIFTS_COMBAT_D->query_apm(this_player());
            write("  APM: " + apm + " base attacks per melee");

            occ_flags = (string *)occ_data["flags"];
            if(occ_flags && sizeof(occ_flags)) {
                tag_line = "  Type: ";
                for(i = 0; i < sizeof(occ_flags); i++) {
                    if(i > 0) tag_line += ", ";
                    tag_line += occ_flags[i];
                }
                write(tag_line);
            }

            bskills = (string *)occ_data["base_skills"];
            if(bskills && sizeof(bskills)) {
                write("  Base Skills:");
                for(i = 0; i < sizeof(bskills); i++)
                    write("    " + bskills[i]);
            }
        }
    }

    ppe     = (int)this_player()->query_stats("PPE");
    max_ppe = (int)this_player()->query_stats("max_PPE");
    isp     = (int)this_player()->query_stats("ISP");
    max_isp = (int)this_player()->query_stats("max_ISP");
    if(max_ppe > 0 || max_isp > 0) {
        write("");
        if(max_ppe > 0)
            write("  PPE: " + ppe + " / " + max_ppe + "  (Potential Psychic Energy)");
        if(max_isp > 0)
            write("  ISP: " + isp + " / " + max_isp + "  (Inner Strength Points)");
    }

    return 1;
}

void help() {
    write("Syntax: <abilities>\n\n"
          "Shows your racial special abilities and OCC-granted abilities.\n"
          "For Rifts characters: lists racial powers, OCC description, APM,\n"
          "type flags, base skills, and PPE/ISP pools.\n"
          "See also: score, skills, stats\n"
    );
}
