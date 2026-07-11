// /domains/chitown/npcs/burb_mechanic.c
// Chop-shop mechanic / light vendor dialogue.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("burb mechanic");
    set_npc_name("Mechanic");
    set_id( ({ "burb mechanic", "mechanic", "human", "tech" }) );
    set_short("a burb mechanic");
    set_long(
        "A grease-stained human with cybernetic fingers on the left hand,\n"
        "working under a half-stripped hovercycle. Tools hang from a belt\n"
        "that has seen better decades.");
    set_level(4);
    set_max_hp(45);
    set_hp(45);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(0);
    set_body_type("human");

    set_stats("SDC", 40);
    set_stats("max_SDC", 40);

    add_response("hello",
        "Mechanic: 'If it rolls, flies, or shoots, I can keep it limping. Credits up front.'");
    add_response("hi",
        "Mechanic: 'If it rolls, flies, or shoots, I can keep it limping. Credits up front.'");
    add_response("repair",
        "Mechanic: 'Bring the wreck. No Coalition serials, no questions.'");
    add_response("hovercycle",
        "Mechanic pats the frame: 'Not for sale. Parts, maybe.'");
    add_response("parts",
        "Mechanic: 'Ask the fence two streets over. I only wrench.'");
    set_default_response(
        "The mechanic ducks back under the hovercycle.");
}
