// /domains/chitown/npcs/dead_boy_guard.c
// Coalition Dead Boy guard. MDC 80, CP-40 rifle.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Dead Boy guard");
    set_npc_name("Dead Boy guard");
    set_id( ({ "dead boy guard", "dead boy", "guard", "soldier", "coalition soldier" }) );
    set_short("a Dead Boy guard");
    set_long(
        "A Coalition soldier in full CA-4 black armor, the skull-faced helmet\n"
        "locked down and visor active. A CP-40 pulse laser rifle is held at\n"
        "ready. The guard moves with practiced efficiency and zero hesitation.");
    set_level(8);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-200);
    set_body_type("human");

    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_property("mdc_creature", 1);

    add_response("hello", "The guard stares straight ahead. Not here for conversation.");
    set_default_response("The guard ignores you.");
    call_out("create_equipment", 0);
}

void init() {
    ::init();
}

void create_equipment() {
    object armor;
    object rifle;

    armor = clone_object(EQ_PATH + "dead_boy_armor.c");
    if(armor) armor->move(this_object());
    rifle = clone_object(EQ_PATH + "cp40_laser.c");
    if(rifle) rifle->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wear dead boy armor");
    force_me("wield rifle in right hand");
}

