/* /domains/Praxis/areas/splynn/alvurron/alvurron_warrens.c
   Alvurron, the Warrens. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, the Warrens");
    set_long(
        "Gargoyle dwellings honeycomb the black stone here, entrances cut\n"
        "at every height with no regard for stairs. Rope bridges and iron\n"
        "rungs serve those who cannot fly. Eyes track you from dark\n"
        "doorways, unblinking and patient. The quarter is east. A red\n"
        "glow and a rhythmic pounding come from the south. A crooked\n"
        "stair descends northwest into the undermarket.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/splynn/alvurron/alvurron_quarter",
        "south" : "/domains/Praxis/areas/splynn/alvurron/alvurron_blood_pit",
        "northwest" : "/domains/Praxis/areas/splynn/alvurron/alvurron_undermarket"
    ]) );
    set_listen("default",
        "Wing-beats overhead and the mutter of gargoyle voices in the stone.");
    set_smell("default", "Cold rock, guano, and smoke.");
    set_items( ([
        "dwellings" : "Cell after cell cut into living rock. Some have been
occupied for centuries.",
        "bridges" : "Rope and iron, strung between entrances. They creak.",
        "eyes" : "They do not blink. They do not look away."
    ]) );
}

void reset() {
    ::reset();
    if(!present("gargoyle watcher", this_object()))
        clone_object("/domains/Praxis/monsters/alvurron_watcher")->move(this_object());
}
