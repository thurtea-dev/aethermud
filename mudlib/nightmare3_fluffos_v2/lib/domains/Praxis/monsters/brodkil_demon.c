// /d/Praxis/monsters/brodkil_demon.c
// Brodkil demon -- supernatural MDC predator.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int mdc;

    ::create();

    mdc = (random(6)+1 + random(6)+1 + random(6)+1) * 10 + 30;

    set_name("brodkil demon");
    set_id( ({ "brodkil demon", "brodkil", "demon", "creature" }) );
    set_short("a Brodkil demon");
    set_long(
        "A hulking supernatural predator standing over eight feet tall.\n"
        "Grayish skin mottled with old scars, arms that hang past its knees,\n"
        "and a face assembled purely for violence. It watches you with dull\n"
        "red eyes. Brodkil are among the most dangerous supernatural threats\n"
        "on Rifts Earth, and this one has already decided what you are:\n"
        "food.");

    set_race("brodkil");
    set_class("demon");
    set_level(6);
    set_exp(40000);
    set_alignment(-900);
    set_aggressive(1);

    set_stats("IQ", 8);
    set_stats("ME", 12);
    set_stats("MA", 6);
    set_stats("PS", 26);
    set_stats("PP", 14);
    set_stats("PE", 22);
    set_stats("PB", 5);
    set_stats("Spd", 16);

    set_stats("MDC", mdc);
    set_stats("max_MDC", mdc);
    set_stats("rifts_hp", 50);
    set_stats("max_rifts_hp", 50);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "brodkil");
    setenv("rifts_flags", "supernatural");

    set_hp(mdc + 50);
    set_mp(10);
    set_sp(80);

    set_property("supernatural", 1);
    set_property("mdc_creature", 1);
    set_property("rifts_no_flee", 1);

    set_money("credits", (random(6)+1) * 10);
    if(!random(2))
        new("/domains/Praxis/equipment/vibro_knife.c")->move(this_object());
}
