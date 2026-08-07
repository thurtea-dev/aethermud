// /d/Praxis/monsters/xiticix_warrior.c
// Xiticix warrior -- insectoid alien, attacks all non-Xiticix on sight.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int mdc;

    ::create();

    mdc = (random(6)+1 + random(6)+1) * 10 + 20;

    set_name("Xiticix warrior");
    set_id( ({ "xiticix warrior", "xiticix", "insect", "alien", "warrior" }) );
    set_short("a Xiticix warrior");
    set_long(
        "A six-limbed insectoid alien standing six feet tall. Chitinous\n"
        "exoskeleton in deep iridescent blue-black. Multi-faceted eyes take in\n"
        "everything simultaneously. Two pairs of bladed forelimbs are raised in\n"
        "a posture that clearly means threat. Xiticix are fiercely territorial\n"
        "and attack every non-Xiticix on sight -- no negotiation, no hesitation.");

    set_race("xiticix");
    set_class("alien");
    set_level(5);
    set_exp(30000);
    set_alignment(-300);
    set_aggressive(1);

    set_stats("IQ", 9);
    set_stats("ME", 10);
    set_stats("MA", 6);
    set_stats("PS", 20);
    set_stats("PP", 17);
    set_stats("PE", 18);
    set_stats("PB", 4);
    set_stats("Spd", 18);

    set_stats("MDC", mdc);
    set_stats("max_MDC", mdc);
    set_stats("rifts_hp", 40);
    set_stats("max_rifts_hp", 40);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "xiticix");
    setenv("rifts_flags", "");

    set_hp(mdc + 40);
    set_mp(0);
    set_sp(80);

    set_property("mdc_creature", 1);
}
