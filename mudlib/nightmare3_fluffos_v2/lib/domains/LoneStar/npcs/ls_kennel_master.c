/* /domains/LoneStar/npcs/ls_kennel_master.c
   Lone Star kennel master: the Dog Boy handler who runs the kennel rows.
   Talk NPC. Not hostile unless attacked. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("kennel master");
    set_npc_name("the kennel master");
    set_id( ({ "kennel master", "master", "handler", "kennelmaster",
               "trainer", "human" }) );
    set_short("a Coalition kennel master");
    set_long(
        "A broad, weathered human in a stained handler's coat, sleeves cut\n"
        "off at the elbow and both forearms marked with old bite scars that\n"
        "were never treated properly. A clicker and a whistle hang on the\n"
        "same cord around the neck. They watch the pens the whole time they\n"
        "are talking to you, and they are better at reading the pens than\n"
        "they are at reading you.");
    set_level(6);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(-20);
    set_body_type("human");
    set_property("coalition_type", "cs");
    set_property("position_str", "watches the pens with one hand on the whistle.");

    add_response("hello",
        "The kennel master does not look over. 'Keep your hands off the mesh.'");
    add_response("hi",
        "The kennel master does not look over. 'Keep your hands off the mesh.'");
    add_response("dog boy",
        "'Dog Boy is what the recruiting posters say,' the kennel master "
        "says. 'Down here they are a line and a number until they earn a "
        "handler. Then they get a name.'");
    add_response("pup",
        "'They come up out of the vats already walking,' the kennel master "
        "says. 'Everything after that is conditioning. That part is mine.'");
    add_response("kennel",
        "'Twenty pens a row, two rows,' the kennel master says. 'Clean, "
        "warm, fed twice a shift. Nobody here mistreats them. That is not "
        "the same as saying it is kind.'");
    add_response("training",
        "'They learn to find magic by smell,' the kennel master says. "
        "'Ring is east. Do not go in while a drill is running.'");
    add_response("magic",
        "The kennel master's expression flattens. 'If you are carrying "
        "anything from a ley line, walk back out the way you came. They "
        "will smell it before I do.'");
    add_response("scars",
        "The kennel master glances at their own forearms. 'Early years. "
        "We did not know what we were doing yet. They did not either.'");
    add_response("lone star",
        "'The labs make them,' the kennel master says. 'I make them useful. "
        "Do not confuse the two jobs.'");
    add_response("bye",
        "The kennel master lifts two fingers off the whistle cord and goes "
        "back to the pens.");
    add_response("escapee", "");
    add_response("specimen", "");
    add_response("cold storage", "");
    set_default_response(
        "The kennel master watches the pens and does not answer.");
}

void init() {
    ::init();
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) { ::catch_tell(str); return; }
    b = lower_case(b);

    if(strsrch(b, "escapee") != -1 || strsrch(b, "specimen") != -1 ||
       strsrch(b, "cold storage") != -1) {
        tp = this_player();
        if(tp && objectp(tp)) call_out("offer_escapee_quest", 1, tp);
        return;
    }

    ::catch_tell(str);
}

void offer_escapee_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "cold_storage_escapee") ||
       (int)QUEST_D->is_quest_active(tp, "cold_storage_escapee")) return;
    say_here("The kennel master's jaw tightens. 'Two cradles in cold storage "
             "came up empty. One surplus line woke up and walked out under "
             "its own power. It is still down there. Put it down and I will "
             "make it worth your while.'");
    catch(QUEST_D->start_quest(tp, "cold_storage_escapee"));
}
