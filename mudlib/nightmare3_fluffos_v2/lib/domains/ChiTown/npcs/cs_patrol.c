// /domains/ChiTown/npcs/cs_patrol.c
// Coalition States patrol officer. Hostile to D-Bees without fake CS ID.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"
#define ITEM_PATH "/domains/ChiTown/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("CS patrol officer");
    set_npc_name("CS Patrol");
    set_id( ({ "cs patrol", "patrol officer", "patrol", "officer",
               "soldier", "coalition soldier", "cs soldier" }) );
    set_short("a CS patrol officer");
    set_long(
        "A Coalition States patrol officer in standard-issue black armor.\n"
        "The skull-faced visor is locked down. One hand rests near a\n"
        "holstered sidearm. The officer's movements are deliberate and\n"
        "practiced, scanning every face in the crowd.");
    set_level(6);
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

    add_response("hello", "The patrol officer eyes you. 'Keep moving, citizen.'");
    set_default_response("The patrol officer ignores you.");
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
        "The CS patrol officer points directly at you.\n"
        "\"Halt, D-Bee. Unauthorized presence in Coalition territory.\"\n");
    tell_room(environment(this_object()),
        "A CS patrol officer points at " + tp->query_cap_name() +
        ". \"Halt, D-Bee. Unauthorized presence in Coalition territory.\"\n",
        ({ this_object(), tp }));
    kill_ob(tp, 0);
}

void create_equipment() {
    object rifle;

    rifle = clone_object(EQ_PATH + "cp40_laser.c");
    if(rifle) rifle->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wield rifle in right hand");
}

void death_drops() {
    int i;
    int num;
    object ob;

    num = random(4) + 1;
    for(i = 0; i < num; i++) {
        ob = clone_object(EQ_PATH+"e_clip");
        if(ob) ob->move(environment(this_object()));
    }
    ob = clone_object(ITEM_PATH+"cs_dog_tag");
    if(ob) ob->move(environment(this_object()));
}

void die(object killer) {
    death_drops();
    ::die(killer);
}
