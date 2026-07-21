// /domains/NewCamelot/areas/camelot_gate.c
// New Camelot's old outer gate. No longer the Europe start room (see
// newcamelot_start.c, 2026-07-21) -- now a quiet passage just past the
// gatehouse. Faction/infamy checkpoint logic moved to the gatehouse.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("New Camelot, Great Gate");
    set_long(
        "Stone walls draped with heraldic banners rise on either side of the\n"
        "great gates of New Camelot. The banners show a white dragon on a\n"
        "blue field, the emblem of the Camelot Order.\n\n"
        "The gatehouse lies through the archway to the west.");
    set_exits( ([
        "west" : "/domains/NewCamelot/areas/newcamelot_start"
    ]) );
    set_listen("default",
        "Pennants snapping in the wind. Distant bells from the city interior.");
    set_smell("default",
        "Stone dust, horse, and the subtle electric tang of ambient magical energy.");
    set_items( ([
        "banners" : "Heraldic banners showing a white dragon on blue. New Camelot.",
        "gate" : "Great stone gates bound with iron. Open to travelers.",
        "spires" : "The tower of the Camelot Order, visible above the roofline.",
        "stones" : "Old stones. Very old. They predate the Rifts by centuries."
    ]) );
}
