/*    /daemon/help.c
 *    from Nightmare IV
 *    daemon handling mud help functions and help interface
 *    created by Descartes of Borg 940420
 */

#include <std.h>
#include <daemons.h>
#include <dirs.h>
#include "help.h"

inherit DAEMON;

#define HELP_SEC_RACES    0
#define HELP_SEC_OCCS     1
#define HELP_SEC_SKILLS   2
#define HELP_SEC_COMBAT   3
#define HELP_SEC_COMM     4
#define HELP_SEC_SYSTEMS  5
#define HELP_SEC_COMMANDS 6
#define HELP_SEC_ALIGN    7
#define HELP_SEC_STAFF    8
#define HELP_SEC_PSIONICS 9

varargs void help(string topic, string category) {
    string staff_topic;
    int sec;
    int len;

    if(!topic || topic == "") {
        categories_display();
        return;
    }
    staff_topic = lower_case(topic);
    if(!category) {
        if(staff_topic == "index" || staff_topic == "all") {
            flat_help_display();
            return;
        }
        /* "help <name> overview" opens the topic file directly even when
         * <name> is claimed by a category listing (combat, occs, staff,
         * creator, admin, ...). */
        len = strlen(staff_topic);
        if(len > 9 && staff_topic[len - 9..] == " overview") {
            cmd_help(staff_topic[0..len - 10], 0);
            return;
        }
        sec = match_category_name(staff_topic);
        if(sec != -1) {
            category_display(sec);
            return;
        }
    }
    cmd_help(topic, category);
}

/* Category keyword -> section id, or -1 when the word is not a category. */
static int match_category_name(string str) {
    switch(str) {
    case "races": case "race":
        return HELP_SEC_RACES;
    case "classes": case "class": case "occs": case "occ":
    case "classes/occs":
        return HELP_SEC_OCCS;
    case "skills": case "skills & abilities":
        return HELP_SEC_SKILLS;
    case "psihelp":
        return HELP_SEC_PSIONICS;
    case "combat": case "combat & movement":
        return HELP_SEC_COMBAT;
    case "communication": case "social": case "comm":
    case "communication & social":
        return HELP_SEC_COMM;
    case "systems": case "system":
        return HELP_SEC_SYSTEMS;
    case "commands": case "command":
        return HELP_SEC_COMMANDS;
    case "alignments": case "alignment":
        return HELP_SEC_ALIGN;
    case "staff": case "wizard": case "staff/wizard":
    case "creator": case "admin":
        return HELP_SEC_STAFF;
    }
    return -1;
}

static string read_first_line(string path) {
    string content;
    int idx;

    if(!file_exists(path)) return "";
    content = read_file(path);
    if(!content || !sizeof(content)) return "";
    idx = strsrch(content, "\n");
    if(idx == -1) return content;
    return content[0..idx - 1];
}

/* Alias -> canonical help filename. Aliases stay findable via help <alias>
 * but are omitted from the flat index when the canonical file exists. */
static string help_alias_canonical(string topic) {
    string norm;

    norm = lower_case(topic);
    switch(norm) {
    case "colour":           return "color";
    case "whimpy":           return "wimpy";
    case "multiplaying":     return "multi-playing";
    case "privacy-policy":   return "privacy";
    case "user_mail":        return "mail";
    case "language":         return "languages";
    case "guilds":           return "guild";
    case "weapon":           return "weapons";
    case "equipment":        return "eq";
    case "special forces merc": return "specialforces";
    default:                 return 0;
    }
}

/* Topics that must never appear in the flat help index. */
static int help_index_junk(string topic) {
    string norm;

    norm = lower_case(topic);
    return member_array(norm, ({
        "foo", "testcolor", "bot", "bots",
        "coalition_grunt", "cs_psi-stalker",
        /* Sub-pages: still available via help <name>, not listed. */
        "concept1", "concept2",
        /* Meta-file shadowed by the "help index" full listing. */
        "index"
    })) != -1;
}

static string topic_slug(string name) {
    string s;

    s = lower_case(name);
    s = replace_string(s, " ", "_");
    s = replace_string(s, "-", "_");
    s = replace_string(s, "(", "");
    s = replace_string(s, ")", "");
    s = replace_string(s, ".", "");
    return s;
}

