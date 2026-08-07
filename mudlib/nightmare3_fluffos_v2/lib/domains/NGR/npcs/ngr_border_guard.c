/* /domains/NGR/npcs/ngr_border_guard.c
   NGR border guard at the checkpoint. Non-aggressive. Level 5, MDC 100. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("NGR border guard");
    set_npc_name("NGR border guard");
    set_id( ({ "ngr border guard", "border guard", "guard", "soldier",
               "ngr soldier", "ngr guard" }) );
    set_short("an NGR border guard");
    set_long(
        "A checkpoint soldier of the New German Republic. Grey body armor,\n"
        "sealed visor, rail pistol holstered. This one processes travelers\n"
        "rather than engaging them: a clipboard terminal, an eye scanner,\n"
        "and the practiced patience of someone who has reviewed ten thousand\n"
        "sets of travel documents and found problems in roughly half of them.\n"
    );

    set_level(5);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(100);
    set_body_type("humanoid");

    set_stats("IQ", 12);
    set_stats("ME", 12);
    set_stats("MA", 10);
    set_stats("PS", 16);
    set_stats("PP", 14);
    set_stats("PE", 15);
    set_stats("PB", 10);
    set_stats("Spd", 18);

    set_stats("rifts_hp",     100);
    set_stats("max_rifts_hp", 100);
    set_stats("MDC",          100);
    set_stats("max_MDC",      100);

    set_exp(5000);
    set_property("mdc_creature", 1);
    set_property("coalition_type", "ngr");
    set_property("AR", 14);
    setenv("rifts_race", "human");
    setenv("rifts_occ",  "ngr soldier");

    add_response("hello",
        "NGR border guard says: 'Identification and purpose of travel. Documents out.'");
    add_response("hi",
        "NGR border guard says, without looking up: 'Documents.'");
    add_response("identification",
        "NGR border guard says: 'Scan the terminal to your left. All visitors. No exceptions.'");
    add_response("documents",
        "NGR border guard says: 'Terminal. Left. Now.'");
    add_response("checkpoint",
        "NGR border guard says: 'Standard border processing. Five minutes if you cooperate.'");
    add_response("ngr",
        "NGR border guard says: 'You are in NGR territory. Conduct yourself accordingly.'");
    add_response("triax",
        "NGR border guard says: 'Triax depot is inside, east of the market. You will\n"
        "need a purchase permit from this station.'");
    add_response("bye",
        "The guard makes a note on the terminal and returns to processing.");
    add_response("gargoyle", "");
    add_response("raider", "");
    set_default_response(
        "The guard says: 'State your purpose or move to the side. Others are waiting.'");
}

void init() {
    ::init();
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) { ::catch_tell(str); return; }
    b = lower_case(b);

    if(strsrch(b, "gargoyle") != -1 || strsrch(b, "raider") != -1) {
        tp = this_player();
        if(tp && objectp(tp)) call_out("offer_raider_quest", 1, tp);
        return;
    }

    ::catch_tell(str);
}

void offer_raider_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "border_gargoyle_raider") ||
       (int)QUEST_D->is_quest_active(tp, "border_gargoyle_raider")) return;
    say_here("'A gargoyle raider has been crossing no man's land near the "
             "wire. Eliminate it before it brings the rest of its caste, "
             "and NGR will make it worth your time.'");
    catch(QUEST_D->start_quest(tp, "border_gargoyle_raider"));
}
