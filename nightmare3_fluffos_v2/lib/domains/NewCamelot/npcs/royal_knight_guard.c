// /domains/NewCamelot/npcs/royal_knight_guard.c
// Royal Knight gate guard. Attacks players with infamy > 300 or coalition rep > 500.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("royal knight guard");
    set_npc_name("Royal Knight");
    set_id( ({ "royal knight guard", "royal knight", "knight", "guard", "soldier" }) );
    set_short("a Royal Knight guard");
    set_long(
        "A knight of New Camelot in gleaming full plate armor bearing the\n"
        "white dragon emblem. The armor is a blend of pre-Rifts and medieval\n"
        "craftsmanship. A lance is held at attention and a long sword hangs\n"
        "at the hip. Psionic energy crackles faintly around the gauntlets.");
    set_level(10);
    set_max_hp(150);
    set_hp(150);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(800);
    set_body_type("human");

    add_response("hello", "The Royal Knight nods. 'Hail, traveler. Welcome to New Camelot.'");
    add_response("hi", "The Royal Knight nods. 'Hail, traveler. Welcome to New Camelot.'");
    add_response("bye", "Royal Knight says: 'Travel safely.'");
    set_default_response("The Royal Knight watches with alert attention.");
}

void init() {
    ::init();
}