/* Strip all separators for loose topic matching: "armor of ithan",
 * "armor_of_ithan", and "armorofithan" all squash to "armorofithan". */
static string squash_topic(string s) {
    s = lower_case(s);
    s = replace_string(s, " ", "");
    s = replace_string(s, "_", "");
    s = replace_string(s, "-", "");
    s = replace_string(s, "(", "");
    s = replace_string(s, ")", "");
    return s;
}

/* Scan DIR_USER_HELP for a filename whose squash equals sq. */
static string squash_user_help_match(string sq) {
    string *files;
    int i;

    if(!sq || !sizeof(sq)) return 0;
    files = get_dir(DIR_USER_HELP + "/");
    if(!files) return 0;
    for(i = 0; i < sizeof(files); i++) {
        if(squash_topic(files[i]) == sq) return files[i];
    }
    return 0;
}

static string find_user_help_file(string name) {
    string slug;
    string alt;
    string match;

    if(!name || name == "") return 0;
    if(file_exists(DIR_USER_HELP + "/" + name)) return name;
    slug = topic_slug(name);
    if(file_exists(DIR_USER_HELP + "/" + slug)) return slug;
    alt = replace_string(slug, "_", "-");
    if(file_exists(DIR_USER_HELP + "/" + alt)) return alt;
    match = squash_user_help_match(squash_topic(name));
    if(match) return match;
    return 0;
}

static string normalize_help_topic(string topic) {
    string alt;
    string canon;
    string sq;
    string match;

    topic = lower_case(topic);
    /* Separator-insensitive resolve first so help armorofithan,
       help armor of ithan, and help armor_of_ithan all hit the same
       on-disk topic before alias / literal checks. */
    sq = squash_topic(topic);
    match = squash_user_help_match(sq);
    if(match) return match;
    canon = help_alias_canonical(topic);
    if(canon) {
        if(file_exists(DIR_USER_HELP + "/" + canon)) return canon;
        /* Fall through if canonical missing; try the alias filename. */
    }
    if(file_exists(DIR_USER_HELP + "/" + topic)) return topic;
    alt = replace_string(topic, " ", "_");
    if(file_exists(DIR_USER_HELP + "/" + alt)) return alt;
    alt = replace_string(topic, "_", " ");
    if(file_exists(DIR_USER_HELP + "/" + alt)) return alt;
    alt = replace_string(topic, "-", "_");
    if(file_exists(DIR_USER_HELP + "/" + alt)) return alt;
    return topic;
}

static string *dedupe_user_topics(string *topics) {
    string *result;
    string underscored;
    string canon;
    int i;

    result = ({});
    for(i = 0; i < sizeof(topics); i++) {
        if(help_index_junk(topics[i])) continue;
        canon = help_alias_canonical(topics[i]);
        if(canon && member_array(canon, topics) != -1) continue;
        if(strsrch(topics[i], " ") != -1) {
            underscored = replace_string(topics[i], " ", "_");
            if(member_array(underscored, topics) != -1) continue;
        }
        result += ({ topics[i] });
    }
    return result;
}

/* Playable Rifts race help filenames that exist on disk. */
static string *index_race_topics() {
    string *races;
    string *out;
    string file;
    int i;

    out = ({});
    if(file_exists(DIR_USER_HELP + "/races"))
        out += ({ "races" });
    races = (string *)RIFTS_D->query_rifts_races();
    if(!races) return out;
    for(i = 0; i < sizeof(races); i++) {
        file = find_user_help_file(races[i]);
        if(file && member_array(file, out) == -1)
            out += ({ file });
    }
    return out;
}

/* OCC help filenames that exist on disk. */
static string *index_occ_topics() {
    string *occs;
    string *out;
    string file;
    int i;

    out = ({});
    if(file_exists(DIR_USER_HELP + "/occs"))
        out += ({ "occs" });
    if(file_exists(DIR_USER_HELP + "/classes"))
        out += ({ "classes" });
    occs = (string *)OCC_D->query_occs();
    if(!occs) return out;
    for(i = 0; i < sizeof(occs); i++) {
        file = find_user_help_file(occs[i]);
        if(file && member_array(file, out) == -1)
            out += ({ file });
    }
    return out;
}

