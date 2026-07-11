// /domains/Lazlo/npcs/lazlo_ley_priest.c
// Lazlo ley line priest -- spiritual mediator of the nexus shrine.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("ley line priest");
    set_npc_name("Priest");
    set_id( ({ "ley line priest", "ley priest", "priest", "robed figure", "meditator" }) );
    set_short("a ley line priest");
    set_long(
        "A robed figure kneeling at the edge of the nexus point, eyes closed,\n"
        "hands resting open on their knees. The robes are deep indigo that\n"
        "seems to absorb light. Their eyes, when they open, glow faintly with\n"
        "ambient magical energy -- a sign of long immersion in ley line work.\n"
        "They speak slowly, as if choosing each word from a very large set\n"
        "of options.");

    set_level(7);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(500);
    set_body_type("human");

    add_response("hello",
        "Priest says: 'The ley lines are stronger today. I can feel three "
        "nexus points from here. Are you a practitioner?'");
    add_response("hi",
        "Priest says: 'The ley lines are stronger today. I can feel three "
        "nexus points from here. Are you a practitioner?'");
    add_response("ley line",
        "Priest says: 'Three major lines converge here. The western one "
        "runs to a nexus near Lake Simcoe. The eastern reaches the coast. "
        "The third I have not fully traced.'");
    add_response("nexus",
        "Priest says: 'This shrine predates Lazlo. Possibly predates the Rifts. "
        "Whatever it was built for, it still works perfectly.'");
    add_response("meditate",
        "Priest says: 'Sit. Breathe. Let the PPE flow through you rather "
        "than trying to hold it. It is not a reservoir. It is a current.'");
    add_response("magic",
        "Priest says: 'Magic on Rifts Earth is not a skill. It is a relationship "
        "with the lines. Practitioners who forget that do not last long.'");
    add_response("rifts",
        "Priest says: 'The lines are not evil. The Rifts are not evil. "
        "They are enormous and indifferent and we are very small. "
        "That is not the same thing.'");
    add_response("midnight",
        "Priest says: 'The surge at midnight is measurable. The shrine "
        "resonates for approximately forty seconds. Stand well back "
        "if you have no ley line training.'");
    add_response("bye",
        "Priest says: 'Go carefully. The lines follow you whether you notice them or not.'");
    set_default_response("The priest opens one eye to regard you, then closes it again.");

    setenv("rifts_flags", "magic");
}
