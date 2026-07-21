// /domains/ChiTown/npcs/cs_recruiter.c
// CS Recruiter NPC with enlist behavior and alarm logic.

#include <std.h>
#include <daemons.h>

#define CS_OCCS ({ "cs grunt", "cs ranger", "cs dead boy", \
                   "cs military specialist", "cs specialist", \
                   "cs tech officer" })

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("CS recruiter");
    set_npc_name("Recruiter");
    set_id( ({ "cs recruiter", "recruiter", "officer", "soldier" }) );
    set_short("the CS Recruiter");
    set_long(
        "A polished Coalition officer in dress whites. Perfect posture,\n"
        "perfect hair, perfect smile. A datapad on the desk beside them\n"
        "holds what appears to be an endless supply of recruitment forms.\n"
        "\nAsk about 'enlist', 'join', or your 'occ'.");
    set_level(6);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello", "");
    add_response("hi", "");
    add_response("enlist", "");
    add_response("join", "");
    add_response("occ", "");
    add_response("mission","");
    add_response("bye", "Recruiter says: 'Serve the Coalition. Protect humanity.'");
    set_default_response("The recruiter smiles pleasantly and waits.");
}

void init() {
    ::init();
    add_action("cmd_enlist", "enlist");
    add_action("cmd_enlist", "join");
}

void catch_tell(string str) {
    string a;
    string b;
    object tp;
    string occ;
    string race;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);
    tp = this_player();
    if(!tp) { ::catch_tell(str); return; }

    if(strsrch(b, "hello") != -1 || strsrch(b, "hi") != -1 ||
       strsrch(b, "enlist") != -1 || strsrch(b, "join") != -1 ||
       strsrch(b, "occ") != -1 || strsrch(b, "mission") != -1) {
        call_out("evaluate_recruit", 1, tp);
        return;
    }
    ::catch_tell(str);
}

void evaluate_recruit(object tp) {
    string occ;
    string race;

    if(!tp || !objectp(tp)) return;
    race = lower_case((string)tp->query_race());
    occ = lower_case((string)tp->getenv("rifts_occ"));

    if(WANTED_D->is_cs_enemy(tp)) {
        say_here("Recruiter's eyes narrow. 'Security breach. Non-human detected.'");
        call_out("alarm_response", 1, tp);
        catch(FACTION_D->modify_standing(
            (string)tp->query_name(), "coalition", -100, "non-human in recruitment center"));
        return;
    }
    if(member_array(occ, CS_OCCS) != -1) {
        if((string)tp->getenv("cs_bonus_given") == "1") {
            say_here("Welcome back, soldier. Keep serving the Coalition.");
            return;
        }
        tp->setenv("cs_bonus_given", "1");
        tp->add_money("credits", 1000);
        write("The recruiter snaps a sharp salute. 'Welcome, soldier. A bonus "
              "of 1000 credits for your commitment to the Coalition.'\n");
        return;
    }
    say_here("'Interested in serving the Coalition? We're always looking for "
             "dedicated citizens. Speak with the quartermaster at the CS armory for equipment.'");
}

void alarm_response(object tp) {
    object g1;
    object g2;
    object env;

    env = environment(this_object());
    if(!env) return;
    tell_room(env,
        "An alarm klaxon sounds. 'SECURITY BREACH. NON-HUMAN DETECTED.'\n", ({}));
    g1 = clone_object("/domains/ChiTown/npcs/dead_boy_guard");
    g2 = clone_object("/domains/ChiTown/npcs/dead_boy_guard");
    if(g1) g1->move(env);
    if(g2) g2->move(env);
    if(g1 && tp) g1->kill_ob(tp, 0);
    if(g2 && tp) g2->kill_ob(tp, 0);
}

int cmd_enlist(string str) {
    evaluate_recruit(this_player());
    return 1;
}