static int name_matches(string topic, string name) {
    string norm;
    string underscored;

    norm = lower_case(topic);
    if(norm == lower_case(name)) return 1;
    underscored = replace_string(lower_case(name), " ", "_");
    underscored = replace_string(underscored, "-", "_");
    if(norm == underscored) return 1;
    if(norm == replace_string(underscored, "_", "-")) return 1;
    /* Collapsed-key form: "cyberknight" vs official "Cyber-Knight". */
    if(squash_topic(norm) == squash_topic(name)) return 1;
    return 0;
}

static int is_alignment_topic(string topic) {
    string norm;

    norm = lower_case(topic);
    return member_array(norm, ({
        "principled", "scrupulous", "unprincipled", "anarchist",
        "miscreant", "diabolic", "aberrant"
    })) != -1;
}

static int is_skills_topic(string topic) {
    string norm;
    string *cats;
    string *names;
    int i;
    int j;

    norm = lower_case(topic);
    if(member_array(norm, ({
        "skills", "bionics", "abilities", "improve", "spells",
        "cast", "languages", "language", "pskills", "sskills", "magic",
        "deception"
    })) != -1) return 1;

    cats = (string *)RIFTS_SKILLS_D->query_skill_categories();
    for(i = 0; i < sizeof(cats); i++) {
        names = (string *)RIFTS_SKILLS_D->query_skills_by_category(cats[i]);
        for(j = 0; j < sizeof(names); j++)
            if(name_matches(topic, names[j])) return 1;
    }

    names = (string *)RIFTS_SPELLS_D->query_all_spells();
    for(i = 0; i < sizeof(names); i++)
        if(name_matches(topic, names[i])) return 1;

    return 0;
}

/* Psionic powers get their own category (psihelp) instead of being
 * buried in the general skills listing. */
static int is_psionics_topic(string topic) {
    string norm;
    string *names;
    int i;

    norm = lower_case(topic);
    if(member_array(norm, ({
        "psihelp", "psionics", "psi"
    })) != -1) return 1;

    names = (string *)RIFTS_PSIONICS_D->query_all_psionics();
    for(i = 0; i < sizeof(names); i++)
        if(name_matches(topic, names[i])) return 1;

    return 0;
}

static int is_combat_topic(string topic) {
    string norm;

    norm = lower_case(topic);
    return member_array(norm, ({
        "combat", "kill", "dodge", "parry", "flee", "stance", "vehicles",
        "weapons", "eject", "autododge", "autoparry", "savingthrows",
        "death", "reload", "unload", "wimpy", "hide", "search",
        "position", "rest", "wake", "movement"
    })) != -1;
}

static int is_comm_topic(string topic) {
    string norm;

    norm = lower_case(topic);
    return member_array(norm, ({
        "say", "ooc", "emote", "converse", "radio", "whisper", "channels",
        "introduce", "touch", "communication", "mail", "user_mail", "postal",
        "post"
    })) != -1;
}

static int is_systems_topic(string topic) {
    string norm;

    norm = lower_case(topic);
    return member_array(norm, ({
        "score", "advance", "levels", "experience", "clan", "quests",
        "getting-started", "account", "save", "stats", "help", "who",
        "finger", "eq", "worn", "inventory", "equipment", "currency",
        "reputation", "guild", "guilds", "faction", "mentors", "elders",
        "marriage", "shop", "terminal", "idling", "prompt", "brief", "sbar",
        "color", "colorize", "days", "months", "moons", "timezones",
        "syntax", "password", "lock", "look", "sirname"
    })) != -1;
}

static int is_race_topic(string topic, string first_line) {
    string *races;
    string lower_first;
    int i;

    if(topic == "races") return 1;
    lower_first = lower_case(first_line);
    if(strsrch(lower_first, "race:") != -1) return 1;
    if(strsrch(lower_first, "rcc:") != -1) return 1;
    if(strsrch(lower_first, "nightmare race") != -1) return 1;

    races = (string *)RIFTS_D->query_rifts_races();
    for(i = 0; i < sizeof(races); i++)
        if(name_matches(topic, races[i])) return 1;

    races = (string *)RACE_D->query_races();
    for(i = 0; i < sizeof(races); i++)
        if(name_matches(topic, races[i])) return 1;

    /* Collapsed-key filenames (firedragon, greenwoodfaerie, ...) still
       carry these words as a bare suffix; substring match covers both
       the old underscored and new collapsed forms. */
    if(strsrch(topic, "dragon") != -1) return 1;
    if(strsrch(topic, "giant") != -1) return 1;
    if(strsrch(topic, "faerie") != -1) return 1;
    if(strsrch(topic, "pixie") != -1) return 1;
    if(strsrch(topic, "sprite") != -1) return 1;
    if(strsrch(topic, "vampire") != -1) return 1;
    return member_array(lower_case(topic), ({
        "faerie", "changeling", "conservator", "scruvy", "pogtal", "artrell",
        "arches", "kataan", "titan", "gurgoyle", "equinoid", "ratling",
        "rahu-man", "coyle", "jotan", "shifter", "burster", "dogboy"
    })) != -1;
}

