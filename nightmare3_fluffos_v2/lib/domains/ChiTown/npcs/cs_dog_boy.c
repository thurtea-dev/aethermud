// /domains/ChiTown/npcs/cs_dog_boy.c
// Coalition Dog Boy on city patrol. Hostile to D-Bees without fake ID.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"
#define ITEM_PATH "/domains/ChiTown/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("CS Dog Boy");
    set_npc_name("Dog Boy");
    set_id( ({ "cs dog boy", "dog boy", "mutant", "patrol", "coalition" }) );
    set_short("a CS Dog Boy");
    set_long(
        "A Coalition Dog Boy in light armor, ears up, snout working the air.\n"
        "A laser pistol rides the hip. The mutant was bred to smell magic,\n"
        "fear, and lies.");
    set_level(6);
    set_max_hp(55);
    set_hp(55);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("dog boy");
    set_alignment(-150);
    set_body_type("human");

    set_stats("SDC", 75);
    set_stats("max_SDC", 75);

    add_response("hello", "The Dog Boy sniffs once. 'Move along.'");
    add_response("hi", "The Dog Boy sniffs once. 'Move along.'");
    set_default_response("The Dog Boy watches you without blinking.");
    call_out("create_equipment", 0);
}

private int player_has_fake_id(object tp) {
    object ob;

    if(!tp || !objectp(tp)) return 0;
    ob = first_inventory(tp);
    while(ob) {
        if((int)ob->query_property("fake_cs_id")) return 1;
        ob = next_inventory(ob);
    }
    return 0;
}

void init() {
    ::init();
    if(!this_player() || !objectp(this_player())) return;
    if(creatorp(this_player())) return;
    if((int)WANTED_D->is_cs_enemy(this_player()) &&
       !player_has_fake_id(this_player())) {
        call_out("engage_dbee", 1, this_player());
    }
}

void engage_dbee(object tp) {
    if(!tp || !objectp(tp)) return;
    if(!environment(tp) || environment(tp) != environment(this_object())) return;
    tell_object(tp,
        "The Dog Boy's ears flatten. He points at you and barks an alert.\n");
    tell_room(environment(this_object()),
        "A Dog Boy points at " + tp->query_cap_name() +
        " and barks a sharp alert.\n",
        ({ this_object(), tp }));
    kill_ob(tp, 0);
}

void create_equipment() {
    object pistol;

    pistol = clone_object(EQ_PATH + "cp30_laser.c");
    if(pistol) pistol->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wield pistol in right hand");
}

void die(object killer) {
    object ob;

    ob = clone_object(ITEM_PATH+"cs_dog_tag");
    if(ob) ob->move(environment(this_object()));
    ::die(killer);
}
