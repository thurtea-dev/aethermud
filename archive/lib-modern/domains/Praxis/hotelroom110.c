/* /d/Praxis/hotelroom110.c
   Praxis Refuge Inn guest room 110 (suite).
   Rented through the receptionist in hotel.c; door key checked by hall5.c. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( ([
        "light" : 2,
        "indoors" : 1,
        "no attack" : 1,
        "no steal" : 1,
        "no castle" : 1,
        "rest_allowed" : 1
    ]) );
    set_short("Praxis Refuge Inn, Room 110");
    set_long(
        "The finest room in the Praxis Refuge Inn. A wide pre-Rifts bed\n"
        "with carved posts fills half the chamber, made up with linens\n"
        "that someone has kept white for a century. A claw-footed bath\n"
        "stands behind a folding screen, and a cabinet holds glasses\n"
        "and a bottle of something old and amber. The window is real\n"
        "stained glass, salvaged from somewhere long gone.\n\n"
        "The hallway is out.");
    set_items( ([
        "bed" : "A wide pre-Rifts bed with carved posts. It looks like\n"
                "sleeping in it could fix a hard year.",
        "bath" : "A claw-footed bathtub with hot water from the inn boiler.",
        "screen" : "A folding privacy screen painted with cranes.",
        "cabinet" : "Glasses and a bottle of amber liquor. House gift for\n"
                    "suite guests.",
        "window" : "Stained glass, salvaged from a ruined chapel. It scatters\n"
                   "colored light across the floor."
    ]) );
    set_exits( ([ "out" : "/domains/Praxis/hall5" ]) );
}
