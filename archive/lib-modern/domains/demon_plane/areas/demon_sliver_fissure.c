// /domains/demon_plane/areas/demon_sliver_fissure.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Bleeding Fissure");
    set_long(
        "A deep crack in the basalt weeps glowing orange magma. The air shimmers\n"
        "with heat. Scorch marks and claw gouges line the walls. Whatever lives\n"
        "here treats the fissure as a hunting ground.");
    set_exits( ([
        "south" : "/domains/demon_plane/areas/demon_sliver_wastes",
        "down" : "/domains/demon_plane/areas/demon_sliver_pit",
        "east" : "/domains/demon_plane/areas/demon_sliver_nest"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver stalker", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_stalker")->move(this_object());
}