static int is_occ_topic(string topic, string first_line) {
    string *occs;
    string lower_first;
    int i;

    if(member_array(topic, ({
        "occs", "classes", "mage", "monk", "fighter", "cleric", "rogue"
    })) != -1) return 1;

    lower_first = lower_case(first_line);
    if(strsrch(lower_first, "occ:") != -1) return 1;
    if(strsrch(lower_first, "class:") != -1) return 1;

    occs = (string *)OCC_D->query_occs();
    for(i = 0; i < sizeof(occs); i++)
        if(name_matches(topic, occs[i])) return 1;

    if(name_matches(topic, "special forces (merc)")) return 1;
    if(name_matches(topic, "pirate (s.a.)")) return 1;
    if(name_matches(topic, "sailor (s.a.)")) return 1;
    if(name_matches(topic, "knight (europe)")) return 1;
    return 0;
}

static int classify_topic(string topic, string first_line) {
    if(is_alignment_topic(topic)) return HELP_SEC_ALIGN;
    if(is_race_topic(topic, first_line)) return HELP_SEC_RACES;
    if(is_occ_topic(topic, first_line)) return HELP_SEC_OCCS;
    if(is_psionics_topic(topic)) return HELP_SEC_PSIONICS;
    if(is_skills_topic(topic)) return HELP_SEC_SKILLS;
    if(is_combat_topic(topic)) return HELP_SEC_COMBAT;
    if(is_comm_topic(topic)) return HELP_SEC_COMM;
    if(is_systems_topic(topic)) return HELP_SEC_SYSTEMS;
    return HELP_SEC_COMMANDS;
}

/* Four columns, column width sized to the longest topic in THIS listing
 * plus a small gap (never a flat guess): short categories (alignments,
 * psihelp) print tight, content-heavy ones (skills, races) use the full
 * line without wasting padding on short entries. */
static void print_help_section(string title, string *topics) {
    string line;
    string disp;
    string *display;
    int i;
    int x;
    int maxi;
    int width;
    int longest;
    int len;

    if(!topics || !sizeof(topics)) return;
    topics = sort_array(topics, 1);
    display = ({});
    longest = 0;
    maxi = sizeof(topics);
    for(i = 0; i < maxi; i++) {
        disp = replace_string(topics[i], "_", " ");
        display += ({ disp });
        len = strlen(disp);
        if(len > longest) longest = len;
    }
    width = longest + 3;
    if(width < 18) width = 18;

    message("info", "\n" + title, this_player());
    message("info", " " + repeat_string("-", width * 4), this_player());
    i = 0;
    while(i < maxi) {
        line = "";
        x = i + 4;
        if(x > maxi) x = maxi;
        while(i < x) {
            line += arrange_string(display[i], width);
            i++;
        }
        message("info", " " + line, this_player());
    }
}

/* Top-level help: only the category names, not every topic. */
static void categories_display() {
    int staff;

    staff = creatorp(this_player()) || high_mortalp(this_player()) ||
        ambassadorp(this_player());
    message("info", "\n  ===================== " + mud_name() +
        " Help =====================\n", this_player());
    message("info", "  races            Playable races and RCCs",
        this_player());
    message("info", "  classes          Occupational Character Classes (OCCs)",
        this_player());
    message("info", "  skills           Skills, spells, abilities",
        this_player());
    message("info", "  psihelp          Psionic powers and psi abilities",
        this_player());
    message("info", "  combat           Combat, movement, and survival",
        this_player());
    message("info", "  communication    Says, channels, radio, mail, social",
        this_player());
    message("info", "  systems          Score, advancement, game systems",
        this_player());
    message("info", "  commands         Other player commands",
        this_player());
    message("info", "  alignments       The Palladium alignments",
        this_player());
    if(staff)
        message("info", "  staff            Staff and wizard help",
            this_player());
    message("info", "\n  Type help <category> to list the topics in a category.",
        this_player());
    message("info", "  Type help <topic> for details on a specific topic.",
        this_player());
    message("info", "  Type help index to see every topic in one list.",
        this_player());
    message("info", "  =====================================================",
        this_player());
}

