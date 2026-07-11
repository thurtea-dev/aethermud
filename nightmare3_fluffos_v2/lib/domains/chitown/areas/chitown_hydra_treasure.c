// /domains/chitown/areas/chitown_hydra_treasure.c
// Hidden recess north of the Chi-Town hydra lair.

#include <std.h>
#include <rooms.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Hidden Recess, Chi-Town Sewers");
    set_long(
        "A narrow recess behind the displaced rock. Cold stale air and the\n"
        "drip of distant water. Pre-Rifts masonry lines the walls.\n\n"
        "Metal glints in the shadows - more than one shape.");
    set_exits( ([
        "south" : "/domains/chitown/areas/chitown_hydra_lair"
    ]) );
}

void reset() {
    ::reset();
    if(!present("obsidian blade", this_object()))
        clone_object(EQ_PATH + "obsidian_blade.c")->move(this_object());
    /* Memories: metal hilt charged with PPE for a flame blade. */
    if(!present("flame hilt", this_object()))
        clone_object(EQ_PATH + "flame_hilt.c")->move(this_object());
}
