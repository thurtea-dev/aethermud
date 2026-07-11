// /domains/Lazlo/npcs/lazlo_gate_guard.c
// Lazlo gate guard -- professional security, neutral to all races.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Lazlo gate guard");
    set_npc_name("Guard");
    set_id( ({ "lazlo gate guard", "gate guard", "guard", "soldier", "security" }) );
    set_short("a Lazlo gate guard");
    set_long(
        "A well-armed guard in non-Coalition composite armor with no faction\n"
        "markings. Calm, professional, scanning everyone who passes through\n"
        "the gate without hostility or judgment. Lazlo guards are known\n"
        "across North America for treating every species equally. The\n"
        "sidearm at their hip is for deterrence, not aggression.");

    set_level(5);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(400);
    set_body_type("human");

    add_response("hello",
        "Guard says: 'All races welcome in Lazlo. No unprovoked violence. "
        "The city council meets in the square. Welcome to the city.'");
    add_response("hi",
        "Guard says: 'All races welcome in Lazlo. No unprovoked violence. "
        "The city council meets in the square. Welcome to the city.'");
    add_response("lazlo",
        "Guard says: 'Population around eight thousand and growing. "
        "Scholars, mages, traders, and every species you can name. "
        "Trouble gets handled swiftly.'");
    add_response("coalition",
        "Guard says: 'We do not turn away Coalition citizens. "
        "CS soldiers entering officially are a different matter -- "
        "that conversation goes to the council.'");
    add_response("rules",
        "Guard says: 'Three rules: no unprovoked violence, no theft, "
        "no slavery. Everything else is negotiable.'");
    add_response("danger",
        "Guard says: 'The city is secure. The walls hold. "
        "I would not be this relaxed if they did not.'");
    add_response("market",
        "Guard says: 'Market quarter is north along the wall, then east. "
        "You will find most things there, legal and otherwise.'");
    add_response("library",
        "Guard says: 'Through the gate, north to the square, then north again. "
        "Hard to miss.'");
    add_response("bye",
        "Guard says: 'Enjoy Lazlo. The library is worth the visit.'");
    set_default_response("The guard watches you steadily without comment.");

    setenv("rifts_occ", "guard");
    setenv("rifts_flags", "");
}
