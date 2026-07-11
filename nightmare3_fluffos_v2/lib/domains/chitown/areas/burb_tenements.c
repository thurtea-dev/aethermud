// /domains/chitown/areas/burb_tenements.c
// Crowded tenement block west of shanty row.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("Burb Tenements");
    set_long(
        "Stacked living boxes rise three stories on either side of a mud\n"
        "lane. Balconies sag under laundry and scavenged solar panels. A\n"
        "narrow stair climbs to the rooftops. The smell of too many people\n"
        "in too little space hangs in the air.\n\n"
        "Shanty row is east. A soup line forms to the north. West leads\n"
        "toward the wall's shadow.");
    set_exits( ([
        "east"  : "/domains/chitown/areas/chitown_burb_shanty",
        "north" : "/domains/chitown/areas/burb_soup_kitchen",
        "west"  : "/domains/chitown/areas/burb_wall_shadow",
        "up"    : "/domains/chitown/areas/burb_rooftops"
    ]) );
    set_listen("default",
        "Arguments through thin walls, a baby crying, distant patrol speakers.");
    set_smell("default",
        "Cooking grease, damp plaster, and open drains.");
    set_items( ([
        "balconies" : "Overloaded with laundry, crates, and jury-rigged antennas.",
        "stair" : "A metal stair bolted to the outer wall, climbing to the roofs.",
        "panels" : "Cracked solar panels wired into apartment windows."
    ]) );
}
