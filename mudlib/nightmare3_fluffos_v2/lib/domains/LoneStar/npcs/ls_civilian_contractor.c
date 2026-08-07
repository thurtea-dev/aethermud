/* /domains/LoneStar/npcs/ls_civilian_contractor.c
   Lone Star support town civilian: a contracted maintenance worker.
   Talk NPC, rumor source about the complex and the registration sweeps.
   Not hostile unless attacked. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("contractor");
    set_npc_name("the contractor");
    set_id( ({ "contractor", "worker", "civilian", "resident",
               "maintenance worker", "human" }) );
    set_short("a contracted maintenance worker");
    set_long(
        "A tired human in coveralls with a laminated day pass clipped to the\n"
        "chest, the kind that opens the gate and nothing beyond it. Their\n"
        "hands are chemical-burned across the knuckles. They have the settled\n"
        "look of somebody who has worked here long enough to stop asking what\n"
        "the tanks are for.");
    set_level(3);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(20);
    set_body_type("human");
    set_property("position_str", "leans against the laundry wall, out of the sun.");

    add_response("hello",
        "The contractor nods. 'New in town? Register at the credit office "
        "before Friday if you are not human. I am serious.'");
    add_response("hi",
        "The contractor nods. 'New in town? Register at the credit office "
        "before Friday if you are not human. I am serious.'");
    add_response("sweep",
        "'Weekly,' the contractor says. 'They walk the row with a Dog Boy "
        "and check papers. If your papers are fine it takes a minute. If "
        "they are not, you do not come back to the row.'");
    add_response("register",
        "'Credit office, half day, costs you a week of pay,' the contractor "
        "says. 'Cheaper than the alternative.'");
    add_response("d-bee",
        "The contractor looks up and down the street first. 'There were "
        "three living on this row when I signed on. I do not know where "
        "any of them went and I have stopped asking.'");
    add_response("dbee",
        "The contractor looks up and down the street first. 'There were "
        "three living on this row when I signed on. I do not know where "
        "any of them went and I have stopped asking.'");
    add_response("work",
        "'Maintenance contract. Filters, ducting, drains,' the contractor "
        "says. 'Day pass gets me through the gate and nowhere near the "
        "labs, which suits me.'");
    add_response("lone star",
        "'It is a good posting if you are human,' the contractor says. "
        "'Steady pay, clean water, no raiders. That is the whole pitch and "
        "it is honestly enough for most people.'");
    add_response("lab",
        "'Above my pass grade,' the contractor says. 'The drain lines out "
        "of that building run warm. That is all I will say about it.'");
    add_response("tunnel",
        "The contractor goes still for a second. 'The drainage under the "
        "fence is a hazard line, not a route. Whoever told you different "
        "should not have.'");
    add_response("bye",
        "'Watch yourself,' the contractor says, and goes back to leaning on "
        "the wall.");
    set_default_response(
        "The contractor shrugs. 'Could not tell you. I just fix the ducting.'");
}

void init() {
    ::init();
}
