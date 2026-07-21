// /domains/ChiTown/areas/burb_soup_kitchen.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("rest_allowed", 1);
    set_short("Burb Soup Kitchen");
    set_long(
        "A converted garage with the door rolled up. Long tables, mismatched\n"
        "bowls, and a drum fire under a battered pot. Chalk on the wall lists\n"
        "today's hours and a warning: NO WEAPONS DRAWN INSIDE.\n\n"
        "The tenement lane is south. East opens onto a small square.\n"
        "A flophouse door is north.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/burb_tenements",
        "east"  : "/domains/ChiTown/areas/burb_preacher_square",
        "north" : "/domains/ChiTown/areas/burb_flophouse"
    ]) );
    set_listen("default",
        "Spoons against bowls, quiet talk, the pop of the drum fire.");
    set_smell("default",
        "Onion, smoke, and something that might be meat.");
    set_items( ([
        "pot" : "A dented industrial pot. Steam rises in thin ribbons.",
        "tables" : "Scratched plastic and wood. Enough seats for a dozen.",
        "chalk" : "Hours scrawled in uneven letters. The weapons rule is underlined twice."
    ]) );
}

void reset() {
    ::reset();
    if(!present("soup cook", this_object()))
        clone_object("/domains/ChiTown/npcs/burb_soup_cook")->move(this_object());
}
