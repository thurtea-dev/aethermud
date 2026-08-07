// /d/Praxis/monsters/coalition_grunt.c
// Coalition Dead Boy grunt - combat test NPC for the Highway 666 checkpoint.
// Moderate difficulty SDC opponent carrying a CP-40 laser rifle.
// The home room (test_zone.c) handles respawning via reset().

#include <std.h>
#include <daemons.h>

inherit MONSTER;

#define EQUIP_PATH "/domains/Praxis/equipment/"

void create() {
    ::create();

    set_name("Coalition grunt");
    set_id( ({ "coalition grunt", "grunt", "dead boy", "soldier",
               "cs grunt", "coalition soldier" }) );
    set_short("a Coalition grunt");
    set_long(
        "A Coalition States infantry trooper in full CA-1 grunt armor. "
        "The black-and-bone livery of the CS marks him clearly. His "
        "CP-40 pulse laser is held at a low ready. He has not spotted "
        "you yet, but he will.");

    set_race("human");
    set_class("cs grunt");
    set_level(3);
    set_exp(17500); // 350 XP to killer (17500 / DEATH_EXP_MOD 50)
    set_alignment(-400);
    set_aggressive(1);

    // Palladium attributes -- moderate CS grunt baseline
    set_stats("IQ", 11);
    set_stats("ME", 10);
    set_stats("MA", 9);
    set_stats("PS", 13);
    set_stats("PP", 12);
    set_stats("PE", 12);
    set_stats("PB", 8);
    set_stats("Spd", 14);

    // SDC damage pools (grunt is standard SDC)
    set_stats("rifts_hp", 28);
    set_stats("max_rifts_hp", 28);
    set_stats("SDC", 45);
    set_stats("max_SDC", 45);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
    set_stats("is_MDC", 0);

    // Flag as a Rifts NPC so the combat daemon routes correctly
    setenv("rifts_occ", "cs grunt");
    setenv("rifts_flags", "");

    // Credit loot (moves to corpse on death)
    set_money("credits", 100 + random(201));

    // Nightmare living stats (standard combat fallback)
    set_hp(60);
    set_mp(20);
    set_sp(80);

    // Starting equipment
    new(EQUIP_PATH + "cp40_laser.c")->move(this_object());
    new(EQUIP_PATH + "coalition_grunt_armor.c")->move(this_object());

    // Equip after items land in inventory (deferred one tick)
    call_out("equip", 0);
}

void equip() {
    force_me("wear coalition grunt armor");
    force_me("wield rifle in right hand");
}
