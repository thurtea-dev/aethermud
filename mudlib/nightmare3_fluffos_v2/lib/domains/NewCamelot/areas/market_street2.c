/* /domains/NewCamelot/areas/market_street2.c
   New Camelot - Market Street East. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("New Camelot - Market Street East");
    set_long(
        "The east end of Market Street. The crowd thins here where the\n"
        "street opens toward the training grounds to the south. The royal\n"
        "stables stand north, sweet with hay. The Chapel of Light rises to\n"
        "the east, its spire catching the sun. Market Street continues west.");
    set_listen("default",
        "Horses whinnying to the north, sword drills to the south.");
    set_smell("default",
        "Hay, horse, and incense drifting from the chapel.");
    set_items( ([
        "spire" : "The chapel spire, tiled in salvaged white ceramic.",
        "crowd" : "Thinner here. Squires and stable hands going about their business."
    ]) );
    set_exits( ([
        "west" : "/domains/NewCamelot/areas/market_street1",
        "north" : "/domains/NewCamelot/areas/stables",
        "east" : "/domains/NewCamelot/areas/chapel_of_light",
        "south" : "/domains/NewCamelot/areas/training_grounds"
    ]) );
}
