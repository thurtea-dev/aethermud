/* /domains/NewCamelot/npcs/dungeon_prisoner.c
   a gaunt prisoner. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("gaunt prisoner");
    set_npc_name("Prisoner");
    set_id( ({ "gaunt prisoner", "prisoner" }) );
    set_short("a gaunt prisoner");
    set_long(
        "A gaunt man sits on the straw pallet of his cell, turning a\n"
        "wooden button over and over in his fingers. Brodkil sympathizer,\n"
        "the guards say. Misunderstood merchant, he says. The button says\n"
        "nothing.");
    set_level(4);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(-300);
    set_body_type("human");
    add_response("hello",
        "Prisoner says: 'Visitors! Tell the quartermaster I want a better bucket.'");
    add_response("crime",
        "Prisoner says: 'Allegedly, friend. Allegedly.'");
    add_response("bye",
        "Prisoner says: 'You know where to find me.'");

    set_property("position_str", "sits on a straw pallet behind the bars.");
    set_default_response("The prisoner turns his wooden button over and over.");
}
