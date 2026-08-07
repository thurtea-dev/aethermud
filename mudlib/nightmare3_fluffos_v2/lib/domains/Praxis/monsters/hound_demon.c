// /d/Praxis/monsters/hound_demon.c
// Hound demon -- pack hunter supernatural minion.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int mdc;

    ::create();

    mdc = (random(6)+1) * 10 + 10;

    set_name("hound demon");
    set_id( ({ "hound demon", "hound", "demon", "creature" }) );
    set_short("a hound demon");
    set_long(
        "A dog-like supernatural creature the size of a small pony. Black fur\n"
        "that seems to absorb light, red eyes, and jaws dripping caustic\n"
        "saliva that smokes where it hits the ground. Hound demons hunt in\n"
        "packs and are often used as forward scouts by greater demons.\n"
        "This one snarls low in its throat and crouches to spring.");

    set_race("hound demon");
    set_class("demon");
    set_level(4);
    set_exp(15000);
    set_alignment(-700);
    set_aggressive(1);

    set_stats("IQ", 6);
    set_stats("ME", 9);
    set_stats("MA", 5);
    set_stats("PS", 18);
    set_stats("PP", 15);
    set_stats("PE", 16);
    set_stats("PB", 3);
    set_stats("Spd", 24);

    set_stats("MDC", mdc);
    set_stats("max_MDC", mdc);
    set_stats("rifts_hp", 30);
    set_stats("max_rifts_hp", 30);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "hound demon");
    setenv("rifts_flags", "supernatural");

    set_hp(mdc + 30);
    set_mp(0);
    set_sp(60);

    set_property("supernatural", 1);
    set_property("mdc_creature", 1);
    set_body_type("animal");
}
