/* /domains/Praxis/npcs/chargen_guide.c
   Gamemaster NPC: conversational walkthrough for Rifts chargen. */

#include <std.h>

inherit MONSTER;

private void tell_player(object tp, string msg) {
    if(!tp || !objectp(tp)) return;
    tell_object(tp, "The Gamemaster says: " + msg);
}

private string query_creation_step(object tp) {
    string step;

    if(!tp) return "";
    step = (string)tp->getenv("creation_step");
    if(!step || !sizeof(step)) return "region";
    return step;
}

private void tell_current_step(object tp) {
    string step;
    string race;
    string occ;

    step = query_creation_step(tp);
    race = (string)tp->query_race();
    occ = (string)tp->getenv("rifts_occ");
    switch(step) {
    case "region":
        tell_player(tp,
            "First, choose your starting zone.\n"
            "  region americas   (Chi-Town area)\n"
            "  region atlantis   (Splynn / Atlantis)\n"
            "  region europe     (New Camelot / NGR)\n");
        break;
    case "stats":
        tell_player(tp,
            "Roll your attributes or pick a fixed array.\n"
            "  roll              random 3d6 per stat\n"
            "  pick fixed        use a balanced array\n"
            "  reroll            try again if rolls are allowed\n");
        break;
    case "race":
        tell_player(tp,
            "Pick a playable race.\n"
            "  read list         show races\n"
            "  pick <race>       choose your race\n");
        break;
    case "alignment":
        tell_player(tp,
            "Choose a Palladium alignment.\n"
            "  read list         show alignments\n"
            "  pick <alignment>  or: alignment <name>\n");
        break;
    case "occ":
        tell_player(tp,
            "Choose an Occupational Character Class (OCC).\n"
            "  read occs         list classes for your race\n"
            "  pick <occ>        choose your class\n"
            "  no occ            stay without a class if allowed\n");
        break;
    case "elective":
        tell_player(tp,
            "Pick your elective skills from the categories shown.\n"
            "  skills            list categories\n"
            "  pick <skill>      choose each skill\n");
        break;
    case "secondary":
        tell_player(tp,
            "Pick your secondary skills.\n"
            "  skills            list options\n"
            "  pick <skill>      choose each skill\n");
        break;
    case "done":
        tell_player(tp,
            "Creation is complete. Type 'score' and 'eq' to review your\n"
            "character. Welcome to the Rifts.\n");
        break;
    default:
        tell_player(tp,
            "Say 'help creation' for the full sequence, or 'status' for\n"
            "where you are now.\n");
        break;
    }
    if(race && sizeof(race))
        tell_player(tp, "Race so far: " + race + ".\n");
    if(occ && sizeof(occ) && occ != "none")
        tell_player(tp, "OCC so far: " + occ + ".\n");
}

void create() {
    ::create();
    set_name("Gamemaster");
    set_id(({ "gamemaster", "guide", "gm", "host" }));
    set_short("a Rifts Gamemaster");
    set_long(
        "A calm figure in Coalition-surplus gear with a data slate.\n"
        "They guide new arrivals through character creation step by step.\n"
        "Say 'help creation', 'status', 'what next', or ask about race,\n"
        "OCC, alignment, or skills.");
    set_level(1);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(0);
    set_property("position_str", "waits patiently with a data slate.");
}

void init() {
    if(this_player() && living(this_player()) && interactive(this_player())) {
        remove_call_out("greet");
        call_out("greet", 1);
    }
}

void greet() {
    object tp;

    if(!environment(this_object())) return;
    tp = this_player();
    if(!tp) return;
    tell_player(tp,
        "Welcome to the void. I can walk you through creation one step\n"
        "at a time. Say 'status' or 'what next' for your current step.\n");
    tell_current_step(tp);
}

void catch_tell(string str) {
    object tp;
    string low;
    string a;
    string b;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) return;
    tp = this_player();
    if(!tp) return;
    low = lower_case(b);

    if(strsrch(low, "status") != -1 || strsrch(low, "what next") != -1 ||
       strsrch(low, "where am i") != -1) {
        tell_current_step(tp);
        return;
    }
    if(strsrch(low, "help creation") != -1 || strsrch(low, "help chargen") != -1) {
        tell_player(tp,
            "Full creation sequence:\n"
            "  1. region americas|atlantis|europe\n"
            "  2. roll or pick fixed attributes\n"
            "  3. read list, then pick <race>\n"
            "  4. read list, then pick <alignment>\n"
            "  5. read occs, then pick <occ>\n"
            "  6. pick elective skills (skills command)\n"
            "  7. pick secondary skills\n"
            "  8. finish - gear and ID card arrive automatically\n\n"
            "Say 'status' at any time for the step you are on now.\n");
        return;
    }
    if(strsrch(low, "help race") != -1) {
        tell_player(tp,
            "Type 'read list' to see races, then 'pick <race name>'.\n"
            "Some races restrict which OCCs you may take.\n");
        return;
    }
    if(strsrch(low, "help occ") != -1 || strsrch(low, "help class") != -1) {
        tell_player(tp,
            "Type 'read occs' for classes available to your race.\n"
            "Then 'pick <occ name>'. Attribute minimums apply.\n");
        return;
    }
    if(strsrch(low, "help alignment") != -1) {
        tell_player(tp,
            "Type 'read list' during the alignment step, then\n"
            "'pick <alignment>' or 'alignment <name>'.\n");
        return;
    }
    if(strsrch(low, "help skills") != -1) {
        tell_player(tp,
            "After OCC selection the game prompts for elective skills,\n"
            "then secondary skills. Use 'skills' to list categories and\n"
            "'pick <skill>' for each choice.\n");
        return;
    }
    if(strsrch(low, "hello") != -1 || strsrch(low, "hi") != -1 ||
       strsrch(low, "help") != -1) {
        tell_player(tp,
            "Say 'status', 'help creation', 'help race', 'help occ',\n"
            "or 'help skills'.\n");
        tell_current_step(tp);
    }
}
