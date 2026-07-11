// /cmds/mortal/_skills.c
// Displays player skills grouped by category.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

// Convert string to uppercase for category headers.
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

// ── Rifts skill display ──────────────────────────────────────────────────────

protected void show_rifts_skills() {
    string *all_skills;
    string *ordered_cats;
    mapping cat_map;
    string hth;
    string active_lang;
    mapping sdata;
    int i, j;
    string sname, cat;
    string *cat_skills;

    // H2H type (stored by rifts_start_d when granting)
    hth         = (string)this_player()->getenv("rifts_hth");
    active_lang = (string)this_player()->getenv("active_language");
    if(!active_lang) active_lang = "American";

    all_skills = (string *)RIFTS_SKILLS_D->query_player_skills(this_player());
    if(!all_skills) all_skills = ({});

    // Build category buckets
    cat_map = ([]);
    for(i = 0; i < sizeof(all_skills); i++) {
        sname = all_skills[i];
        // Skip raw combat bonuses (shown in score)
        if(sname == "melee" || sname == "attack" || sname == "defense") continue;
        sdata = (mapping)RIFTS_SKILLS_D->query_rifts_skill(sname);
        cat   = sdata ? (string)sdata["category"] : "other";
        if(!cat || !sizeof(cat)) cat = "other";
        if(!cat_map[cat]) cat_map[cat] = ({ sname });
        else cat_map[cat] = cat_map[cat] + ({ sname });
    }

    ordered_cats = ({ "weapons", "pilot", "military", "espionage",
                      "survival", "physical", "medical", "technical",
                      "lore", "other" });

    write("%^BOLD%^=== SKILLS: " + (string)this_player()->query_cap_name() +
          " ===%^RESET%^");
    write("");

    // H2H section
    if(hth && sizeof(hth)) {
        write("%^BOLD%^COMBAT%^RESET%^");
        write(sprintf("  %-36s %s", "Hand to Hand " + hth, "[innate]"));
        write("");
    }

    // Categorised skills
    for(j = 0; j < sizeof(ordered_cats); j++) {
        cat       = ordered_cats[j];
        cat_skills = (string *)cat_map[cat];
        if(!cat_skills || !sizeof(cat_skills)) continue;

        write("%^BOLD%^" + str_upper(cat) + "%^RESET%^");
        write_skill_columns(cat_skills);
        write("");
    }

    // Language section
    write("%^BOLD%^LANGUAGE%^RESET%^");
    write(sprintf("  %-36s %s", active_lang + " (active)", "98%"));

    // Summary
    write("");
    write("Known skills: " + sizeof(all_skills));
    write("* = innate/master. Use score for skill percentages.");
}

// ── Standard Nightmare display ───────────────────────────────────────────────

protected void show_standard_skills() {
    string *attack, *weapon, *decep, *mag;
    int i;

    attack = ({ "melee", "attack", "defense", "double wielding" });
    weapon = ({ "knife", "blade", "blunt", "two handed", "projectile" });
    decep  = ({ "locks", "stealth", "murder", "stealing", "bargaining" });
    mag    = ({ "faith", "healing", "conjuring", "magic attack", "magic defense" });

    write("You have the following learned attributes:");
    write(arrange_string("Combat", 22) + arrange_string("Weapons", 17) +
          arrange_string("Deception", 17) + "Magic");
    write("--------------------------------------------------------------------------");
    for(i = 0; i < 5; i++) {
        message("Ninfo", (i < 4 ? arrange_string(attack[i], 16) + ": " :
            arrange_string(" ", 16) + "  "), this_player());
        message("Ninfo", arrange_string(i > 3 ? " " :
            "" + (int)this_player()->query_base_skill(attack[i]), 4), this_player());
        message("Ninfo", arrange_string(weapon[i], 11) + ": ", this_player());
        message("Ninfo", arrange_string(
            "" + this_player()->query_base_skill(weapon[i]), 4), this_player());
        message("Ninfo", arrange_string(decep[i], 11) + ": ", this_player());
        message("Ninfo", arrange_string(
            "" + this_player()->query_base_skill(decep[i]), 4), this_player());
        message("Ninfo", arrange_string(mag[i], 15) + ": ", this_player());
        message("Ninfo", arrange_string(
            "" + this_player()->query_base_skill(mag[i]), 4) + "\n", this_player());
    }
}

// ── Command entry ─────────────────────────────────────────────────────────────

int cmd_skills(string str) {
    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(str) return 0;

    if((int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        show_rifts_skills();
        return 1;
    }
    show_standard_skills();
    return 1;
}

void help() {
    write("Syntax: skills\n\n"
          "Rifts characters: shows all skills grouped by category.\n"
          "Other characters: shows numeric skill levels in Combat/Weapons/Deception/Magic.\n"
    );
}
