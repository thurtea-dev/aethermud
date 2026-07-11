/* /domains/newcamelot/npcs/mrrlyn.c
   Mrrlyn, counselor to the King. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("Mrrlyn");
    set_npc_name("Mrrlyn");
    set_id( ({ "mrrlyn", "merlin", "counselor", "advisor" }) );
    set_short("Mrrlyn, counselor to the King");
    set_long(
        "An old man in grey robes stands at the King's right hand, leaning\n"
        "on a staff of pale wood. His beard is long, his smile is kind, and\n"
        "his eyes are wrong: too dark, too deep, like holes cut in the day.\n"
        "Ley-line energy bends very slightly toward him, the way water\n"
        "leans toward a drain.");
    set_level(30);
    set_max_hp(1600);
    set_hp(1600);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-100);
    set_body_type("human");
    add_response("hello",
        "Mrrlyn says: 'Yes, yes, welcome. We have been expecting you. We are always expecting everyone.'");
    add_response("magic",
        "Mrrlyn says: 'The Academy teaches what is safe to teach. I advise on the remainder.'");
    add_response("king",
        "Mrrlyn says: 'A great man. A very great man. I have made quite sure of it.'");
    add_response("bye",
        "Mrrlyn smiles. You feel briefly catalogued.");

    set_property("position_str", "stands at the right hand of the throne.");
    set_default_response("Mrrlyn studies you the way a jeweler studies a stone.");
}
