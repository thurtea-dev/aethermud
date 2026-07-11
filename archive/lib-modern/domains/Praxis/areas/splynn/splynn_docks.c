// /d/Praxis/areas/splynn/splynn_docks.c
// The Splynn Docks: on the Atlantic coast, Splugorth barges moored here.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 0);
    set_short("Splynn Docks");
    set_long(
        "A massive stone quay juts into the grey Atlantic. Splugorth barges\n"
        "are moored in rows, black-hulled vessels the size of city blocks,\n"
        "their superstructures crowned with alien architecture. Crane-arms\n"
        "of bone and metal work in silence, unloading cargo that twitches.\n\n"
        "The dockworkers are a mix of Kittani overseers and enslaved beings\n"
        "doing the heavy lifting. The smell of brine mixes with something\n"
        "acrid and metallic. In the distance, the open Atlantic is vast\n"
        "and indifferent.\n\n"
        "Exits: south back to the Splynn Dimensional Market; north toward\n"
        "the open ocean where a dimensional rift glows on the horizon.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/splynn_market",
        "north" : "/domains/Praxis/areas/splynn/ocean_rift"
    ]) );
    set_listen("default",
        "The lap of waves, the groan of mooring lines under tension, and\n"
        "the mechanical hum of crane arms working in an inhuman rhythm.");
    set_smell("default",
        "Atlantic brine, engine coolant, and something biological that\n"
        "should not be loaded as cargo.");
    set_items( ([
        "barges" : "Enormous black-hulled Splugorth transport vessels. Each one is a\n"
                        "mobile command center and cargo hold. The hulls are MDC-rated.",
        "cranes" : "Crane-arms of shaped bone and worked metal, moving silently.\n"
                        "Their precision is unsettling.",
        "quay" : "Massive stone blocks fitted without mortar. Pre-Rifts construction,\n"
                        "heavily modified with Splugorth additions.",
        "ocean" : "The Atlantic. Vast and grey. Whatever was here before the Rifts\n"
                        "is long gone.",
        "cargo" : "Crates, containers, and things that move. Better not to look too closely.",
        "dockworkers" : "Kittani overseers with shock prods moving around enslaved beings.\n"
                        "Nobody looks at you for long."
    ]) );
}

void reset() {
    ::reset();
    if(!present("kittani overseer", this_object()))
        clone_object("/domains/Praxis/monsters/kittani_overseer")->move(this_object());
}
