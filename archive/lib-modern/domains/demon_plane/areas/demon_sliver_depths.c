// /domains/demon_plane/areas/demon_sliver_depths.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Deep Hollow");
    set_long(
        "The tunnel opens into a vast hollow beneath the sliver. Stalactites of\n"
        "black glass hang overhead. Far below, something moves in the dark.\n"
        "This place feels older than the rift that opened above it.");
    set_exits( ([
        "east" : "/domains/demon_plane/areas/demon_sliver_corridor",
        "south" : "/domains/demon_plane/areas/demon_sliver_scar"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver alpha", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_alpha")->move(this_object());
}
