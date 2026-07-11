// /domains/chitown/npcs/burb_refugee.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("burb refugee");
    set_npc_name("Refugee");
    set_id( ({ "burb refugee", "refugee", "human", "person" }) );
    set_short("a burb refugee");
    set_long(
        "A travel-worn human wrapped in a blanket, sitting on a crate near\n"
        "a cluster of tents. Dust from the Illinois Road still clings to\n"
        "their boots.");
    set_level(1);
    set_max_hp(25);
    set_hp(25);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(50);
    set_body_type("human");

    set_stats("SDC", 20);
    set_stats("max_SDC", 20);

    add_response("hello",
        "Refugee: 'Came up the road looking for the wall. Found the burbs instead.'");
    add_response("hi",
        "Refugee: 'Came up the road looking for the wall. Found the burbs instead.'");
    add_response("wall",
        "Refugee: 'They do not let people like me through. Not without papers.'");
    add_response("chi-town",
        "Refugee: 'Inside is clean. Out here is where the rest of us live.'");
    set_default_response(
        "The refugee pulls the blanket tighter and looks away.");
}
