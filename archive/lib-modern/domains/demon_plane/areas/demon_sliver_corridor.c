// /domains/demon_plane/areas/demon_sliver_corridor.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Bone Corridor");
    set_long(
        "A natural tunnel through fused bone and stone. Rib arches curve overhead,\n"
        "each one larger than a Coalition APC. Bioluminescent fungus casts sickly\n"
        "green light. The floor is worn smooth by something heavy passing often.");
    set_exits( ([
        "east" : "/domains/demon_plane/areas/demon_sliver_wastes",
        "north" : "/domains/demon_plane/areas/demon_sliver_shrine",
        "west" : "/domains/demon_plane/areas/demon_sliver_depths"
    ]) );
}

void reset() {
    ::reset();
    if(!present("demon sliver hunter", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_hunter")->move(this_object());
}
