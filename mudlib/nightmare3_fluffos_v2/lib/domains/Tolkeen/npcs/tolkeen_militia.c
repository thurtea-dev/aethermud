// /domains/Tolkeen/npcs/tolkeen_militia.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Tolkeen militia");
    set_npc_name("Militia");
    set_id( ({ "tolkeen militia", "militia", "soldier", "guard" }) );
    set_short("a Tolkeen militia fighter");
    set_long(
        "A Tolkeen fighter in mismatched armor plates and a ley-charged\n"
        "rifle. Tired. Still standing.");
    set_level(5);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(150);
    set_body_type("human");

    set_stats("MDC", 70);
    set_stats("max_MDC", 70);
    set_property("mdc_creature", 1);

    add_response("hello", "Militia: 'Wall shifts rotate every six hours. You volunteering?'");
    add_response("scout", "");
    add_response("siege", "");
    set_default_response("The militia fighter checks a ward charm and moves on.");
}

void init() {
    ::init();
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) { ::catch_tell(str); return; }
    b = lower_case(b);

    if(strsrch(b, "scout") != -1 || strsrch(b, "siege") != -1) {
        tp = this_player();
        if(tp && objectp(tp)) call_out("offer_scout_quest", 1, tp);
        return;
    }

    ::catch_tell(str);
}

void offer_scout_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "silence_the_scout") ||
       (int)QUEST_D->is_quest_active(tp, "silence_the_scout")) return;
    say_here("'A CS scout has been on the siege edge marking our walls. "
             "Put it down before it reports back and I will see you paid.'");
    catch(QUEST_D->start_quest(tp, "silence_the_scout"));
}