static string category_title(int sec) {
    switch(sec) {
    case HELP_SEC_RACES:    return "RACES";
    case HELP_SEC_OCCS:     return "CLASSES / OCCs";
    case HELP_SEC_SKILLS:   return "SKILLS & ABILITIES";
    case HELP_SEC_PSIONICS: return "PSIONICS";
    case HELP_SEC_COMBAT:   return "COMBAT & MOVEMENT";
    case HELP_SEC_COMM:     return "COMMUNICATION & SOCIAL";
    case HELP_SEC_SYSTEMS:  return "SYSTEMS";
    case HELP_SEC_COMMANDS: return "COMMANDS";
    case HELP_SEC_ALIGN:    return "ALIGNMENTS";
    case HELP_SEC_STAFF:    return "STAFF / WIZARD";
    }
    return "HELP";
}

/* Category keywords that are also real topic files: the category listing
 * wins the bare word, so tell the reader how to open the file itself. */
static string *category_shadowed_files(int sec) {
    string *kws;
    string *out;
    int i;

    switch(sec) {
    case HELP_SEC_RACES:    kws = ({ "races" });            break;
    case HELP_SEC_OCCS:     kws = ({ "occs", "classes" });  break;
    case HELP_SEC_SKILLS:   kws = ({ "skills" });           break;
    case HELP_SEC_PSIONICS: kws = ({ "psihelp" });          break;
    case HELP_SEC_COMBAT:   kws = ({ "combat" });           break;
    case HELP_SEC_COMM:     kws = ({ "communication" });    break;
    case HELP_SEC_COMMANDS: kws = ({ "commands" });         break;
    default:                kws = ({});                     break;
    }
    out = ({});
    for(i = 0; i < sizeof(kws); i++) {
        if(file_exists(DIR_USER_HELP + "/" + kws[i]))
            out += ({ kws[i] });
    }
    return out;
}

static void category_footer(int sec) {
    string *shadow;
    int i;

    message("info", "\n  Type help <topic> for details on any topic above.",
        this_player());
    shadow = category_shadowed_files(sec);
    for(i = 0; i < sizeof(shadow); i++) {
        message("info", "  For the '" + shadow[i] +
            "' overview page itself, type: help " + shadow[i] + " overview",
            this_player());
    }
    message("info", "  Type help with no argument to list the categories.",
        this_player());
}

