/* /domains/Horton/npcs/horton_hermit.c
   Quiet hermit living near the quarry rim. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Horton hermit");
    set_npc_name("the hermit");
    set_id( ({ "horton hermit", "hermit", "old man", "man", "human" }) );
    set_short("an old hermit");
    set_long(
        "An old human in layered coats, sitting on a crate with a kettle\n"
        "and a battered shotgun across his knees. He watches the quarry\n"
        "rim more than he watches you.\n"
    );

    set_race("human");
    set_level(8);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_gender("male");
    set_alignment(200);
    set_body_type("humanoid");

    add_response("hello", "The hermit nods. 'Stay off the quarry floor after dark.'");
    add_response("hi", "The hermit nods. 'Stay off the quarry floor after dark.'");
    add_response("quarry", "The hermit says, 'Something nests down there. Takes scrap. Takes people.'");
    add_response("horton", "The hermit says, 'Good town. Soft edges. The woods are not soft.'");
    add_response("lurker", "The hermit says, 'If you hear glass-clicking on stone, climb out fast.'");
    set_default_response("The hermit shrugs. 'Ask about the quarry if you are curious.'");
    set_property("position_str", "sits here with a kettle and a shotgun.");
}

void init() {
    ::init();
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says, \"%s\"", a, b) == 2) {
        b = lower_case(b);
        if(strsrch(b, "lurker") != -1 || strsrch(b, "quarry") != -1) {
            tp = this_player();
            if(tp && objectp(tp)) call_out("offer_lurker_quest", 2, tp);
        }
    }
    ::catch_tell(str);
}

void offer_lurker_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "quarry_lurker_cull") ||
       (int)QUEST_D->is_quest_active(tp, "quarry_lurker_cull")) return;
    say_here("'You want to earn your keep, go deal with it. Whatever's "
             "left of my old scrap is yours if you do.'");
    catch(QUEST_D->start_quest(tp, "quarry_lurker_cull"));
}
