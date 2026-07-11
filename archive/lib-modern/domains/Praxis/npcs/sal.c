// /d/Praxis/npcs/sal.c
// Sal, owner and manager of the Survivors' Hostel in Praxis.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Sal");
    set_npc_name("Sal");
    set_id( ({ "sal", "hostel owner", "owner", "manager", "woman", "human" }) );
    set_short("a grizzled human in a patched apron");
    set_long(
        "Sal has been running this place since before the Rifts hit. "
        "She looks like she has seen everything and survived all of it. "
        "Her apron is patched in at least four different materials, none "
        "of them matching. A short-barreled shotgun leans against the "
        "wall behind her within easy reach. She watches the door.");
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello", "Sal says: 'Bunk rooms are through the hallway. "
        "Kitchen is open. No fighting in here - I shoot first.'");
    add_response("hi", "Sal says: 'Bunk rooms are through the hallway. "
        "Kitchen is open. No fighting in here - I shoot first.'");
    add_response("rest", "Sal says: 'Bunk rooms are east and through the hall. "
        "Type rest when you're in a bunk room.'");
    add_response("bunk", "Sal says: 'Through the hallway. Take your pick.'");
    add_response("kitchen", "Sal says: 'East hall, then east again. "
        "Basic rations only. Credits accepted.'");
    add_response("price", "Sal says: 'Bunks are free for now. "
        "Kitchen charges by the item.'");
    add_response("cost", "Sal says: 'Bunks are free for now. "
        "Kitchen charges by the item.'");
    add_response("fight", "Sal's hand moves toward the shotgun. "
        "'Not in here you don't.'");
    add_response("bye", "Sal nods. 'Stay alive out there.'");
    set_default_response("Sal keeps her eyes on the door while she listens.");
}

void init() {
    ::init();
}
