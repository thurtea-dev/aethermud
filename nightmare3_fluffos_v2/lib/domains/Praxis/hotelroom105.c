/* /d/Praxis/hotelroom105.c
   Praxis Refuge Inn guest room 105 (deluxe).
   Rented through the receptionist in hotel.c; door key checked by hall3.c. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( ([
        "light" : 2,
        "indoors" : 1,
        "no attack" : 1,
        "no steal" : 1,
        "no castle" : 1,
        "rest_allowed" : 1
    ]) );
    set_short("Praxis Refuge Inn, Room 105");
    set_long(
        "A deluxe guest room in the Praxis Refuge Inn. A full-sized bed\n"
        "with a thick quilt dominates the room. There is a writing desk\n"
        "with a working lamp, a wash alcove with a filtered water tap,\n"
        "and an armored window with real glass. Somebody put thought\n"
        "into making this room feel like the world never ended.\n\n"
        "The hallway is out.");
    set_items( ([
        "bed" : "A full-sized bed with a thick quilt. Genuine luxury.",
        "desk" : "A writing desk with paper, pen, and a working lamp.",
        "lamp" : "An honest-to-goodness electric lamp, wired to the inn\n"
                 "generator.",
        "alcove" : "A wash alcove with a basin and a filtered water tap.",
        "tap" : "Filtered water, safe to drink. A rarity.",
        "window" : "Armored glass. The city lights of Praxis glitter beyond."
    ]) );
    set_exits( ([ "out" : "/domains/Praxis/hall3" ]) );
}
