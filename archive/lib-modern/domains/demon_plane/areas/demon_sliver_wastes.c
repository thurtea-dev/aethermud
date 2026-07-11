// /domains/demon_plane/areas/demon_sliver_wastes.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Ash Wastes");
    set_long(
        "Gray ash covers everything in drifts knee-deep in places. Basalt spires\n"
        "jut from the ground like broken teeth. Green lightning crawls across\n"
        "a sky that has no sun. The heat is dry and relentless.");
    set_exits( ([
        "south" : "/domains/demon_plane/areas/demon_sliver_gate",
        "north" : "/domains/demon_plane/areas/demon_sliver_fissure",
        "west" : "/domains/demon_plane/areas/demon_sliver_corridor"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver hunter", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_hunter")->move(this_object());
}
