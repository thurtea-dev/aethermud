/* /domains/NewCamelot/areas/castle_courtyard.c
   Camelot Castle - Courtyard. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Camelot Castle - Courtyard");
    set_long(
        "The great courtyard of Camelot Castle. White stone walls rise on\n"
        "every side, hung with the dragon banners of the Order. The great\n"
        "hall stands north behind brass-bound doors. The barracks are east\n"
        "and the royal armory west. A spiral stair climbs the tallest\n"
        "tower, and a narrow stair descends beside the gatehouse.");
    set_listen("default",
        "Boots on flagstones and the flap of banners overhead.");
    set_smell("default",
        "Clean wind off the walls.");
    set_items( ([
        "banners" : "A white dragon rampant on a field of blue.",
        "walls" : "White stone over an MDC core. Beautiful and functional.",
        "stair" : "The spiral stair climbs to the dragon perch. The narrow one goes down to the dungeon."
    ]) );
    set_exits( ([
        "south" : "/domains/NewCamelot/areas/castle_gatehouse",
        "north" : "/domains/NewCamelot/areas/castle_great_hall",
        "east" : "/domains/NewCamelot/areas/castle_barracks",
        "west" : "/domains/NewCamelot/areas/castle_armoury_royal",
        "up" : "/domains/NewCamelot/areas/castle_tower_top",
        "down" : "/domains/NewCamelot/areas/castle_dungeon"
    ]) );
}
