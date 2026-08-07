/* /domains/Horton/monsters/feral_boar.c
   Aggressive feral boar. Level 5, SDC 70. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("feral boar");
    set_id( ({ "feral boar", "boar", "pig", "animal" }) );
    set_short("a feral boar");
    set_long(
        "A heavy-shouldered boar with yellow tusks and mud-caked bristles.\n"
        "It snorts, stamps, and charges anything that smells like food or\n"
        "threat. The wilderness around Horton breeds them mean.\n"
    );

    set_race("animal");
    set_class("beast");
    set_level(5);
    set_alignment(-50);
    set_aggressive(1);
    set_gender("neutral");
    set_body_type("animal");

    set_stats("IQ", 4);
    set_stats("ME", 6);
    set_stats("MA", 3);
    set_stats("PS", 16);
    set_stats("PP", 10);
    set_stats("PE", 15);
    set_stats("PB", 2);
    set_stats("Spd", 14);

    set_max_hp(40);
    set_hp(40);
    set_stats("SDC", 70);
    set_stats("max_SDC", 70);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 40);
    set_stats("max_rifts_hp", 40);
}

int query_attack_damage() {
    return random(5) + 4;
}
