// /domains/tolkeen/npcs/tolkeen_refugee.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Tolkeen refugee");
    set_npc_name("Refugee");
    set_id( ({ "tolkeen refugee", "refugee", "human", "person" }) );
    set_short("a Tolkeen refugee");
    set_long(
        "A displaced farmer wrapped in a blanket, sitting near a cookfire.\n"
        "Village gone. Coalition advance. Tolkeen was the only wall left.");
    set_level(1);
    set_max_hp(25);
    set_hp(25);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(50);
    set_body_type("human");

    add_response("hello",
        "Refugee: 'They burned the fields first. Then the church. Then us.'");
    add_response("coalition",
        "Refugee: 'Gray armor. No faces. No mercy.'");
    set_default_response("The refugee stares into the fire.");
}
