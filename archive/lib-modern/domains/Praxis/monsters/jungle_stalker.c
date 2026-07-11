// /d/Praxis/monsters/jungle_stalker.c
// Jungle stalker -- large reptilian ambush predator.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int hp;

    ::create();

    hp = (random(6)+1 + random(6)+1 + random(6)+1 + random(6)+1) * 5;

    set_name("jungle stalker");
    set_id( ({ "jungle stalker", "stalker", "reptile", "predator", "animal" }) );
    set_short("a jungle stalker");
    set_long(
        "A reptilian predator six feet at the shoulder and twelve feet long.\n"
        "The coloring shifts to match the surrounding vegetation: browns and\n"
        "greens that make it nearly invisible until it moves. The stalker has\n"
        "been watching you from the underbrush for the last thirty seconds.\n"
        "Now it decides to move.");

    set_race("reptile");
    set_class("animal");
    set_level(4);
    set_exp(12500);
    set_alignment(-200);
    set_aggressive(1);

    set_stats("IQ", 5);
    set_stats("ME", 8);
    set_stats("MA", 4);
    set_stats("PS", 18);
    set_stats("PP", 18);
    set_stats("PE", 16);
    set_stats("PB", 4);
    set_stats("Spd", 18);

    set_stats("rifts_hp", hp);
    set_stats("max_rifts_hp", hp);
    set_stats("SDC", 40);
    set_stats("max_SDC", 40);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
    set_stats("is_MDC", 0);

    setenv("rifts_occ", "animal");
    setenv("rifts_flags", "");

    set_hp(hp + 40);
    set_mp(0);
    set_sp(80);

    set_body_type("animal");
}
