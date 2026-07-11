// /d/Praxis/monsters/deserter.c
// CS infantry deserter hiding in the concealed alcove off the back alley.
// Talking to him updates the cs_deserter quest objective.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

private int __has_spoken;

void create() {
    ::create();

    set_name("deserter");
    set_npc_name("the deserter");
    set_id( ({ "deserter", "soldier", "cs soldier", "young man", "man" }) );
    set_short("a frightened CS deserter");
    set_long(
        "A young Coalition soldier, mid-twenties, in the remnants of his\n"
        "infantry uniform. He has cut the rank insignia from the top half\n"
        "and is wearing a civilian jacket over it. His hands won't stop\n"
        "moving. He has the look of someone who made a decision he can't\n"
        "unmake and is still processing that fact.");
    set_level(2);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(200);

    add_response("hello", "The deserter says: Don't tell them I'm here. Please.");
    add_response("hi", "The deserter says: Don't tell them I'm here. Please.");
    add_response("name", "The deserter says: No names. Not yet.");
    add_response("why", "The deserter says: I saw what they did in the outer zones. "
                             "I can't do that job anymore. That's all I'll say.");
    add_response("coalition","The deserter says: They'll shoot me on sight. Or worse. "
                             "Whatever Griz is offering, it won't end well for me.");
    add_response("griz", "The deserter says: I've heard of him. Arms dealer, right? "
                             "If he sent you, I'm already done.");
    add_response("talk", "");
    add_response("help", "The deserter says: Unless you can get me to Lazlo, there's nothing to help.");
    add_response("lazlo", "The deserter says: They take in people like me up there. Or so I heard. "
                             "I just need a way north that doesn't cross a checkpoint.");
    add_response("bye", "The deserter says: If you're going to report me, just go. Get it over with.");
    set_default_response("The deserter watches you with exhausted, wary eyes.");
    __has_spoken = 0;
}

void init() {
    ::init();
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);

    // Talking to the deserter updates the quest objective.
    tp = this_player();
    if(tp && !__has_spoken &&
       (int)QUEST_D->is_quest_active(tp, "cs_deserter")) {
        __has_spoken = 1;
        call_out("update_quest", 2, tp);
    }

    ::catch_tell(str);
}

void update_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    QUEST_D->update_objective(tp, "cs_deserter", "deserter_found", 1);
    tell_object(tp,
        "The deserter holds your gaze for a long moment.\n"
        "The deserter says: You found what you were looking for. "
        "Now go collect your payment.\n"
        "He looks away. He knows what comes next.\n");
}
