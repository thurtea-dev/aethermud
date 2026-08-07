/* /d/Praxis/hotelroom101.c
   Praxis Refuge Inn guest room 101 (regular).
   Rented through the receptionist in hotel.c; door key checked by hall.c. */

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
    set_short("Praxis Refuge Inn, Room 101");
    set_long(
        "A small, clean guest room in the Praxis Refuge Inn. A narrow bed\n"
        "with fresh linens stands against the wall beside a steel foot\n"
        "locker. A battery lantern on the nightstand gives warm light,\n"
        "and a shuttered window looks out over the Praxis rooftops. It\n"
        "is not much, but the door locks and the sheets are clean.\n\n"
        "The hallway is out.");
    set_items( ([
        "bed" : "A narrow but comfortable bed with clean linens.",
        "locker" : "A steel foot locker for your gear. It is empty.",
        "lantern" : "A battery lantern, dented but bright.",
        "window" : "Shuttered against the night. The rooftops of Praxis\n"
                   "stretch away outside.",
        "nightstand" : "A small table beside the bed."
    ]) );
    set_exits( ([ "out" : "/domains/Praxis/hall" ]) );
}
