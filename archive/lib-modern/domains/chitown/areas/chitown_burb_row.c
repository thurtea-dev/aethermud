// /domains/chitown/areas/chitown_burb_row.c
// Deep burb row south of the shanties.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("Deep Burb Row");
    set_long(
        "The burbs at their worst: mud, refuse, and structures that might\n"
        "collapse in the next storm. A CS patrol drone circles overhead but\n"
        "does not descend. The people here survive on salvage, barter, and\n"
        "whatever the wall city throws away.\n\n"
        "Shanty row is north. Illinois Road lies south.");
    set_exits( ([
        "north" : "/domains/chitown/areas/chitown_burb_shanty",
        "south" : "/domains/chitown/areas/illinois_road"
    ]) );
    set_listen("default",
        "A drone engine overhead. Somewhere nearby, someone is crying.");
    set_smell("default",
        "Rot, smoke, and chemical runoff from the city wall.");
}
