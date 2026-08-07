// /domains/Praxis/monsters/preserve_captive.c
// Flavor-only caged captive for the Splynn Preserves Cage Row. No rescue
// or ownership mechanic; deliberately not the splynn_slave.c claim NPC.

#include <std.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("captive");
    set_npc_name("the captive");
    set_id( ({ "captive", "prisoner", "chained figure", "figure" }) );
    set_short("a chained captive");
    set_long(
        "A lone captive, chained to the bars of the nearest cage. Filthy\n"
        "and silent, watching you the way something already given up\n"
        "watches anything that might be worse than what already has it.\n"
        "Whatever the Splugorth intend for this one has not happened yet.\n"
    );

    set_race("human");
    set_level(1);
    set_max_hp(10);
    set_hp(10);
    set_aggressive(0);
    set_alignment(0);
    set_body_type("humanoid");

    add_response("hello", "The captive doesn't answer. Its eyes track you, but it says nothing.");
    add_response("hi", "The captive doesn't answer. Its eyes track you, but it says nothing.");
    add_response("help", "The captive flinches back against the bars and says nothing.");
    set_default_response("The captive stays silent, watching the cage door.");
    set_property("position_str", "sits chained against the bars of the cage.");
}
