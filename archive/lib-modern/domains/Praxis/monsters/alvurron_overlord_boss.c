/* /domains/Praxis/monsters/alvurron_overlord_boss.c
   the Overlord of Alvurron. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("gargoyle overlord");
    set_id( ({ "gargoyle overlord", "overlord", "gargoyle" }) );
    set_short("the Overlord of Alvurron");
    set_long(
        "The Overlord of Alvurron is a gargoyle grown vast and old, wings\n"
        "mantled like a cathedral roof, eyes the dull red of cooling iron.\n"
        "Centuries of tribute hang from its throne in chains: weapons,\n"
        "skulls, insignia of every army foolish enough to come down here.\n"
        "It does not rise. It has not needed to rise in a long time.");
    set_race("gargoyle");
    set_level(22);
    set_exp(400000);
    set_alignment(-900);
    set_aggressive(0);
    set_stats("MDC", 500);
    set_stats("max_MDC", 500);
    set_stats("PS", 45);
    set_hp(500);
    set_property("mdc_creature", 1);
    set_money("credits", 1000 + random(1000));
}
