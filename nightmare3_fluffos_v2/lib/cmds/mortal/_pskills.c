/* /cmds/mortal/_pskills.c
   Display primary OCC skills -- those granted automatically by the OCC.
   Syntax: pskills */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private string str_upper(string s) {
    string r;
    int i, c;
    r = "";
    for(i = 0; i < sizeof(s); i++) {
        c = s[i];
        if(c >= 'a' && c <= 'z') c -= 32;
        r += sprintf("%c", c);
    }
    return r;
}

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

int cmd_pskills(string str) {
    string occ, sname, cat, hth;
    string *base_skills, *ordered_cats, *cat_skills;
    mapping occ_data, cat_map, sdata;
    int i, j;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(!(int)RIFTS_D->is_rifts_race(
            (string)this_player()->query_race())) {
        write("Primary skills only apply to Rifts characters.\n");
        return 1;
    }

    occ = (string)this_player()->getenv("rifts_occ");
    if(!occ || !sizeof(occ) || occ == "none") {
        write("You have no O.C.C. primary skill block.");
        write("Use skills for your full skill list.");
        return 1;
    }

    occ_data = (mapping)OCC_D->query_occ(occ);
    if(!occ_data || !occ_data["base_skills"]) {
        write("No primary skill data found for your O.C.C. (" + occ + ").");
        write("Use skills for your full skill list.");
        return 1;
    }

    base_skills = (string *)occ_data["base_skills"];
    hth = (string)this_player()->getenv("rifts_hth");

    cat_map = ([]);
    for(i = 0; i < sizeof(base_skills); i++) {
        sname = base_skills[i];
        if(sname == "melee" || sname == "attack" || sname == "defense") continue;
        sdata = (mapping)RIFTS_SKILLS_D->query_rifts_skill(sname);
        cat = sdata ? (string)sdata["category"] : "other";
        if(!cat || !sizeof(cat)) cat = "other";
        if(!cat_map[cat]) cat_map[cat] = ({ sname });
        else cat_map[cat] = cat_map[cat] + ({ sname });
    }

    ordered_cats = ({ "weapons", "pilot", "military", "espionage",
                      "survival", "physical", "medical", "technical",
                      "lore", "other" });

    write("%^BOLD%^=== PRIMARY SKILLS: " + str_upper(occ) + " ===%^RESET%^");
    write("");

    if(hth && sizeof(hth)) {
        write("%^BOLD%^COMBAT%^RESET%^");
        write(sprintf("  %-36s %s", "Hand to Hand " + hth, "[primary]"));
        write("");
    }

    for(j = 0; j < sizeof(ordered_cats); j++) {
        cat = ordered_cats[j];
        cat_skills = (string *)cat_map[cat];
        if(!cat_skills || !sizeof(cat_skills)) continue;
        write("%^BOLD%^" + str_upper(cat) + "%^RESET%^");
        write_skill_columns(cat_skills);
        write("");
    }

    write("Total primary skills: " + sizeof(base_skills));
    return 1;
}

void help() {
    message("help",
        "Syntax: pskills\n\n"
        "Displays your primary O.C.C. skills -- those granted automatically\n"
        "from your Occupational Character Class.\n\n"
        "See also: skills, sskills, psionics",
        this_player());
}
