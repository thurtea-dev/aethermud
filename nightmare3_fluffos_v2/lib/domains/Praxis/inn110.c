// /d/Praxis/inn110.c
// Survivors' Hostel, Private Room 4. Locked. Future rental.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Private Room 4");
    set_long(
        "The end room of the hostel hallway. A single bed, a lockable\n"
        "cabinet, and a reinforced door with a keypad lock. The quietest\n"
        "room in the building. Currently unoccupied.");
    set_exits( ([ "out" : "/domains/Praxis/inn102" ]) );
    set_items( ([
        "bed"     : "A single bed. Quieter here than anywhere else in the hostel.",
        "cabinet" : "A lockable steel cabinet.",
        "door"    : "Reinforced steel door with a keypad lock. Good security.",
        "keypad"  : "A numeric keypad lock."
    ]) );
}

void reset() { ::reset(); }
