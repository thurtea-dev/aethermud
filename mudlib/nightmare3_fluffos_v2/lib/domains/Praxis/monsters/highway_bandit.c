// /d/Praxis/monsters/highway_bandit.c
// Highway bandit -- road scavenger preying on travellers.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

#define EQUIP_PATH "/domains/Praxis/equipment/"

void create() {
    ::create();

    set_name("highway bandit");
    set_id( ({ "highway bandit", "bandit", "scavenger", "raider" }) );
    set_short("a highway bandit");
    set_long(
        "A road scavenger in mismatched armor: pre-Rifts motorcycle leathers\n"
        "over military surplus plates, none of it matching. A scarred face\n"
        "under a cracked visor. The laser pistol in their hand is old but\n"
        "functional. They have been watching the road for hours and they\n"
        "look hungry.");

    set_race("human");
    set_class("bandit");
    set_level(3);
    set_exp(10000);
    set_alignment(-300);
    set_aggressive(1);

    set_stats("IQ", 9);
    set_stats("ME", 9);
    set_stats("MA", 8);
    set_stats("PS", 12);
    set_stats("PP", 12);
    set_stats("PE", 11);
    set_stats("PB", 8);
    set_stats("Spd", 13);

    set_stats("rifts_hp", 25);
    set_stats("max_rifts_hp", 25);
    set_stats("SDC", 35);
    set_stats("max_SDC", 35);
    set_stats("MDC", 0);
    set_stats("max_MDC", 0);
    set_stats("is_MDC", 0);

    setenv("rifts_occ", "bandit");
    setenv("rifts_flags", "");
    set_property("rifts_prefers_ranged", 1);
    set_property("rifts_flee_pct", 25);

    set_hp(50);
    set_mp(10);
    set_sp(60);

    set_money("credits", random(150) + 50);

    new(EQUIP_PATH + "c18_pistol.c")->move(this_object());
    call_out("equip", 0);
}

void equip() {
    force_me("wield pistol in right hand");
}

void die() {
    object patch;
    object env;

    env = environment(this_object());
    if(env) {
        patch = clone_object("/domains/Praxis/equipment/bandit_patch.c");
        if(patch) patch->move(env);
    }
    ::die();
}
