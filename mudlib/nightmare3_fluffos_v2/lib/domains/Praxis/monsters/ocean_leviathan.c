/* /domains/Praxis/monsters/ocean_leviathan.c
   Massive dimensional leviathan guarding the deep Ocean Rift. Level 12, MDC 350. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("ocean leviathan");
    set_id( ({ "ocean leviathan", "leviathan", "beast", "creature" }) );
    set_short("a massive ocean leviathan");
    set_long(
        "Something vast and ancient, too large to fully comprehend in the dark.\n"
        "Scales the size of shields. Eyes like cold lights at great depth.\n"
        "The rift energy has fused with it over decades or centuries. It is\n"
        "no longer a creature of this dimension, or of any single one.\n"
        "It is aware of you.\n"
    );

    set_race("monster");
    set_class("beast");
    set_level(12);
    set_exp(180000);
    set_alignment(-600);
    set_aggressive(1);

    set_stats("IQ", 6);
    set_stats("ME", 12);
    set_stats("MA", 4);
    set_stats("PS", 38);
    set_stats("PP", 12);
    set_stats("PE", 30);
    set_stats("PB", 2);
    set_stats("Spd", 16);

    set_stats("MDC", 350);
    set_stats("max_MDC", 350);
    set_stats("is_MDC", 1);
    set_stats("rifts_hp", 350);
    set_stats("max_rifts_hp", 350);

    set_property("mdc_creature", 1);
}
