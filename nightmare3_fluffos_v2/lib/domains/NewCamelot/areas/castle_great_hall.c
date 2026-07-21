/* /domains/NewCamelot/areas/castle_great_hall.c
   Camelot Castle - Great Hall. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Camelot Castle - Great Hall");
    set_long(
        "The great hall of Camelot Castle. Long tables run the length of\n"
        "the room beneath iron chandeliers, and the walls carry tapestries\n"
        "of the Order's deeds: the raising of the walls, the pact with the\n"
        "dragons, the breaking of a demon horde at the Channel. The throne\n"
        "room lies north behind gilded doors. The war room is east and the\n"
        "courtyard south.");
    set_listen("default",
        "Echoes. The hall remembers feasts.");
    set_smell("default",
        "Candle wax and old woodsmoke.");
    set_items( ([
        "tapestries" : "Woven history. In one, a knight takes a young dragon as sworn brother rather than trophy.",
        "tables" : "Oak, seating two hundred at need.",
        "chandeliers" : "Iron rings of candles, raised and lowered on chains."
    ]) );
    set_exits( ([
        "south" : "/domains/NewCamelot/areas/castle_courtyard",
        "north" : "/domains/NewCamelot/areas/castle_throne_room",
        "east" : "/domains/NewCamelot/areas/castle_war_room"
    ]) );
}