/* List the topics belonging to one category. */
static void category_display(int sec) {
    string *topics;
    string *sec_races;
    string *sec_occs;
    string *all;
    string *staff_topics;
    string path;
    string first_line;
    int i;

    if(sec == HELP_SEC_STAFF) {
        if(!creatorp(this_player()) && !high_mortalp(this_player()) &&
           !ambassadorp(this_player())) {
            message("help", "Staff help is not available to you.",
                this_player());
            return;
        }
        topics = ({});
        if(creatorp(this_player())) {
            staff_topics = get_dir(DIR_WIZ_HELP + "/");
            if(staff_topics && sizeof(staff_topics)) topics += staff_topics;
            staff_topics = get_dir(DIR_CREATOR_HELP + "/");
            if(staff_topics && sizeof(staff_topics)) topics += staff_topics;
        }
        if(high_mortalp(this_player()) || creatorp(this_player())) {
            staff_topics = get_dir(DIR_HM_HELP + "/");
            if(staff_topics && sizeof(staff_topics)) topics += staff_topics;
        }
        if(ambassadorp(this_player()) || creatorp(this_player())) {
            staff_topics = get_dir(DIR_AMBASSADOR_HELP + "/");
            if(staff_topics && sizeof(staff_topics)) topics += staff_topics;
        }
        print_help_section(category_title(sec), topics);
        category_footer(sec);
        if(admin_wizp(this_player())) {
            message("info", "\n  Admin commands (setrole, warmboot, sever, "
                "etc.) are not listed above; each has its own\n  help "
                "page reachable directly: help <command>. For a one-line "
                "index of every admin command,\n  take the admin cheat "
                "sheet from the staff supplies chest and read it.",
                this_player());
        }
        return;
    }
    if(sec == HELP_SEC_RACES) {
        print_help_section(category_title(sec), index_race_topics());
        category_footer(sec);
        return;
    }
    if(sec == HELP_SEC_OCCS) {
        print_help_section(category_title(sec), index_occ_topics());
        category_footer(sec);
        return;
    }
    sec_races = index_race_topics();
    sec_occs = index_occ_topics();
    topics = ({});
    all = dedupe_user_topics(get_dir(DIR_USER_HELP + "/"));
    for(i = 0; i < sizeof(all); i++) {
        if(member_array(all[i], sec_races) != -1) continue;
        if(member_array(all[i], sec_occs) != -1) continue;
        path = DIR_USER_HELP + "/" + all[i];
        first_line = read_first_line(path);
        if(classify_topic(all[i], first_line) != sec) continue;
        topics += ({ all[i] });
    }
    if(!sizeof(topics)) {
        message("help", "No help topics found in that category.",
            this_player());
        return;
    }
    print_help_section(category_title(sec), topics);
    category_footer(sec);
}

static void flat_help_display() {
    string *topics;
    string *sec_races;
    string *sec_occs;
    string *sec_skills;
    string *sec_psionics;
    string *sec_combat;
    string *sec_comm;
    string *sec_systems;
    string *sec_commands;
    string *sec_align;
    string *sec_staff;
    string path;
    string first_line;
    string *staff_topics;
    int i;
    int sec;

    sec_races = index_race_topics();
    sec_occs = index_occ_topics();
    sec_skills = ({});
    sec_psionics = ({});
    sec_combat = ({});
    sec_comm = ({});
    sec_systems = ({});
    sec_commands = ({});
    sec_align = ({});
    sec_staff = ({});

    /* Remaining user help: skip race/OCC files already listed, aliases,
     * junk, and legacy fantasy race stubs that are not playable. */
    topics = dedupe_user_topics(get_dir(DIR_USER_HELP + "/"));
    for(i = 0; i < sizeof(topics); i++) {
        if(member_array(topics[i], sec_races) != -1) continue;
        if(member_array(topics[i], sec_occs) != -1) continue;
        path = DIR_USER_HELP + "/" + topics[i];
        first_line = read_first_line(path);
        sec = classify_topic(topics[i], first_line);
        /* Drop leftover race/OCC misfiles (legacy NM3 fantasy, monsters). */
        if(sec == HELP_SEC_RACES || sec == HELP_SEC_OCCS) continue;
        switch(sec) {
        case HELP_SEC_SKILLS:   sec_skills   += ({ topics[i] }); break;
        case HELP_SEC_PSIONICS: sec_psionics += ({ topics[i] }); break;
        case HELP_SEC_COMBAT:   sec_combat   += ({ topics[i] }); break;
        case HELP_SEC_COMM:     sec_comm     += ({ topics[i] }); break;
        case HELP_SEC_SYSTEMS:  sec_systems  += ({ topics[i] }); break;
        case HELP_SEC_ALIGN:    sec_align    += ({ topics[i] }); break;
        default:                sec_commands += ({ topics[i] }); break;
        }
    }

    if(creatorp(this_player())) {
        staff_topics = get_dir(DIR_WIZ_HELP + "/");
        if(staff_topics && sizeof(staff_topics))
            sec_staff += staff_topics;
        staff_topics = get_dir(DIR_CREATOR_HELP + "/");
        if(staff_topics && sizeof(staff_topics))
            sec_staff += staff_topics;
    }
    if(high_mortalp(this_player()) || creatorp(this_player())) {
        staff_topics = get_dir(DIR_HM_HELP + "/");
        if(staff_topics && sizeof(staff_topics))
            sec_staff += staff_topics;
    }
    if(ambassadorp(this_player()) || creatorp(this_player())) {
        staff_topics = get_dir(DIR_AMBASSADOR_HELP + "/");
        if(staff_topics && sizeof(staff_topics))
            sec_staff += staff_topics;
    }

    message("info", "\n  ===================== " + mud_name() + " Help =====================",
        this_player());
    print_help_section("RACES", sec_races);
    print_help_section("CLASSES / OCCs", sec_occs);
    print_help_section("SKILLS & ABILITIES", sec_skills);
    print_help_section("PSIONICS", sec_psionics);
    print_help_section("COMBAT & MOVEMENT", sec_combat);
    print_help_section("COMMUNICATION & SOCIAL", sec_comm);
    print_help_section("SYSTEMS", sec_systems);
    print_help_section("COMMANDS", sec_commands);
    print_help_section("ALIGNMENTS", sec_align);
    if(sizeof(sec_staff))
        print_help_section("STAFF / WIZARD", sec_staff);
    message("info", "\n  =====================================================", this_player());
    message("info", "  Type help <topic> for details on any topic above.",
        this_player());
}

