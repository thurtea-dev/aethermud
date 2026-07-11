/* /d/Praxis/monsters/splugorth_minion.c
   Splugorth Minion -- slave-soldier of the Splugorth overlords.
   MDC creature, aggressive, guards Splugorth territory. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int mdc;

    ::create();

    mdc = (random(6)+1 + random(6)+1 + random(6)+1) * 10;

    set_name("Splugorth minion");
    set_id( ({ "splugorth minion", "minion", "servant", "slave soldier",
               "splugorth", "creature" }) );
    set_short("a Splugorth minion");
    set_long(
        "A humanoid soldier in the service of the Splugorth, its body a\n"
        "patchwork of flesh and Kittani-manufactured augmentation. The eyes\n"
        "are flat and obedient: it does not think for itself, it executes\n"
        "orders. Heavy bio-armor plates have been surgically grafted to its\n"
        "torso and arms. It carries a Kittani energy weapon in one hand and\n"
        "a short blade in the other.\n\n"
        "It has spotted you and it is moving.");

    set_race("splugorth minion");
    set_class("monster");
    set_level(7);
    set_exp(40000);
    set_alignment(-500);
    set_aggressive(1);

    set_stats("IQ", 8);
    set_stats("ME", 10);
    set_stats("MA", 6);
    set_stats("PS", 22);
    set_stats("PP", 16);
    set_stats("PE", 18);
    set_stats("PB", 4);
    set_stats("Spd", 16);

    set_stats("MDC", mdc);
    set_stats("max_MDC", mdc);
    set_stats("rifts_hp", 50);
    set_stats("max_rifts_hp", 50);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "splugorth minion");
    setenv("rifts_flags", "");

    set_hp(mdc + 50);
    set_mp(0);
    set_sp(80);

    set_property("mdc_creature", 1);
    set_property("AR", 12);
}
