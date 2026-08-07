// /d/Praxis/inn108.c
// Survivors' Hostel, Private Room 2. Second room off the rental hallway.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Private Room 2");
    set_long(
        "A small private room with a single bed, a lockable cabinet, and\n"
        "a narrow window overlooking the alley behind the hostel. The door\n"
        "has a keypad lock. Currently unoccupied.\n\n"
        "The rental hall continues south back toward the main hallway,\n"
        "and north to the remaining doors.");
    set_exits( ([
        "south" : "/domains/Praxis/inn107",
        "north" : "/domains/Praxis/inn109"
    ]) );
    set_items( ([
        "bed"     : "A single bed. Not fancy but private.",
        "cabinet" : "A lockable steel cabinet.",
        "window"  : "Narrow window. The alley behind the hostel is quiet.",
        "keypad"  : "A numeric keypad lock. Rental rate posted on the door."
    ]) );
}

void reset() { ::reset(); }
