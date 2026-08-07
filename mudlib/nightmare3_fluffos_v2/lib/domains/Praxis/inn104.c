// /d/Praxis/inn104.c
// Survivors' Hostel, Bunk Room B. Type rest to recover in this safe room.

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
    set_short("Survivors' Hostel, Bunk Room B");
    set_long(
        "The second bunk room is quieter than the first. Six bunks here\n"
        "instead of eight, giving a bit more room between them. A small\n"
        "window near the ceiling lets in a sliver of amber sky. Someone\n"
        "has taped a hand-drawn map of safe routes to the wall beside\n"
        "the exit.\n\n"
        "Type 'rest' to sleep and recover. Rest slowly restores HP and SDC\n"
        "while you remain in this safe bunk room.");
    set_exits( ([ "north" : "/domains/Praxis/inn102" ]) );
    set_listen("default",
        "Quiet. The occasional sound of movement from the hallway.");
    set_smell("default",
        "Concrete and the faint smell of whatever the previous occupant ate.");
    set_items( ([
        "bunks"    : "Six steel-framed bunks. More space between them than in Room A.",
        "bunk"     : "Steel-framed with a thin mattress. Gets the job done.",
        "window"   : "A narrow window near the ceiling. The sky outside is amber. "
                     "Ley line interference colors it that way most days.",
        "map"      : "A hand-drawn map taped to the wall. Safe routes out of Praxis, "
                     "marked in pencil. Whoever drew this knew the area.",
        "mattress" : "Thin but functional.",
        "blanket"  : "Wool blanket, military issue. Standard hostel kit."
    ]) );
}

void reset() { ::reset(); }
