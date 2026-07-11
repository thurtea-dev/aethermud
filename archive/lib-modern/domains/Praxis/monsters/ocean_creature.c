/* /domains/Praxis/monsters/ocean_creature.c
   Rift-touched sea creature in the Ocean Rift shallows. Level 6, MDC 80. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("ocean creature");
    set_id( ({ "ocean creature", "creature", "beast", "sea creature" }) );
    set_short("a rift-touched sea creature");
    set_long(
        "A deep-sea creature pulled through the rift: part eel, part something\n"
        "with no classification. Bioluminescent patches along its flanks pulse\n"
        "in slow rhythms. The dimensional energy has warped it beyond its\n"
        "original form. It does not seem interested in you. Yet.\n"
    );

    set_race("monster");
    set_class("beast");
    set_level(6);
    set_exp(25000);
    set_alignment(0);
    set_aggressive(0);

    set_stats("IQ", 4);
    set_stats("ME", 8);
    set_stats("MA", 5);
    set_stats("PS", 18);
    set_stats("PP", 14);
    set_stats("PE", 16);
    set_stats("PB", 3);
    set_stats("Spd", 14);

    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_stats("is_MDC", 1);
    set_stats("rifts_hp", 80);
    set_stats("max_rifts_hp", 80);

    set_property("mdc_creature", 1);
}
