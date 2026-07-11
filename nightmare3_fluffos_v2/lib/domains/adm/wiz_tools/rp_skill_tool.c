// /domains/adm/wiz_tools/rp_skill_tool.c
// RP Wizard Skill Tool -- grants/removes skills and languages for players.
// Carried in inventory. Usable by creatorp() rank (rp_wiz and above).

#include <std.h>
#include <daemons.h>

inherit OBJECT;

static private object selected_player;
static private string pending_action;
static private string *skill_page;
static private int skill_page_num;

private void show_skill_page();
private void show_menu();
void do_view_skills();
void do_grant_skill_entry();
void do_remove_skill_entry();
void do_grant_language_entry();
void do_remove_language_entry();
void do_view_queue();

void create() {
    ::create();
    set_name("rp skill tool");
    set_id( ({ "rp skill tool", "skill tool", "rp_skill_tool",
               "rp-wiz skill tool", "tool", "rp tool" }) );
    set_short("an RP-Wiz skill tool");
    set_long(
        "A sleek silver datapad imprinted with skill management software.\n"
        "Allows RP Wizards to grant, view, and remove skills and languages\n"
        "from players. For RP Wizard use only. Type: tool or rptool");
    set_mass(100);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
    set_invis(1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_use_tool", "tool");
    add_action("cmd_use_tool", "rptool");
}

private void show_menu() {
    string pname;
    pname = (selected_player && objectp(selected_player))
          ? (string)selected_player->query_cap_name()
          : "(none)";
    write("\n=== RP SKILL TOOL === [Player: " + pname + "]");
    write(" 1. Select a player");
    write(" 2. View player skills");
    write(" 3. Grant a skill");
    write(" 4. Remove a skill");
    write(" 5. Grant a language");
    write(" 6. Remove a language");
    write(" 7. View skill request queue");
    write(" 0. Exit");
    write("Choice: ");
}

int cmd_use_tool(string str) {
    if(!admin_wizp(this_player()) && !rp_wizp(this_player()) &&
       !domain_wizp(this_player())) {
        write("The tool does not respond to you.\n");
        return 1;
    }
    if(!creatorp(this_player())) {
        write("The tool does not respond to you.\n");
        return 1;
    }
    pending_action = "";
    skill_page = ({});
    skill_page_num = 0;
    show_menu();
    input_to("handle_choice");
    return 1;
}

void handle_choice(string str) {
    int choice;
    if(!str || !sizeof(str)) {
        write("Invalid choice.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    switch(choice) {
    case 0: write("RP Skill Tool closed.\n"); return;
    case 1: write("Player name: "); input_to("select_player"); return;
    case 2: do_view_skills(); return;
    case 3: do_grant_skill_entry(); return;
    case 4: do_remove_skill_entry(); return;
    case 5: do_grant_language_entry(); return;
    case 6: do_remove_language_entry(); return;
    case 7: do_view_queue(); return;
    default:
        write("Invalid option.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
}

// ── Option 1: Select player ──────────────────────────────────────────────────

void select_player(string name) {
    object ob;
    if(!name || !sizeof(name)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    ob = find_player(lower_case(name));
    if(!ob) {
        write("Player '" + capitalize(name) + "' not found online.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    selected_player = ob;
    write("Selected: " + (string)ob->query_cap_name() + "\n");
    show_menu();
    input_to("handle_choice");
}

// ── Option 2: View skills ────────────────────────────────────────────────────

void do_view_skills() {
    string *slist;
    mapping langs;
    string *lang_keys;
    string *tattoos;
    int i, pct;

    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    write("\nSkills for " + (string)selected_player->query_cap_name() + ":");

    langs = (mapping)LANGUAGE_D->query_known(selected_player);
    lang_keys = keys(langs);
    if(sizeof(lang_keys)) {
        write(" [Languages]");
        for(i = 0; i < sizeof(lang_keys); i++) {
            pct = langs[lang_keys[i]];
            if(pct >= 98)
                write(sprintf(" %-34s %s [native]", lang_keys[i], "98%"));
            else
                write(sprintf(" %-34s %d%%", lang_keys[i], pct));
        }
    }

    slist = (string *)RIFTS_SKILLS_D->query_player_skills(selected_player);
    if(!slist) slist = ({});
    slist -= ({ "melee", "attack", "defense" });
    if(sizeof(slist)) {
        write(" [Skills]");
        for(i = 0; i < sizeof(slist); i++) {
            pct = (int)selected_player->query_base_skill(slist[i]);
            write(sprintf(" %-34s %d%%", slist[i], pct));
        }
    }

    tattoos = (string *)selected_player->query_property("tattoos");
    if(tattoos && sizeof(tattoos)) {
        write(" [Tattoos]");
        for(i = 0; i < sizeof(tattoos); i++)
            write(" " + tattoos[i]);
    }

    write("");
    show_menu();
    input_to("handle_choice");
}

// ── Option 3: Grant a skill ──────────────────────────────────────────────────

void do_grant_skill_entry() {
    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    skill_page = ({
        "acrobatics", "advanced electronics", "basic electronics",
        "biology", "body building", "boxing", "camouflage",
        "chemistry", "climbing", "computer operation",
        "computer programming", "concealment", "demolitions",
        "detect ambush", "detect concealment", "disguise",
        "electronic countermeasures", "first aid", "fishing",
        "fly", "forgery", "gambling", "gymnastics", "holistic medicine",
        "horsemanship", "hunting", "impersonation", "intelligence",
        "interrogation", "land navigation", "lore demons and monsters",
        "lore magic", "lore psychic", "lore rifts and dimensions",
        "lore spirits", "mechanical engineer", "medical doctor",
        "meditation", "military etiquette", "navigation", "paramedic",
        "pathology", "pick locks", "pick pockets", "pilot automobile",
        "pilot boat", "pilot glitter boy", "pilot hovercraft",
        "pilot hovercycle", "pilot jet pack", "pilot power armor",
        "pilot robot", "pilot submersible", "prowl",
        "radio basic", "read sensory equipment", "research",
        "robot mechanics", "running", "safe-cracking",
        "streetwise", "surveillance", "swimming", "tactics",
        "tracking", "weapon systems", "weapons engineer",
        "wilderness survival", "wp archery", "wp blunt",
        "wp energy pistol", "wp energy rifle", "wp heavy weapons",
        "wp knife", "wp rifle", "wp sword", "wp vibroblade"
    });
    skill_page_num = 0;
    pending_action = "grant_skill";

    write("Available skills (type number or skill name, 'next'/'prev' to page):");
    show_skill_page();
    input_to("pick_skill");
}

private void show_skill_page() {
    int start, end_idx, i, total_pages;
    start = skill_page_num * 20;
    end_idx = start + 20;
    if(end_idx > sizeof(skill_page)) end_idx = sizeof(skill_page);
    total_pages = (sizeof(skill_page) + 19) / 20;
    write(sprintf("--- Page %d/%d ---", skill_page_num + 1, total_pages));
    for(i = start; i < end_idx; i++)
        write(sprintf(" %2d. %s", i + 1, skill_page[i]));
    write("Skill (number/name/next/prev): ");
}

void pick_skill(string str) {
    int choice;
    string skill_name;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    if(str == "next") {
        if((skill_page_num + 1) * 20 < sizeof(skill_page)) skill_page_num++;
        show_skill_page();
        input_to("pick_skill");
        return;
    }
    if(str == "prev") {
        if(skill_page_num > 0) skill_page_num--;
        show_skill_page();
        input_to("pick_skill");
        return;
    }
    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(skill_page))
        skill_name = skill_page[choice - 1];
    else
        skill_name = lower_case(str);

    if(!skill_name || !sizeof(skill_name)) {
        write("Invalid skill.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    pending_action = "grant_skill:" + skill_name;
    write("Percentage for " + skill_name + " (1-98): ");
    input_to("apply_skill_pct");
}

void apply_skill_pct(string str) {
    int pct;
    string skill_name;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    pct = to_int(str);
    if(pct < 1 || pct > 98) {
        write("Percentage must be 1-98.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    sscanf(pending_action, "grant_skill:%s", skill_name);
    if(!skill_name || !sizeof(skill_name)) {
        write("Error: no skill selected.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    RIFTS_SKILLS_D->grant_skill(selected_player, skill_name, pct);
    RIFTS_SKILLS_D->set_skill_pct(selected_player, skill_name, pct);

    write("Granted " + skill_name + " at " + pct + "% to " +
          (string)selected_player->query_cap_name() + ".\n");
    tell_object(selected_player,
        "An RP wizard has granted you: " + skill_name +
        " (" + pct + "%)\n");

    catch(log_file("rp_wizard/skills",
        (string)this_player()->query_name() + " granted '" + skill_name +
        "' at " + pct + "% to " + (string)selected_player->query_name() +
        ": " + ctime(time()) + "\n"));

    show_menu();
    input_to("handle_choice");
}

// ── Option 4: Remove a skill ─────────────────────────────────────────────────

void do_remove_skill_entry() {
    string *slist;
    int i, pct;

    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    slist = (string *)RIFTS_SKILLS_D->query_player_skills(selected_player);
    if(!slist) slist = ({});
    slist -= ({ "melee", "attack", "defense" });

    if(!sizeof(slist)) {
        write((string)selected_player->query_cap_name() + " has no skills.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    skill_page = slist;
    skill_page_num = 0;
    write("Current skills for " + (string)selected_player->query_cap_name() + ":");
    for(i = 0; i < sizeof(slist); i++) {
        pct = (int)selected_player->query_base_skill(slist[i]);
        write(sprintf(" %2d. %-34s %d%%", i + 1, slist[i], pct));
    }
    write("Remove which skill (number/name): ");
    input_to("pick_remove_skill");
}

void pick_remove_skill(string str) {
    int choice;
    string skill_name;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(skill_page))
        skill_name = skill_page[choice - 1];
    else
        skill_name = lower_case(str);

    if(!skill_name || !sizeof(skill_name)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    RIFTS_SKILLS_D->remove_skill(selected_player, skill_name);
    write("Removed " + skill_name + " from " +
          (string)selected_player->query_cap_name() + ".\n");
    tell_object(selected_player,
        "An RP wizard has removed: " + skill_name + "\n");

    catch(log_file("rp_wizard/skills",
        (string)this_player()->query_name() + " removed '" + skill_name +
        "' from " + (string)selected_player->query_name() +
        ": " + ctime(time()) + "\n"));

    show_menu();
    input_to("handle_choice");
}

// ── Option 5: Grant a language ───────────────────────────────────────────────

void do_grant_language_entry() {
    string *langs;
    int i;

    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    langs = (string *)LANGUAGE_D->query_all_languages();
    skill_page = langs;
    skill_page_num = 0;

    write("Available languages:");
    for(i = 0; i < sizeof(langs); i++)
        write(sprintf(" %2d. %s", i + 1, langs[i]));
    write("Language (number/name): ");
    input_to("pick_grant_language");
}

void pick_grant_language(string str) {
    int choice;
    string lang_name;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(skill_page))
        lang_name = skill_page[choice - 1];
    else
        lang_name = capitalize(lower_case(str));

    if(!lang_name || !sizeof(lang_name)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    pending_action = "grant_language:" + lang_name;
    write("Percentage for " + lang_name + " (1-98): ");
    input_to("apply_language_pct");
}

void apply_language_pct(string str) {
    int pct;
    string lang_name;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    pct = to_int(str);
    if(pct < 1 || pct > 98) {
        write("Percentage must be 1-98.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    sscanf(pending_action, "grant_language:%s", lang_name);
    if(!lang_name || !sizeof(lang_name)) {
        write("Error: no language selected.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    LANGUAGE_D->grant_language(selected_player, lang_name, pct);
    write("Granted " + lang_name + " at " + pct + "% to " +
          (string)selected_player->query_cap_name() + ".\n");
    tell_object(selected_player,
        "An RP wizard has granted you the language: " + lang_name +
        " (" + pct + "%)\n");

    catch(log_file("rp_wizard/skills",
        (string)this_player()->query_name() + " granted language '" + lang_name +
        "' at " + pct + "% to " + (string)selected_player->query_name() +
        ": " + ctime(time()) + "\n"));

    show_menu();
    input_to("handle_choice");
}

// ── Option 6: Remove a language ──────────────────────────────────────────────

void do_remove_language_entry() {
    mapping langs;
    string *lang_keys;
    int i, pct;

    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    langs = (mapping)LANGUAGE_D->query_known(selected_player);
    lang_keys = keys(langs);

    if(!sizeof(lang_keys)) {
        write((string)selected_player->query_cap_name() + " knows no languages.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    skill_page = lang_keys;
    skill_page_num = 0;

    write("Current languages for " + (string)selected_player->query_cap_name() + ":");
    for(i = 0; i < sizeof(lang_keys); i++) {
        pct = langs[lang_keys[i]];
        if(pct >= 98)
            write(sprintf(" %2d. %-30s 98%% [native]", i + 1, lang_keys[i]));
        else
            write(sprintf(" %2d. %-30s %d%%", i + 1, lang_keys[i], pct));
    }
    write("Remove which language (number/name): ");
    input_to("pick_remove_language");
}

void pick_remove_language(string str) {
    int choice;
    string lang_name;
    mapping langs;
    string *ks;
    string *parts;
    string new_env;
    int pct;
    int i;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(skill_page))
        lang_name = skill_page[choice - 1];
    else
        lang_name = capitalize(lower_case(str));

    if(!lang_name || !sizeof(lang_name)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    langs = (mapping)LANGUAGE_D->query_known(selected_player);
    pct = langs[lang_name];
    if(pct >= 98) {
        write("Cannot remove " + lang_name + " -- it is a native language.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    ks = keys(langs);
    parts = ({});
    for(i = 0; i < sizeof(ks); i++) {
        if(ks[i] == lang_name) continue;
        parts += ({ ks[i] + ":" + langs[ks[i]] });
    }
    new_env = implode(parts, ",");
    selected_player->setenv("rifts_languages", new_env);

    write("Removed " + lang_name + " from " +
          (string)selected_player->query_cap_name() + ".\n");
    tell_object(selected_player,
        "An RP wizard has removed: " + lang_name + "\n");

    catch(log_file("rp_wizard/skills",
        (string)this_player()->query_name() + " removed language '" + lang_name +
        "' from " + (string)selected_player->query_name() +
        ": " + ctime(time()) + "\n"));

    show_menu();
    input_to("handle_choice");
}

// ── Option 7: View skill request queue ───────────────────────────────────────

void do_view_queue() {
    mapping all_reqs;
    string *pnames;
    mapping req;
    int i, age_mins;

    all_reqs = (mapping)SKILL_REQUEST_D->get_all_requests();
    pnames = keys(all_reqs);

    if(!sizeof(pnames)) {
        write("No pending skill requests.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    skill_page = pnames;
    skill_page_num = 0;

    write("=== Pending Skill Requests ===");
    for(i = 0; i < sizeof(pnames); i++) {
        req = all_reqs[pnames[i]];
        age_mins = (time() - (int)req["time"]) / 60;
        write(sprintf(" %d. %s requests: %s",
            i + 1,
            (string)req["cap_name"],
            (string)req["text"]));
        write(sprintf(" Submitted: %d minute%s ago",
            age_mins, (age_mins == 1 ? "" : "s")));
    }
    write("\nType request number to load player, or 0 to return: ");
    pending_action = "queue_select";
    input_to("queue_pick");
}

void queue_pick(string str) {
    int choice;
    string pname;
    object ob;
    mapping req;

    if(!str || str == "0") {
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    if(choice < 1 || choice > sizeof(skill_page)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    pname = skill_page[choice - 1];
    req = (mapping)SKILL_REQUEST_D->get_request(pname);
    ob = find_player(pname);

    if(ob) {
        selected_player = ob;
        write("Selected: " + (string)ob->query_cap_name() + "\n");
    } else {
        write("Note: " + capitalize(pname) + " is not currently online.\n");
    }

    if(req)
        write("Their request: " + (string)req["text"] + "\n");

    SKILL_REQUEST_D->clear_request(pname);
    write("Request cleared from queue.\n");
    show_menu();
    input_to("handle_choice");
}
