/* /domains/Praxis/monsters/alvurron_champion.c
   the Blood Pit champion. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("pit champion");
    set_id( ({ "pit champion", "champion", "gargoyle champion", "gargoyle" }) );
    set_short("the Blood Pit champion");
    set_long(
        "A gargoyle bred and built for the pit, wings docked to stubs to\n"
        "keep fights on the ground, hide layered with scar tissue over\n"
        "scar tissue. It works a stone pell with fists like engine blocks,\n"
        "and turns to look at you with professional interest.");
    set_race("gargoyle");
    set_level(15);
    set_exp(100000);
    set_alignment(-600);
    set_aggressive(0);
    set_stats("MDC", 260);
    set_stats("max_MDC", 260);
    set_stats("PS", 34);
    set_hp(260);
    set_property("mdc_creature", 1);
    set_money("credits", 200 + random(200));
}
