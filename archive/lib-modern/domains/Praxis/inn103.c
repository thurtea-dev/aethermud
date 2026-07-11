// /d/Praxis/inn103.c
// Survivors' Hostel, Bunk Room A. Type rest to recover in this safe room.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("rest_allowed", 1);
    set_short("Survivors' Hostel, Bunk Room A");
    set_long(
        "Eight steel-framed bunk beds line the walls, each with a thin\n"
        "mattress and a single blanket. Hooks above each bunk hold gear.\n"
        "A narrow aisle runs between the rows. The room is dark except\n"
        "for a low-wattage red safety light near the exit.\n\n"
        "Bunks are first-come. No reservations. Type 'rest' to sleep.\n"
        "Rest here slowly restores HP and SDC in safe areas.");
    set_exits( ([ "south" : "/domains/Praxis/inn102" ]) );
    set_listen("default",
        "The breathing of other survivors, and the faint hum of the city outside.");
    set_smell("default",
        "Sweat, canvas, and the faintly metallic smell of people who work hard.");
    set_items( ([
        "bunks"    : "Steel-framed. Thin mattress. A blanket. Enough.",
        "bunk"     : "Steel-framed. Thin mattress. A blanket. Enough.",
        "mattress" : "Thin foam, worn to the springs in a few spots. Still functional.",
        "blanket"  : "Military-issue wool. Heavy and warm.",
        "hooks"    : "Metal hooks above each bunk for hanging armor and gear.",
        "light"    : "A single red safety light near the exit. Enough to navigate by.",
        "aisle"    : "Narrow walkway between the bunk rows."
    ]) );
}

void reset() { ::reset(); }
