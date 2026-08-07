#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no castle", 1);
    set_short("Monastery stairwell");
    set_long(
        "A spiraling stone stairwell in the old Praxis monastery. Steps lead\n"
        "up to a sealed attic and down toward the immortal hall. The prayer\n"
        "chamber lies west.");
    set_exits( ([
        "west" : "/domains/Praxis/monastery",
        "up"   : "/domains/Praxis/monk_join",
        "down" : "/domains/Praxis/immortal_hall"
    ]) );
    set_items( ([
        "stairs" : "Worn stone steps circling the monastery tower."
    ]) );
}
