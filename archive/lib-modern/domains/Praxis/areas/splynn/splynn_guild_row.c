// /domains/Praxis/areas/splynn/splynn_guild_row.c
// Guild and clan offices south of the east bazaar.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("Guild Row, Splynn");
    set_long(
        "A stone corridor lined with fortified doors bearing clan sigils,\n"
        "mercenary company plaques, and Splugorth administrative seals.\n"
        "Armed guards stand at several entrances. This is where deals are\n"
        "made that never appear in the open market.\n\n"
        "The east bazaar is north. The clan hall lies west.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/splynn_east_bazaar",
        "west"  : "/domains/Praxis/areas/splynn/splynn_clan_hall"
    ]) );
    set_listen("default",
        "Muffled negotiations behind heavy doors. Boots on stone.");
    set_smell("default",
        "Incense, weapon oil, and cold stone.");
}
