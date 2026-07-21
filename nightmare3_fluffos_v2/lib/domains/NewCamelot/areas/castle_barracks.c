/* /domains/NewCamelot/areas/castle_barracks.c
   Camelot Castle - Barracks. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("Camelot Castle - Barracks");
    set_long(
        "The barracks of the Dragon Knights. Rows of iron cots stand in\n"
        "perfect order, each with a footlocker and an armor stand. Most\n"
        "stands are empty; their owners are on patrol or at drill. The\n"
        "courtyard is west.");
    set_listen("default",
        "Someone snoring off a night patrol at the far end.");
    set_smell("default",
        "Armor polish and soap.");
    set_items( ([
        "cots" : "Made so tight a thrown credit would bounce.",
        "footlockers" : "Locked, every one.",
        "stands" : "Racks shaped to hold full suits of knight plate."
    ]) );
    set_exits( ([
        "west" : "/domains/NewCamelot/areas/castle_courtyard"
    ]) );
}

void reset() {
    ::reset();
    if(!present("dragon knight", this_object()))
        new("/domains/NewCamelot/npcs/dragon_knight")->move(this_object());
}