static void suggest_similar_topics(string topic) {
    string *topics;
    string *matches;
    string lower;
    int i;

    /* Squash both sides so spaced input still finds underscored files. */
    lower = squash_topic(topic);
    topics = dedupe_user_topics(get_dir(DIR_USER_HELP + "/"));
    matches = ({});
    for(i = 0; i < sizeof(topics); i++) {
        if(strsrch(squash_topic(topics[i]), lower) != -1)
            matches += ({ topics[i] });
    }
    if(!sizeof(matches)) return;
    if(sizeof(matches) > 8) matches = matches[0..7];
    /* Filenames use underscores; players must only ever see spaces. */
    for(i = 0; i < sizeof(matches); i++)
        matches[i] = replace_string(matches[i], "_", " ");
    message("help", "Similar topics: " + implode(matches, ", "), this_player());
}

static string *query_categories() {
    string *tmp;

    tmp = ({ "*player general", "*player commands", "*abilities" });
    if((string)this_player()->query_guild()) tmp += ({ "*guild commands" });
    if(high_mortalp(this_player()) || creatorp(this_player()))
      tmp += ({ "*high mortal general", "*high mortal commands" });
    if(ambassadorp(this_player()))
      tmp += ({ "*ambassador general", "*ambassador commands" });
    if(creatorp(this_player())) 
      tmp += ({ "*creator general", "*creator commands", "*wizard staff" });
    if(archp(this_player())) tmp += ({ "*admin commands" }); 
    return tmp;
  }

void cmd_help(string topic, string category) {
    object ob;
    string *tmp;
    int i;

    if(!category) topic = normalize_help_topic(topic);
    if(category) {
        if(member_array(category, query_categories()) == -1) {
            message("help", sprintf("Category %s not available.",
              category), this_player());
	  }
        else if(!find_help(topic, category)) {
            message("help", sprintf("The help topic %s does not exist in "
              "the category %s.", replace_string(topic, "_", " "),
              category), this_player());
	  }
        return;
      }
    else {
        /* Prefer player help files over command stubs so topics like
         * "color" show the full doc, not only the command synopsis. */
        if(find_help(topic, "*player general")) return;
        i = sizeof(tmp = query_categories());
        while(i--) {
            if(tmp[i] == "*player general") continue;
            if(find_help(topic, tmp[i])) return;
        }
        if((ob = present(topic, this_player())) &&
          function_exists("help", ob)) {
            ob->help();
            return;
	  }
        else if((ob = present(topic, environment(this_player()))) &&
          function_exists("help", ob)) {
            ob->help();
            return;
	  }
        message("help", sprintf("No help available for the topic %s.",
          replace_string(topic, "_", " ")), this_player());
        suggest_similar_topics(topic);
        return;
      }
  }

