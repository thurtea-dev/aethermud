/* /domains/NGR/npcs/ngr_sniper.c
   an NGR sniper. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("ngr sniper");
    set_npc_name("Sniper");
    set_id( ({ "ngr sniper", "sniper", "marksman" }) );
    set_short("an NGR sniper");
    set_long(
        "A motionless figure at the tower rail, wrapped in a sensor-baffle\n"
        "cloak, cheek welded to the stock of a long Triax rail rifle. You\n"
        "get the strong impression she saw you six minutes before you saw\n"
        "the tower.");
    set_level(11);
    set_max_hp(180);
    set_hp(180);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(500);
    set_body_type("human");
    add_response("hello",
        "The sniper does not move. 'You are in my sight line. Half a step left.'");
    add_response("gargoyles",
        "Sniper says: 'Ridge, two thousand meters. They test the wire at dusk. I discourage it.'");
    add_response("bye",
        "The sniper's attention has already returned to the ridge.");
    set_default_response("The sniper tracks something on the far ridge, breathing slow.");
}
