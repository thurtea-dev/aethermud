// /domains/ChiTown/monsters/cs_juicer.c
// Coalition Juicer shock trooper. Hostile to D-Bees.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit MONSTER;

void create() {
    ::create();
    set_name("CS Juicer");
    set_id(({ "cs juicer", "juicer", "soldier", "shock trooper" }));
    set_short("a CS Juicer shock trooper");
    set_long(
        "A Coalition Juicer in reinforced armor, veins glowing with\n"
        "chem-enhancement. Hyper-fast and brutal. The skull emblem on\n"
        "the shoulder marks CS service.");
    set_race("human");
    set_class("juicer");
    set_level(10);
    set_exp(45000);
    set_alignment(-400);
    set_aggressive(0);
    set_stats("MDC", 120);
    set_stats("max_MDC", 120);
    set_stats("SDC", 80);
    set_stats("max_SDC", 80);
    set_stats("rifts_hp", 60);
    set_stats("max_rifts_hp", 60);
    set_stats("PS", 24);
    set_stats("Spd", 22);
    setenv("rifts_occ", "juicer");
    set_hp(200);
    set_moving(0);
    new(EQ_PATH + "cp40_laser.c")->move(this_object());
    new(EQ_PATH + "environmental_body_armor.c")->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wear environmental body armor");
    force_me("wield rifle in right hand");
}

void init() {
    ::init();
    if(this_player() && living(this_player()) &&
       (int)WANTED_D->is_cs_enemy(this_player()))
        call_out("attack_intruder", 0, this_player());
}

void attack_intruder(object enemy) {
    if(!objectp(enemy) || !objectp(this_object())) return;
    kill_ob(enemy, 0);
}
