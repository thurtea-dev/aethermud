/* /domains/newcamelot/npcs/chapel_priestess.c
   a priestess of the light. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("priestess of light");
    set_npc_name("Priestess");
    set_id( ({ "priestess of light", "priestess", "priest", "cleric" }) );
    set_short("a priestess of the light");
    set_long(
        "A calm woman in white and gold robes, her hair bound in a simple\n"
        "cord. She tends the chapel candles and the people who drift in\n"
        "from the street, treating both with the same unhurried care.");
    set_level(8);
    set_max_hp(140);
    set_hp(140);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(900);
    set_body_type("human");
    add_response("hello",
        "Priestess says: 'Peace be on you, traveler. Sit in the light awhile.'");
    add_response("heal",
        "Priestess says: 'Rest here. The light does the work, not me.'");
    add_response("bye",
        "Priestess says: 'Walk in the light.'");

    set_property("position_str", "is kneeling in quiet prayer.");
    set_default_response("The priestess trims a candle wick, unhurried.");
}
