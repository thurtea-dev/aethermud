// /d/Praxis/inn109.c
// Survivors' Hostel, Private Room 3. Locked. Future rental.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Private Room 3");
    set_long(
        "A small private room, slightly larger than the others. Two single\n"
        "beds, a table, and two lockable cabinets. Designed for pairs.\n"
        "The door has a keypad lock. Currently unoccupied.");
    set_exits( ([ "out" : "/domains/Praxis/inn102" ]) );
    set_items( ([
        "beds"    : "Two single beds. Enough space for a two-person team.",
        "table"   : "A small folding table. Enough room to spread a map.",
        "cabinet" : "Two lockable steel cabinets, one per occupant.",
        "keypad"  : "A numeric keypad lock."
    ]) );
}

void reset() { ::reset(); }
