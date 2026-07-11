/* /domains/NGR/npcs/ngr_barkeep.c
   the biergarten keeper. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("biergarten keeper");
    set_npc_name("Barkeep");
    set_id( ({ "biergarten keeper", "barkeep", "keeper" }) );
    set_short("the biergarten keeper");
    set_long(
        "A stout man with forearms like bridge cable and an apron older\n"
        "than most of his customers. He pulls dark beer with mechanical\n"
        "precision and remembers every tab to the pfennig.");
    set_level(5);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(300);
    set_body_type("human");
    add_response("hello",
        "Barkeep says: 'Sit. Drink. The front is forty kilometers that way and staying there.'");
    add_response("beer",
        "Barkeep says: 'Dark, darker, and officer stock. All three work.'");
    add_response("bye",
        "Barkeep says: 'Door is where you left it.'");
    set_default_response("The barkeep polishes a stein to regulation shine.");
}
