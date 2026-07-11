// /d/Praxis/monsters/kittani_warrior.c
// Kittani warrior -- aggressive alien humanoid, patrols Splynn Slave Pits.
// MDC: 45, PS: 18, PP: 16. Energy blade (3d6 MDC) + laser pistol (2d6 MDC).
// XP reward: 600 (set_exp 30000 / DEATH_EXP_MOD 50).

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("Kittani warrior");
    set_id( ({ "kittani warrior", "kittani", "warrior", "alien warrior", "soldier" }) );
    set_short("a Kittani warrior");
    set_long(
        "A Kittani warrior in full articulated power armor. The reptilian\n"
        "face is partly visible through the helmet visor: angular features,\n"
        "golden eyes, and a flat expression that communicates nothing good.\n"
        "An energy blade hums on one hip, a Kittani laser pistol on the other.\n"
        "The armor itself is MDC-rated and shows the marks of past battles.");

    set_race("kittani");
    set_class("warrior");
    set_level(5);
    set_exp(30000);
    set_alignment(-600);
    set_aggressive(1);

    set_stats("IQ", 12);
    set_stats("ME", 11);
    set_stats("MA", 8);
    set_stats("PS", 18);
    set_stats("PP", 16);
    set_stats("PE", 15);
    set_stats("PB", 7);
    set_stats("Spd", 18);

    set_stats("rifts_hp", 35);
    set_stats("max_rifts_hp", 35);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("MDC", 45);
    set_stats("max_MDC", 45);
    set_stats("is_MDC", 1);

    setenv("rifts_occ", "kittani warrior");
    setenv("rifts_flags", "mdc_being");

    set_money("credits", 200 + random(401));

    set_hp(80);
    set_mp(20);
    set_sp(100);

    set_property("damage_num", 3);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "3d6 MDC (energy blade)");
    set_property("mdc_weapon", 1);
    set_property("is_MDC", 1);
}
