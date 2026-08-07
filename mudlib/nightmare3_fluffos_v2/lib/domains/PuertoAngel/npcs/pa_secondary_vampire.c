/* /domains/PuertoAngel/npcs/pa_secondary_vampire.c
   Puerto Angel secondary vampire -- neutral patron, fights if attacked. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("secondary vampire");
    set_npc_name("secondary vampire");
    set_id( ({ "secondary vampire", "vampire", "patron", "figure", "undead",
               "pa vampire" }) );
    set_short("a secondary vampire");
    set_long(
        "Seated at a corner table with the particular stillness of something that\n"
        "does not need to breathe. Human enough in form: pale, still, dressed in\n"
        "clothing that is clean and well-kept. The eyes, when they move to you,\n"
        "are flat and reflective. Not hostile. Simply evaluating. They decide\n"
        "you are not a threat and look away. Good call on their part.");
    set_level(6);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("secondary vampire");
    set_alignment(-200);
    set_body_type("humanoid");

    set_stats("IQ", 12);
    set_stats("ME", 14);
    set_stats("MA", 11);
    set_stats("PS", 22);
    set_stats("PP", 18);
    set_stats("PE", 20);
    set_stats("PB", 10);
    set_stats("Spd", 22);

    /* MDC being */
    set_stats("MDC", 80);
    set_stats("max_MDC", 80);
    set_stats("rifts_hp", 60);
    set_stats("max_rifts_hp", 60);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("is_MDC", 1);

    set_property("vampire", 1);
    set_property("mdc_creature", 1);
    set_property("undead", 1);
    set_property("no_corpse", 1);
    set_property("AR", 12);
    set_property("visible_race", "human");

    setenv("rifts_race", "secondary vampire");
    setenv("rifts_flags", "undead");

    add_response("hello",
        "The vampire glances at you once and nods, very slightly.");
    add_response("hi",
        "The vampire acknowledges you with the minimum polite gesture and returns "
        "to whatever they were contemplating.");
    add_response("name",
        "secondary vampire says, quietly: 'That is not something I share with strangers.'");
    add_response("vampire",
        "secondary vampire says: 'Yes. Is that a problem?'");
    add_response("town",
        "secondary vampire says: 'Puerto Angel suits us. We maintain it. "
        "You are welcome here as long as you cause no harm.'");
    add_response("bye",
        "The vampire nods once. 'Travel safely.'");
    set_default_response(
        "The vampire regards you with flat, reflective eyes and says nothing.");
}

void init() {
    ::init();
}
