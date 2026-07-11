/* /domains/Horton/monsters/quarry_lurker.c
   Something that nests in the old quarry. Level 7, SDC 90. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("quarry lurker");
    set_id( ({ "quarry lurker", "lurker", "beast", "animal" }) );
    set_short("a quarry lurker");
    set_long(
        "A low, mottled predator with too many joints in its forelimbs.\n"
        "It hugs the quarry walls and drops on anything that climbs down\n"
        "looking for scrap. Eyes reflect like wet glass.\n"
    );

    set_race("animal");
    set_class("beast");
    set_level(7);
    set_alignment(-250);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("animal");

    set_stats("IQ", 6);
    set_stats("ME", 8);
    set_stats("MA", 4);
    set_stats("PS", 17);
    set_stats("PP", 16);
    set_stats("PE", 15);
    set_stats("PB", 2);
    set_stats("Spd", 18);

    set_max_hp(55);
    set_hp(55);
    set_stats("SDC", 90);
    set_stats("max_SDC", 90);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 55);
    set_stats("max_rifts_hp", 55);
}

int query_attack_damage() {
    return random(7) + 5;
}
