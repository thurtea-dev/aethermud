/* /domains/Praxis/areas/splynn/alvurron/alvurron_roost.c
   Alvurron, the High Roost. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, the High Roost");
    set_long(
        "The upper reaches of Alvurron open into a vast vault where the\n"
        "city's gargoyles roost. Perches of black stone jut from the walls\n"
        "in tiers that vanish into darkness overhead. Wing-folded shapes\n"
        "line them like grotesque statuary, until one turns its head. The\n"
        "plaza is down.");
    set_exits( ([
        "down" : "/domains/Praxis/areas/splynn/alvurron/alvurron_square"
    ]) );
    set_listen("default",
        "The leathery rustle of hundreds of folded wings.");
    set_smell("default", "Dust, guano, and warm stone.");
    set_items( ([
        "perches" : "Tier upon tier of stone perches, rising out of sight.",
        "shapes" : "Roosting gargoyles. Dozens. Perhaps hundreds. Best not
to count.",
        "vault" : "The ceiling is somewhere above, in the dark."
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle watcher", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_watcher")->move(this_object());
}
