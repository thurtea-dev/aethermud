// /d/Praxis/inn107.c
// Survivors' Hostel, Private Room 1. First room off the rental hallway.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Private Room 1");
    set_long(
        "A small private room with a single bed, a lockable cabinet, and\n"
        "a hook on the door for gear. Cleaner and quieter than the bunk\n"
        "rooms. The door has a keypad lock. Currently unoccupied.\n\n"
        "The main hallway is back southwest. The rental hall continues\n"
        "north, past three more doors.");
    set_exits( ([
        "southwest" : "/domains/Praxis/inn102",
        "north"     : "/domains/Praxis/inn108"
    ]) );
    set_items( ([
        "bed"     : "A single bed with a real mattress. More comfortable than the bunks.",
        "cabinet" : "A lockable steel cabinet for personal storage.",
        "hook"    : "A door hook for hanging armor or a pack.",
        "keypad"  : "A numeric keypad lock. This room rents by the night."
    ]) );
}

void reset() { ::reset(); }
