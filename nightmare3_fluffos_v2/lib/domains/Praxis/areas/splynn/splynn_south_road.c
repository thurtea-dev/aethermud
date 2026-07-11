// /d/Praxis/areas/splynn/splynn_south_road.c
// South road below Rocky's Bar, connecting to the Preserves and west road.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("South Road, Splynn");
    set_long(
        "A wide stone road running south from Rocky's Bar, flanked by low\n"
        "walls of worked bone and obsidian. The buildings on either side\n"
        "are Splugorth construction: organic curves and jutting angles that\n"
        "feel wrong in ways you cannot quite describe.\n\n"
        "Traffic here is lighter than the main market. A few figures move\n"
        "with purpose and do not make eye contact. To the west, a narrower\n"
        "road branches off toward the outer edge of the city. To the south,\n"
        "the structures give way to open ground: the beginning of the\n"
        "Splynn Preserves.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/rockys_bar_splynn",
        "west" : "/domains/Praxis/areas/splynn/splynn_west_road",
        "south" : "/domains/Praxis/areas/splynn/splynn_preserves",
        "down"  : "/domains/Praxis/areas/splynn/alvurron/alvurron_gate"
    ]) );
    set_listen("default",
        "The distant hum of the market, footsteps on stone, and from the\n"
        "south, something large moving through heavy undergrowth.");
    set_smell("default",
        "The damp stone smell of Splugorth construction and something earthy\n"
        "from the Preserves to the south.");
    set_items( ([
        "walls" : "Low walls of shaped bone fitted with obsidian panels.\n"
                     "The construction is older than it looks.",
        "buildings": "Splugorth architecture. Organic and angular at the same time.\n"
                     "The windows, where there are windows, are narrow and dark.",
        "figures" : "A few figures moving fast with eyes down.\n"
                     "Nobody wants to be noticed on this street.",
        "road" : "Wide stone road, worn smooth. Pre-Rifts construction\n"
                     "repurposed by Splugorth expansion."
    ]) );
}

void reset() {
    ::reset();
}
