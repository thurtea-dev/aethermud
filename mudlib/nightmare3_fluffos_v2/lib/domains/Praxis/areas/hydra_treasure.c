// /domains/Praxis/areas/hydra_treasure.c
// Hidden recess behind the hydra lair rock.

#include <std.h>
#include <rooms.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Hidden Recess");
    set_long(
        "A narrow recess behind the displaced rock. The air is stale and\n"
        "cold. Pre-Rifts stonework lines the walls, carved with symbols\n"
        "that predate the coming of the Rifts.\n\n"
        "Something glints in the shadows.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/hydra_lair"
    ]) );
    set_items( ([
        "symbols" : "Ancient carvings, worn smooth by time.",
        "shadows" : "Deep shadows cling to the corners of the recess."
    ]) );
}

void reset() {
    ::reset();
    if(!present("obsidian blade", this_object()) &&
       !present("blade", this_object()))
        clone_object(EQ_PATH + "obsidian_blade.c")->move(this_object());
}
