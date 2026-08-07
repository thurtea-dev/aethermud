// /d/Praxis/monsters/gargoyle.c
// Gargoyle -- winged supernatural MDC creature.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int mdc;

    ::create();

    mdc = (random(6)+1 + random(6)+1) * 10;

    set_name("gargoyle");
    set_id( ({ "gargoyle", "creature", "winged creature" }) );
    set_short("a gargoyle");
    set_long(
        "A winged stone-grey humanoid, roughly man-sized but built like a tank.\n"
        "Its wings fold tight against its back. The gargoyle watches you with\n"
        "yellow compound eyes and bares teeth that are more chisel than fang.\n"
        "Gargoyles are common supernatural vermin on Rifts Earth, often found\n"
        "near ley lines and ruins. This one is not going to let you pass.");

    set_race("gargoyle");
    set_class("supernatural");
    set_level(5);
    set_exp(25000);
    set_alignment(-500);
    set_aggressive(1);

    set_stats("IQ", 7);
    set_stats("ME", 10);
    set_stats("MA", 6);
    set_stats("PS", 24);
    set_stats("PP", 13);
    set_stats("PE", 18);
    set_stats("PB", 4);
    set_stats("Spd", 12);

    set_stats("MDC", mdc);
    set_stats("max_MDC", mdc);
    set_stats("rifts_hp", 40);
    set_stats("max_rifts_hp", 40);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "gargoyle");
    setenv("rifts_flags", "supernatural");

    set_hp(mdc + 40);
    set_mp(10);
    set_sp(70);

    set_property("supernatural", 1);
    set_property("mdc_creature", 1);
    set_property("flight_capable", 1);
}
