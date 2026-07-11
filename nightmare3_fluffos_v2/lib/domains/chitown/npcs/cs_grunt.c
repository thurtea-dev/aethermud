// /domains/chitown/npcs/cs_grunt.c
// Standard CS grunt on boulevard / plaza patrol.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"
#define ITEM_PATH "/domains/chitown/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("CS grunt");
    set_npc_name("CS Grunt");
    set_id( ({ "cs grunt", "grunt", "soldier", "coalition soldier" }) );
    set_short("a CS grunt");
    set_long(
        "A Coalition grunt in CA-4 armor, rifle held across the chest.\n"
        "Standard patrol. Standard orders. Standard suspicion.");
    set_level(5);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-150);
    set_body_type("human");

    set_stats("MDC", 70);
    set_stats("max_MDC", 70);
    set_property("mdc_creature", 1);

    add_response("hello", "Grunt: 'Papers if asked. Move if not.'");
    set_default_response("The grunt stares past you.");
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
    tell_object(tp, "The CS grunt levels a rifle. \"Halt, D-Bee!\"\n");
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

void die(object killer) {
    object ob;

    ob = clone_object(ITEM_PATH+"cs_dog_tag");
    if(ob) ob->move(environment(this_object()));
    ::die(killer);
}
