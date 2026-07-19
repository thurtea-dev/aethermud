/* /domains/Praxis/npcs/chargen_guide.c
   Gamemaster NPC: optional walkthrough for Rifts chargen.
   Setter.c prints the authoritative step banners; this NPC does not
   auto-repeat them (that caused a third copy of STEP 1 on login). */

#include <std.h>
#include <config.h>

inherit MONSTER;

static private object pending_greet;

private void tell_player(object tp, string msg) {
    if(!tp || !objectp(tp)) return;
    tell_object(tp, "The Gamemaster says: " + msg);
}

/* Total silence for completed chargen and for all staff. No farewell. */
private int gm_silent(object tp) {
    string step;
    string pos;

    if(!tp) return 1;
    if((string)tp->getenv("chargen_complete") == "1") return 1;
    step = (string)tp->getenv("creation_step");
    if(step && step == "done") return 1;
    /* creatorp: position outside MORTAL_POSITIONS (player/high mortal/
       ambassador). Covers wiz_level / non-mortal position checks. */
    if(creatorp(tp)) return 1;
    pos = (string)tp->query_position();
    if(pos && sizeof(pos) &&
       member_array(pos, MORTAL_POSITIONS) == -1)
        return 1;
    return 0;
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

    if(gm_silent(tp)) return;
    step = query_creation_step(tp);
    race = (string)tp->query_race();
    occ = (string)tp->getenv("rifts_occ");
    switch(step) {
    case "region":
        tell_player(tp,
            "Choose your starting zone. Type: americas, europe, or atlantis.\n");
        break;
    case "stats":
        tell_player(tp,
            "Roll attributes, then accept or reroll:\n"
            "  roll      random 3d6 per stat\n"
            "  reroll    roll again (up to 4 rerolls)\n"
            "  accept    keep the roll and continue to race\n");
        break;
    case "race":
        tell_player(tp,
            "Choose a playable race. Type list, then the race name.\n");
        break;
    case "alignment":
        tell_player(tp,
            "Choose a Palladium alignment. Type its name:\n"
            "  principled, scrupulous, unprincipled, anarchist,\n"
            "  miscreant, aberrant, or diabolic\n");
        break;
    case "occ":
        tell_player(tp,
            "Choose an OCC. Type list, an OCC name, or none.\n");
        break;
    case "elective":
    case "secondary":
        tell_player(tp,
            "Skill picking during creation was removed. Your OCC/RCC\n"
            "package grants baseline skills. Request more via RP-Wiz.\n");
        break;
    case "done":
        return;
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
        "They can answer questions about character creation if asked.\n"
        "Say 'help creation', 'status', or 'what next'.");
    set_level(1);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(0);
    set_property("position_str", "waits patiently with a data slate.");
}

void init() {
    if(!this_player() || !living(this_player()) ||
       !interactive(this_player()))
        return;
    if(gm_silent(this_player())) return;
    pending_greet = this_player();
    remove_call_out("greet");
    call_out("greet", 1);
}

void greet() {
    object tp;

    if(!environment(this_object())) return;
    tp = pending_greet;
    pending_greet = 0;
    if(!tp || !objectp(tp)) return;
    if(environment(tp) != environment(this_object())) return;
    if(gm_silent(tp)) return;
    /* Short notice only. Do not call tell_current_step() here: setter.c
       already printed the authoritative STEP banner for this login. */
    tell_player(tp,
        "I am here if you need help. Say 'status' or 'what next' for\n"
        "your current step, or 'help creation' for the full sequence.\n");
}

void catch_tell(string str) {
    object tp;
    string low;
    string a;
    string b;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) return;
    tp = this_player();
    if(!tp) return;
    if(gm_silent(tp)) return;
    low = lower_case(b);

    if(strsrch(low, "status") != -1 || strsrch(low, "what next") != -1 ||
       strsrch(low, "where am i") != -1) {
        tell_current_step(tp);
        return;
    }
    if(strsrch(low, "help creation") != -1 || strsrch(low, "help chargen") != -1) {
        tell_player(tp,
            "Full creation sequence. Type each answer as a plain word:\n"
            "  1. starting zone: americas, europe, or atlantis\n"
            "  2. roll, then accept (or reroll, up to 4 times)\n"
            "  3. type a race name from the list\n"
            "  4. type an alignment name\n"
            "  5. type an OCC name, or none\n"
            "  6. finish - OCC/RCC package skills and gear arrive automatically\n\n"
            "Additional skills are requested in-game via RP-Wiz, not here.\n"
            "Say 'status' at any time for the step you are on now.\n");
        return;
    }
    if(strsrch(low, "help race") != -1) {
        tell_player(tp,
            "Type 'list' to see races, then type the race name.\n"
            "Some races restrict which OCCs you may take.\n");
        return;
    }
    if(strsrch(low, "help occ") != -1 || strsrch(low, "help class") != -1) {
        tell_player(tp,
            "Type 'list' for classes available to your race.\n"
            "Then type the OCC name. Attribute minimums apply.\n");
        return;
    }
    if(strsrch(low, "help alignment") != -1) {
        tell_player(tp,
            "During the alignment step type the alignment name,\n"
            "for example: scrupulous\n");
        return;
    }
    if(strsrch(low, "help skills") != -1) {
        tell_player(tp,
            "Creation no longer picks elective or secondary skills.\n"
            "Your OCC/RCC package grants baseline skills. Request more\n"
            "after you enter the world via the RP-Wiz tool.\n");
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
