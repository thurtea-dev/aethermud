// /domains/chitown/npcs/cs_sergeant.c
// Coalition sergeant -- veteran Dead Boy with three stripes.
// Attacks D-Bees and non-humans on sight.

#include <std.h>
#include <daemons.h>

#define EQUIP_PATH "/domains/Praxis/equipment/"

inherit MONSTER;

void create() {
    ::create();

    set_name("CS sergeant");
    set_id( ({ "cs sergeant", "sergeant", "coalition sergeant", "soldier",
               "dead boy", "guard" }) );
    set_short("a Coalition sergeant");
    set_long(
        "A veteran Coalition soldier, skull-face helmet tucked under one arm.\n"
        "Three stripes on the shoulder of his CA-4 armor. He stands with the\n"
        "practiced ease of someone who has killed a great many things and expects\n"
        "to kill more. His eyes track everyone in the area without appearing to\n"
        "move at all.");

    set_race("human");
    set_class("cs dead boy");
    set_level(6);
    set_exp(25000);
    set_alignment(-400);
    set_aggressive(0);

    set_stats("IQ", 14);
    set_stats("ME", 12);
    set_stats("MA", 10);
    set_stats("PS", 16);
    set_stats("PP", 14);
    set_stats("PE", 15);
    set_stats("PB", 8);
    set_stats("Spd", 16);

    set_stats("rifts_hp", 45);
    set_stats("max_rifts_hp", 45);
    set_stats("SDC", 40);
    set_stats("max_SDC", 40);
    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_stats("is_MDC", 0);
    setenv("rifts_occ", "cs dead boy");

    set_hp(120);
    set_mp(10);
    set_sp(100);

    set_moving(0);
    set_money("credits", random(300) + 100);

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
    add_action("cmd_hello", "hello");
    add_action("cmd_say", "say");
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
        "The sergeant's hand drops to his weapon. 'D-BEE SPOTTED. TERMINATE!'\n",
        ({ this_object() }));
    kill_ob(enemy, 0);
}

int cmd_hello(string str) {
    if((int)WANTED_D->is_cs_enemy(this_player())) {
        call_out("attack_intruder", 0, this_player());
        return 1;
    }
    tell_room(environment(this_object()),
        "The sergeant looks " + this_player()->query_cap_name() +
        " over briefly. 'Move along, citizen. Report suspicious activity "
        "to the nearest checkpoint.'\n",
        ({ this_object() }));
    return 1;
}

int cmd_say(string str) {
    if(!str || !sizeof(str)) return 0;
    if((int)WANTED_D->is_cs_enemy(this_player())) {
        call_out("attack_intruder", 0, this_player());
        return 1;
    }
    return 0;
}