static int find_help(string topic, string category) {
    object ob;
    string str;
    string tfile;
    string disp;
    mixed tmp;

    if(category == "*player general")
        topic = normalize_help_topic(topic);
    /* Command files are named _<verb>.c with underscores; accept the
       spaced form players see in listings by underscoring for lookup. */
    tfile = replace_string(topic, " ", "_");

    switch(category) {
    case "*player general":
        if(!file_exists(tmp = DIR_USER_HELP+"/"+topic)) return 0;
        break;
    case "*player commands":
        if(!file_exists(tmp = DIR_MORTAL_CMDS+"/_"+tfile+".c") &&
          !file_exists(tmp = DIR_SECURE_MORTAL_CMDS+"/_"+tfile+".c")) return 0;
        if(!(ob = load_object(tmp)) || !function_exists("help", ob))
          return 0;
        tmp = ob;
        break;
    case "*abilities":
        if(!file_exists(tmp = DIR_CLASS_CMDS+"/_"+tfile+".c")) return 0;
        if(!(ob = load_object(tmp)) || !function_exists("help", ob))
          return 0;
        tmp = ob;
        break;
    case "*creator general":
        if(!file_exists(tmp = DIR_CREATOR_HELP+"/"+topic) &&
          !file_exists(tmp = DIR_CREATOR_HELP+"/"+
            replace_string(topic, " ", "_"))) return 0;
        break;
    case "*wizard staff":
        if(!file_exists(tmp = DIR_WIZ_HELP+"/"+topic) &&
          !file_exists(tmp = DIR_WIZ_HELP+"/"+
            replace_string(topic, " ", "_"))) return 0;
        break;
    case "*creator commands":
        if(file_exists(tmp = DIR_CREATOR_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;
        else if(file_exists(tmp = DIR_SYSTEM_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;
        else if(file_exists(tmp = DIR_AMBASSADOR_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;
        else if(file_exists(tmp = DIR_SECURE_AMBASSADOR_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;
        else if(file_exists(tmp = DIR_SECURE_CREATOR_CMDS+"/_"+tfile+".c") &&
         (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;     
        else return 0;
        break;
    case "*ambassador general":
        if(file_exists(tmp = DIR_AMBASSADOR_CMDS+"/_"+tfile+".c") &&       
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;     
        else if(file_exists(tmp = DIR_SECURE_AMBASSADOR_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;     
        else return 0;
        break; 
    case "*ambassador commands":
        if(file_exists(tmp = DIR_AMBASSADOR_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;
        else if(file_exists(tmp = DIR_SYSTEM_CMDS+"/_"+tfile+".c") &&
          (ob = load_object(tmp)) && function_exists("help", ob)) tmp = ob;
        else return 0;
        break;
    case "*high mortal general":
        if(!file_exists(tmp = DIR_HM_HELP+"/"+topic) &&
          !file_exists(tmp = DIR_HM_HELP+"/"+
            replace_string(topic, " ", "_"))) return 0;
        break;
    case "*high mortal commands":  
        if(!file_exists(tmp = DIR_HM_CMDS+"/_"+tfile+".c")) return 0;
        if(!(ob = load_object(tmp)) || !function_exists("help", ob))
          return 0;
        tmp = ob;
        break;
    case "*guild commands":
        if(!file_exists(tmp = sprintf("%s/%s/_%s.c", DIR_GUILD_CMDS,
          (string)this_player()->query_guild(), tfile))) return 0;
        if(!(ob = load_object(tmp)) || !function_exists("help", ob))
          return 0;
        tmp = ob;
        break;
    case "*admin commands":
        if(!archp(this_player())) return 0;
        if(!file_exists(tmp = DIR_ADMIN_CMDS+"/_"+tfile+".c") &&
          !file_exists(tmp = DIR_SECURE_ADMIN_CMDS+"/_"+tfile+".c")) return 0;
        if(!(ob = load_object(tmp)) || !function_exists("help", ob))
          return 0;
        tmp = ob;
        break; 
      }
    /* topic may be an on-disk filename by now; never echo underscores. */
    disp = replace_string(topic, "_", " ");
    if(!(int)this_player()->query_env("NO_CLEAR"))
    message("info", sprintf("\n%%^INITTERM%%^Topic: %%^GREEN%%^%s"
      "%%^RESET%%^  \t%s System Help \tCategory: %%^GREEN%%^%s\n",
      disp, mud_name(), category), this_player());
    else
    message("info", sprintf("\nTopic: %%^GREEN%%^%s"
      "%%^RESET%%^  \t%s System Help \tCategory: %%^GREEN%%^%s\n",
      disp, mud_name(), category), this_player());
    if(objectp(tmp)) {
        tmp->help();
    }
    else this_player()->more(tmp, "help");
    return 1;
  }
