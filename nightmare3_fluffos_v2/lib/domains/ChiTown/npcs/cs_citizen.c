// /domains/ChiTown/npcs/cs_citizen.c
// A Chi-Town human citizen. Neutral, knows to keep head down.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Chi-Town citizen");
    set_npc_name("Citizen");
    set_id( ({ "chi-town citizen", "citizen", "human", "person", "man", "woman" }) );
    set_short("a Chi-Town citizen");
    set_long(
        "A human in clean Coalition-approved clothing. Eyes forward, walking\n"
        "briskly. Whatever they think about anything, they are not saying it\n"
        "out loud in earshot of a patrol. Living in the city means following\n"
        "the rules.");
    set_level(1);
    set_max_hp(30);
    set_hp(30);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello",
        "Citizen says quietly: 'Keep your head down and don't ask questions. "
        "That is how you survive here.'");
    add_response("hi",
        "Citizen says quietly: 'Keep your head down and don't ask questions. "
        "That is how you survive here.'");
    add_response("coalition",
        "The citizen glances around nervously before responding: "
        "'Strength and unity. That is all there is to say about that.'");
    add_response("cs",
        "The citizen glances around nervously before responding: "
        "'Strength and unity. That is all there is to say about that.'");
    add_response("magic",
        "Citizen whispers: 'Don't say that word out loud here. They monitor for it.'");
    add_response("bye",
        "The citizen nods once and moves on quickly.");
    add_response("danger",
        "Citizen says: 'Danger? Here? Everything is perfectly safe in Chi-Town.'");
    set_default_response(
        "The citizen keeps moving without making eye contact.");
}
