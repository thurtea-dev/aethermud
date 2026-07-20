/* /cmds/mortal/_sskills.c
   Show secondary/elective skills (those not in the OCC's base skill list).
   Syntax: sskills */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

#define SKILL_COLUMNS 4

private string compact_skill_label(string sname) {
    int pct;
    string s;

    pct = (int)this_player()->query_base_skill(sname);
    s = capitalize(sname);
    if(strlen(s) > 16) s = s[0..15];
    if(pct >= 98) s += "*";
    return s;
}

private void write_skill_columns(string *names) {
    string *labels;
    int i;

    if(!names || !sizeof(names)) return;
    labels = ({});
    for(i = 0; i < sizeof(names); i++)
        labels += ({ compact_skill_label(names[i]) });
    write(format_page(labels, SKILL_COLUMNS));
}

int cmd_sskills(string str) {
    string *all_skills, *base_skills, *norm_base, *secondary;
    mapping occ_data;
    string occ, sname, norm;
    int i, j, found;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(!(int)RIFTS_D->is_rifts_race(
            (string)this_player()->query_race())) {
        write("Secondary skills only apply to Rifts characters.\n");
        return 1;
    }

    all_skills = (string *)RIFTS_SKILLS_D->query_player_skills(this_player());
    if(!all_skills) all_skills = ({});

    occ       = (string)this_player()->getenv("rifts_occ");
    base_skills = ({});

    if(occ && sizeof(occ)) {
        occ_data = (mapping)OCC_D->query_occ(occ);
        if(occ_data && occ_data["base_skills"])
            base_skills = (string *)occ_data["base_skills"];
    }

    /* Normalize each raw OCC skill string the same way apply_occ_skills()
       does before granting, so the exclusion match actually lines up with
       the player's real (normalized) granted skill keys. */
    norm_base = ({});
    for(i = 0; i < sizeof(base_skills); i++) {
        norm = (string)RIFTS_SKILLS_D->normalize_skill(base_skills[i]);
        if(!norm || !sizeof(norm)) continue;
        norm_base += ({ norm });
    }

    secondary = ({});
    for(i = 0; i < sizeof(all_skills); i++) {
        sname = all_skills[i];
        if(sname == "melee" || sname == "attack" || sname == "defense") continue;
        found = 0;
        for(j = 0; j < sizeof(norm_base); j++) {
            if(lower_case(norm_base[j]) == lower_case(sname)) {
                found = 1;
                break;
            }
        }
        if(!found) secondary += ({ sname });
    }

    write(repeat_string("-", 50));
    write("  SECONDARY / ELECTIVE SKILLS");
    write(repeat_string("-", 50));
    if(!sizeof(secondary)) {
        write("  You have no secondary skills.");
    } else {
        write_skill_columns(secondary);
    }
    write(repeat_string("-", 50));
    write("Total secondary skills: " + sizeof(secondary));
    return 1;
}

void help() {
    message("help",
        "Syntax: sskills\n\n"
        "Displays your secondary and elective skills -- those not\n"
        "included automatically with your OCC.\n\n"
        "See also: skills, abilities, pskills",
        this_player());
}
