// /domains/ChiTown/npcs/cs_psi_stalker.c
// CS Psi-Stalker on inner-city watch.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("CS Psi-Stalker");
    set_npc_name("Psi-Stalker");
    set_id( ({ "cs psi-stalker", "psi-stalker", "psi stalker", "stalker",
               "coalition" }) );
    set_short("a CS Psi-Stalker");
    set_long(
        "A lean Psi-Stalker in Coalition black, eyes half-lidded as if\n"
        "listening to something you cannot hear. PPE and ISP leave trails\n"
        "this one can taste.");
    set_level(7);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("cs psi-stalker");
    set_alignment(-200);
    set_body_type("human");

    set_stats("SDC", 80);
    set_stats("max_SDC", 80);

    add_response("hello",
        "Psi-Stalker: 'Keep your mind quiet. I can hear the loud ones.'");
    add_response("hi",
        "Psi-Stalker: 'Keep your mind quiet. I can hear the loud ones.'");
    add_response("magic",
        "Psi-Stalker smiles without warmth. 'That word has a smell here.'");
    set_default_response("The Psi-Stalker tilts her head, listening past you.");
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
        "The Psi-Stalker's eyes snap open. \"Unauthorized. Marked.\"\n");
    kill_ob(tp, 0);
}

void create_equipment() {
    object knife;

    knife = clone_object(EQ_PATH + "vibro_knife.c");
    if(knife) knife->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wield knife in right hand");
}
