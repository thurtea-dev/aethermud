// /domains/chitown/npcs/cs_iss_clerk.c
// ISS desk at the inner checkpoint.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("ISS clerk");
    set_npc_name("ISS Clerk");
    set_id( ({ "iss clerk", "clerk", "iss", "officer" }) );
    set_short("an ISS clerk");
    set_long(
        "An Internal Security Service clerk at a steel desk. Datapads,\n"
        "a retinal scanner, and a sidearm within reach. The ISS watches\n"
        "citizens as carefully as it watches D-Bees.");
    set_level(5);
    set_max_hp(55);
    set_hp(55);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(-250);
    set_body_type("human");

    add_response("hello",
        "Clerk: 'State your business. Inner city access is logged.'");
    add_response("hi",
        "Clerk: 'State your business. Inner city access is logged.'");
    add_response("papers",
        "Clerk: 'If you have none, you do not belong past this desk.'");
    add_response("d-bee",
        "Clerk: 'Report sightings. Do not engage without authorization.'");
    add_response("magic",
        "Clerk's eyes harden. 'That is an ISS matter. Speak carefully.'");
    set_default_response("The clerk returns to her datapad.");
}
