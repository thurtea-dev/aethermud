// /domains/tolkeen/npcs/tolkeen_innkeeper.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Tolkeen innkeeper");
    set_npc_name("Innkeeper");
    set_id( ({ "tolkeen innkeeper", "innkeeper", "barkeep", "human" }) );
    set_short("a Tolkeen innkeeper");
    set_long(
        "A broad innkeeper wiping a counter that has seen too many last\n"
        "nights before wall duty. A shotgun rests under the bar.");
    set_level(4);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello",
        "Innkeeper: 'Bed upstairs if you can sleep through ward alarms. Drink is cheaper.'");
    add_response("hi",
        "Innkeeper: 'Bed upstairs if you can sleep through ward alarms. Drink is cheaper.'");
    add_response("room",
        "Innkeeper: 'Rest is allowed in the common room. Pay in favors if you are broke.'");
    set_default_response("The innkeeper keeps wiping.");
}
