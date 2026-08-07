// /d/Praxis/monsters/coalition_dead_boy.c
// CS Dead Boy checkpoint guard.
// Attacks D-Bees and wanted players on sight. Demands papers from others.

#include <std.h>
#include <daemons.h>

#define EQUIP_PATH "/domains/Praxis/equipment/"

inherit MONSTER;

void create() {
    ::create();

    set_name("CS soldier");
    set_id( ({ "cs soldier", "dead boy", "soldier", "guard",
               "coalition soldier", "cs guard", "trooper" }) );
    set_short("a CS Dead Boy soldier");
    set_long(
        "A Coalition States soldier in full CA-4 Dead Boy armor, black\n"
        "composite plates over a sealed environmental suit. The white\n"
        "skull faceplate of the helmet stares back at you with dead eyes.\n"
        "He holds a CP-40 at port arms and watches the square with\n"
        "practiced hostility.");

    set_race("human");
    set_class("cs dead boy");
    set_level(4);
    set_exp(20000);
    set_alignment(-300);
    set_aggressive(0);

    set_stats("IQ", 12);
    set_stats("ME", 10);
    set_stats("MA", 8);
    set_stats("PS", 14);
    set_stats("PP", 13);
    set_stats("PE", 13);
    set_stats("PB", 7);
    set_stats("Spd", 15);

    set_stats("rifts_hp", 35);
    set_stats("max_rifts_hp", 35);
    set_stats("SDC", 30);
    set_stats("max_SDC", 30);
    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_stats("is_MDC", 0);
    setenv("rifts_occ", "cs dead boy");
    setenv("rifts_flags", "");
    set_property("rifts_prefers_ranged", 1);
    set_property("rifts_no_flee", 1);

    set_hp(100);
    set_mp(10);
    set_sp(90);

    set_moving(0);

    set_money("credits", random(200) + 100);

    new(EQUIP_PATH + "cp40_laser.c")->move(this_object());
    new(EQUIP_PATH + "dead_boy_armor.c")->move(this_object());
    new(EQUIP_PATH + "e_clip.c")->move(this_object());
    call_out("equip", 0);
}

void equip() {
    force_me("wear dead boy armor");
    force_me("wield rifle in right hand");
}

void init() {
    ::init();
    add_action("cmd_papers", "papers");
    add_action("cmd_hello", "hello");
    if(!this_player() || this_player() == this_object()) return;
    if(this_player()->query_ghost()) return;
    if((int)WANTED_D->is_cs_enemy(this_player()))
        call_out("attack_intruder", 0, this_player());
}

void attack_intruder(object enemy) {
    if(!objectp(enemy) || !objectp(this_object())) return;
    if(!environment(enemy) || !environment(this_object())) return;
    if(environment(enemy) != environment(this_object())) return;
    tell_room(environment(this_object()),
        "The CS soldier raises his CP-40. 'D-BEE! TERMINATE!'\n",
        ({ this_object() }));
    kill_ob(enemy, 0);
}

int cmd_papers(string str) {
    string race;

    if((int)WANTED_D->is_cs_enemy(this_player())) {
        tell_room(environment(this_object()),
            "The CS soldier aims his rifle. 'D-Bees have no papers. FIRE!'\n",
            ({ this_object() }));
        call_out("attack_intruder", 0, this_player());
        return 1;
    }
    race = lower_case((string)this_player()->query_race());
    tell_room(environment(this_object()),
        "The soldier scrutinizes "+this_player()->query_cap_name()+
        "'s face, then waves them through. 'Move along, citizen.'\n",
        ({ this_object() }));
    return 1;
}

int cmd_hello(string str) {
    if((int)WANTED_D->is_cs_enemy(this_player())) {
        call_out("attack_intruder", 0, this_player());
        return 1;
    }
    tell_room(environment(this_object()),
        "The soldier stares at "+this_player()->query_cap_name()+
        " through the skull faceplate. 'Keep moving, citizen.'\n",
        ({ this_object() }));
    return 1;
}
