// /domains/LoneStar/npcs/coalition_scientist.c
// Coalition scientist working at the Lone Star genetics complex.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Coalition scientist");
    set_npc_name("the scientist");
    set_id( ({ "coalition scientist", "scientist", "cs scientist",
               "researcher", "doctor", "human" }) );
    set_short("a Coalition scientist in a white lab coat");
    set_long(
        "A lean human in a Coalition-issued white lab coat over standard-issue\n"
        "gray fatigues.  A data pad is tucked under one arm and a security badge\n"
        "on a lanyard around their neck marks them as Lone Star research staff.\n"
        "They look perpetually distracted, running calculations behind their eyes.");
    set_level(5);
    set_max_hp(45);
    set_hp(45);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(0);
    set_body_type("human");

    add_response("hello",      "The scientist glances up briefly.  'Yes?  What is it?'");
    add_response("hi",         "The scientist glances up briefly.  'Yes?  What is it?'");
    add_response("research",   "The scientist says, 'All research is classified.  Standard operational security.'");
    add_response("dog boy",    "'The Dog Boy program is the crown jewel of Lone Star,' they say with a thin pride.");
    add_response("gene",       "'Genetic engineering at this scale is unprecedented.  We are rewriting species here.'");
    add_response("emperor",    "The scientist straightens slightly.  'Emperor Prosek's vision guides everything we do here.'");
    add_response("lone star",  "'Lone Star is the future of the Coalition.  Everything you see here was built from nothing fifty years ago.'");
    add_response("danger",     "They look at you sharply.  'This complex is perfectly safe.  Every precaution has been taken.'");
    add_response("bye",        "The scientist waves a hand dismissively and goes back to their data pad.");
    set_default_response("The scientist frowns and makes a note on their data pad.");
}

void init() {
    ::init();
}
