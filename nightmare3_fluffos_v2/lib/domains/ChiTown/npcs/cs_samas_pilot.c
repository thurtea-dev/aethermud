// /domains/ChiTown/npcs/cs_samas_pilot.c
// SAMAS pilot near the hangar pad.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("SAMAS pilot");
    set_npc_name("SAMAS Pilot");
    set_id( ({ "samas pilot", "pilot", "soldier", "coalition" }) );
    set_short("a SAMAS pilot");
    set_long(
        "A Coalition power-armor pilot in a flight suit, helmet under one\n"
        "arm. Behind them, a SAMAS frame stands locked in a charging cradle.\n"
        "The pilot looks bored and dangerous in equal measure.");
    set_level(8);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-100);
    set_body_type("human");

    set_stats("MDC", 90);
    set_stats("max_MDC", 90);
    set_property("mdc_creature", 1);

    add_response("hello",
        "Pilot: 'Unless you have flight clearance, keep clear of the pad.'");
    add_response("hi",
        "Pilot: 'Unless you have flight clearance, keep clear of the pad.'");
    add_response("samas",
        "Pilot pats the cradle. 'PA-06A. She flies. You do not.'");
    add_response("armor",
        "Pilot: 'Power armor is for pilots. Not tourists.'");
    set_default_response("The pilot checks a datapad and ignores you.");
}
