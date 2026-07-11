/* /domains/newcamelot/areas/castle_tower_top.c
   Camelot Castle - Dragon Perch. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Camelot Castle - Dragon Perch");
    set_long(
        "The flat crown of the castle's tallest tower, ringed by a low\n"
        "parapet. The stone here is scored by claw marks, and the wind\n"
        "carries the dry, hot smell of dragon. All of New Camelot spreads\n"
        "out below: Market Street, the gardens, the lake glinting in the\n"
        "west. The stair descends into the courtyard.");
    set_listen("default",
        "Wind, and far below, the sounds of the city.");
    set_smell("default",
        "Hot stone and the dry musk of dragon.");
    set_items( ([
        "claw marks" : "Gouges in solid stone, each as long as your arm.",
        "parapet" : "Low enough that a dragon can land over it.",
        "view" : "The whole of New Camelot, and the green country beyond."
    ]) );
    set_exits( ([
        "down" : "/domains/newcamelot/areas/castle_courtyard"
    ]) );
}

void reset() {
    ::reset();
    if(!present("young dragon", this_object()))
        new("/domains/newcamelot/npcs/young_dragon")->move(this_object());
}
